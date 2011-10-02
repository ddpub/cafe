
#if !defined(_UNIX_BUILD_) && !defined(_WIN32_BUILD_)
#error define _UNIX_BUILD_ or _WIN32_BUILD_
#endif

#include <winsock2.h>
#include <ws2tcpip.h>   
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ksi_include/monitor.hpp>
#include <printer/printer.hpp>
#include <printer/xml_check_printer.hpp>
#include <serial/com.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ksi_util/xml_config.hpp>
#include <fstream>
#include <ksi_util/logger.hpp>
#include <boost/algorithm/string.hpp>

#define TIXML_USE_STL
#include <tinyxml/tinyxml.h>


namespace version {
extern std::string version;
extern std::string compamny;
extern std::string product;
extern std::string copyright;
extern std::string name;
};


std::string logDir = "receipts";
std::string logName = version::name + "_v" + version::version;

std::auto_ptr<ksi_logger::logger_t> log_ptr;
ksi_logger::logger_t app_err(new ksi_logger::file_log_t(version::name + "_" + version::version + "_err.log", 10));

using boost::asio::ip::tcp;
const int max_length = 8192;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

std::map<socket_ptr, std::vector<char> > raw_;
std::map<socket_ptr, std::string> response_;

boost::mutex raw_mutex_;

MonitorQueue<std::pair<std::string, std::string> >  queue_;

std::auto_ptr<base_printer_t> printer;

void add_job(const std::string& message, const std::string& addr) {
	queue_.push(std::make_pair(message, addr));
}

void response(const std::string& message, socket_ptr sock) {
	try {
		char msg[1024];
		for (size_t i=0; i<1024; ++i) {
			if (i < message.length()) msg[i] = message[i];
			else msg[i] = 0;
		}
		boost::asio::write(*sock, boost::asio::buffer(msg, 1024));
	}
	catch (std::exception& ex) {
		*log_ptr << "response error: " << ex.what() << std::endl;
	}
}

std::vector<char> process(const std::vector<char>& cmd, socket_ptr sock) {
	if (cmd.size() > 14) {
		char size_str[4] = {cmd[4], cmd[5], cmd[6], cmd[7] };
		size_t size = *reinterpret_cast<size_t*>(size_str);

		if (cmd.size() == size + 8) { // Valid size
			std::string message(cmd.begin()+8, cmd.end());
			TiXmlDocument xml(logDir + "/receipt."+boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time())+".xml");
			xml.ClearError();
			xml.Parse(message.c_str());
			xml.SaveFile();
			if (xml.Error()) {
				*log_ptr << xml.ErrorDesc() << std::endl;
				xml.SaveFile("error_receipt.xml");
				response_[sock] = "failed";
			}
			else {
				add_job(message, boost::lexical_cast<std::string>(sock->remote_endpoint().address()));
				response_[sock] = "successful";
			}
		}
	}
	return cmd;
}

void switcher(const std::vector<char>& buff, socket_ptr sock) {
	boost::mutex::scoped_lock lock(raw_mutex_);

	std::vector<char> t = raw_[sock];
	std::copy(buff.begin(), buff.end(), std::back_inserter(t));
	raw_[sock] = t;
	raw_[sock] = process(raw_[sock], sock);
}

void session(socket_ptr sock) {
	try {
		for (;;) {
			std::vector<char> buff(max_length);
			boost::system::error_code error;
			size_t length = sock->read_some(boost::asio::buffer(buff), error);
			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			buff.resize(length);
			switcher(buff, sock);
			if (response_[sock] == "failed" || response_[sock] == "successful") {
				response(response_[sock], sock);
				break;
			}
		}
	}
	catch (std::exception& ex) {
		*log_ptr << "exception in thread: " << ex.what() << std::endl;
	}
}

void process_job() {
	for (;;) {
		std::pair<std::string, std::string> pr = queue_.popWait();
		std::string message = pr.first;
		std::string addr = pr.second;
		
		check_printer_t p(printer.get());
		TiXmlDocument xml;
		xml.Parse(message.c_str());
		
		ksi_logger::continue_t log_cont = *log_ptr << "printing receipt from " << addr << "...";

		for ( TiXmlNode* node = xml.FirstChild("receipt")->FirstChild(); node != 0; node = node->NextSibling()) {
			if (node->Type() != TiXmlNode::ELEMENT) continue;
			if (node->ValueStr() == "bell") p.bell();
			else if (node->ValueStr() == "open_drawer") p.open_drawer();
			else if (node->ValueStr() == "test") p.test();
			else if (node->ValueStr() == "lf") p.lf();
			else if (node->ValueStr() == "cut") p.cut();
			else if (node->ValueStr() == "logo") p.logo();
			else if (node->ValueStr() == "fl") {
				std::map<std::string, std::string> attr_map;
				{
					TiXmlAttribute* attr = node->ToElement()->FirstAttribute();
					while (attr) {
						attr_map[attr->NameTStr()] = attr->ValueStr();
						attr = attr->Next();
					}
				}
				if (attr_map["font"] == "normal") {
					p.set_font(xml_attr::normal);
				}
				else if (attr_map["font"] == "hx2") {
					p.set_font(xml_attr::hx2);
				}
				else if (attr_map["font"] == "hx2wx2") {
					p.set_font(xml_attr::hx2wx2);
				}
				p.fl();
			}
			else if (node->ValueStr() == "table") {
				std::vector<row_t> rows;
				for (TiXmlNode* row_node = node->FirstChild(); row_node != 0; row_node = row_node->NextSibling()) {
					row_t cols;
					for (TiXmlNode* col_node = row_node->FirstChild(); col_node != 0; col_node = col_node->NextSibling()) {
						std::map<std::string, std::string> attr_map; 
						{
							TiXmlAttribute* attr = col_node->ToElement()->FirstAttribute();
							while (attr) {
								attr_map[attr->NameTStr()] = attr->ValueStr();
								attr = attr->Next();
							}
						}
						bool multiline = true;
						if (attr_map["multiline"] == "") multiline = false;
						bool stretch = true;
						if (attr_map["stretch"] == "") stretch = false;
						xml_attr::align_t align = xml_attr::left;
						if (attr_map["align"] == "right") align = xml_attr::right;
						else if (attr_map["align"] == "center") align = xml_attr::center;
						size_t lspace = 0;
						if (attr_map["lspace"] != "") lspace = boost::lexical_cast<size_t>(attr_map["lspace"]);
						size_t rspace = 0;
						if (attr_map["rspace"] != "") rspace = boost::lexical_cast<size_t>(attr_map["rspace"]);
						xml_attr::font_t font = xml_attr::normal;
						if (attr_map["font"] == "hx2") font = xml_attr::hx2;
						else if (attr_map["font"] == "hx2wx2") font = xml_attr::hx2wx2;
						size_t lf = 0;
						if (attr_map["lf"] != "") lf = boost::lexical_cast<size_t>(attr_map["lf"]);

						std::string text;
						for (TiXmlNode* find_text_node = col_node->FirstChild(); find_text_node != 0; find_text_node = find_text_node->NextSibling()) {
							if (find_text_node->Type() != TiXmlNode::TEXT) continue;
							text = find_text_node->ToText()->ValueStr();
							break;
						}
						cols.push_back(column_t(text, multiline, stretch, align, lspace, rspace, font, lf));
					}
					rows.push_back(cols);
				}
				if (rows.size()) p.table(xml_table_t(rows));
			}
		}
		log_cont << "...successful";
	}
}

void server(boost::asio::io_service& io_service, short port) {
	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
	for (;;) {
		socket_ptr sock(new tcp::socket(io_service));
		a.accept(*sock);
		boost::thread thr(boost::bind(session, sock));
	}
}

void save_default_config(const std::string& config_file) {
	std::ofstream out(config_file.c_str());
	out << "<?xml version=\"1.0\" encoding=\"windows-1251\" ?>" << std::endl;
	out << "	<server port=\"703\"/>" << std::endl;
	out << "	<log dir=\"/var/local/ksi/log/\" file=\"\"/>" << std::endl;
	out << "	<printer1 port=\"COM1\" baud_rate=\"9600\" command_set=\"STAR\"/>" << std::endl;
}

int main(int argc, char* argv[]) {
	std::cout << version::product << " v" << version::version << std::endl;
	std::cout << version::copyright << std::endl;
	
	std::string config_file = "";
	if (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
		std::cout << "Usage: " << argv[0] << " [OPTION]" << std::endl << std::endl;
		std::cout << "Mandatory arguments to long options are mandatory for short options too." << std::endl;
		std::cout << "  -c FILE, --config=FILE   Read FILE as a config file" << std::endl;
		std::cout << "  -g FILE, --generate=FILE Generate default config file" << std::endl;
		std::cout << "  -h, --help               display this help and exit" << std::endl;
		std::cout << "  -v, --version            output version information and exit" << std::endl;
		return 0;
	}
	else if (argc == 2 && (std::string(argv[1]) == "--version" || std::string(argv[1]) == "-v")) return 0;
	else if (argc == 3 && std::string(argv[1]) == "-c") config_file = argv[2];
	else if (argc == 2 && std::string(argv[1]).find("--config-file=") == 0) config_file = std::string(argv[1] + sizeof("--config-file=") - 1);
	else if (argc == 3 && std::string(argv[1]) == "-g") {
		save_default_config(argv[2]);
		return 0;
	}
	else if (argc == 2 && std::string(argv[1]).find("--generate=") == 0) {
		save_default_config(std::string(argv[1] + sizeof("--generate=") -1));
		return 0;
	}
	else if (argc == 1) {
		config_file = "print_server.xml";
	}

	if (config_file == "") {
		std::cerr << argv[0] << ": missing FILE operand" << std::endl;
		app_err << argv[0] << ": missing FILE operand" << std::endl;
		std::cerr << "Try `" << argv[0] << " --help' for more information." << std::endl;
		app_err << "Try `" << argv[0] << " --help' for more information." << std::endl;
		return 1;
	}
	
	std::ifstream in(config_file.c_str());
	if (!in) {
		std::cerr << argv[0] << ": missing destination file operand after `" << config_file << "'" << std::endl;
		app_err << argv[0] << ": missing destination file operand after `" << config_file << "'";
		std::cerr << "Try `" << argv[0] << " --help' for more information." << std::endl;
		app_err << "Try `" << argv[0] << " --help' for more information.";
		return 1;
	}

	
	try {
		xml_config_t config(config_file);
		int server_port = 703;
		if (config.get_attr("server", "port") != "")
			server_port = boost::lexical_cast<int>(config.get_attr("server", "port"));
		int baud_rate = 9600;
		if (config.get_attr("printer1", "baud_rate") != "")
			baud_rate = boost::lexical_cast<int>(config.get_attr("printer1", "baud_rate"));
		std::string command_set = boost::to_lower_copy(config.get_attr("printer1", "command_set"));
		std::string port = config.get_attr("printer1", "port");
		
		std::string log_dir = config.get_attr("log", "dir") + (config.get_attr("log", "dir") == "" ? "" : "/");
		logDir = log_dir + logDir;
		std::string log_file = config.get_attr("log", "file");
		if (log_file == "") log_file = logName;
		
		try {
			log_ptr.reset(new ksi_logger::logger_t(new ksi_logger::file_log_t(log_dir + log_file, 10)));
		}
		catch (std::exception& ex) {				
			app_err << ex.what();
			return 3;
		}
		
		std::auto_ptr<com_t> com_ptr;
		if (command_set == "star") {
			com_ptr.reset(new com_t(port, baud_rate));
			printer.reset(new star_printer_t(*com_ptr));
		}
		else if (command_set == "epson") {
			com_ptr.reset(new com_t(port, baud_rate));
			printer.reset(new epson_printer_t(*com_ptr));
		}
		else if (command_set == "null") {
			printer.reset(new null_printer_t());
		}
		else {
			*log_ptr << "Invalid command_set: [" << command_set << "]" << std::endl;
			return 3;
		}
		boost::thread trh(boost::bind(process_job));
		boost::asio::io_service io_service;
		server(io_service, server_port);
		trh.join();
	}
	catch (std::exception& ex) {
		*log_ptr << ex.what();
	}

	return 0;
}
