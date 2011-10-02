#include <memory>
#include <oracle_client/oracle_client.hpp>
#include <oracle_client/ksi_exception.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <ksi_util/xml_config.hpp>
#include <boost/lexical_cast.hpp>
#include <printer/printer.hpp>
#include <fstream>
#include <ksi_util/users.hpp>
#include "sync.hpp"
#include <shared_modules/kitchen_util.hpp>
#include <shared_modules/connect.hpp>
#include <printer/xml_check_printer.hpp>
#include <ksi_util/logger.hpp>
#include <serial/com.hpp>
#include <ksi_include/thread_pulse.hpp>
#include <ksi_util/util.hpp>
#include <locale>
#include <boost/algorithm/string.hpp>
#include <ksi_util/date_time.hpp>
#include <shared_modules/kitchen_receipt.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#ifndef __WIN32__
#include <signal.h>
#include <unistd.h>
#endif // __WIN32__

bool term = false;

int lang = 1;

namespace version {
extern std::string version;
extern std::string compamny;
extern std::string product;
extern std::string copyright;
extern std::string name;
};


std::string logName = version::name + "_v" + version::version;

std::auto_ptr<ksi_logger::logger_t> log_ptr;
ksi_logger::logger_t app_err(new ksi_logger::file_log_t(version::name + "_" + version::version + "_err", 10));

void term_handler(int a)
{ 
	term = true;
	*log_ptr << "application breaked";
}

void process(int interval, boost::function<size_t ()> func) {
	thread_pulse_t pulse(interval);
	while (!term) {
		boost::thread thrd(pulse);
		thrd.join();
		if (term) break;
		func();
	}
}

class connection_factory_t {
public:
	connection_factory_t(const std::string& login_, const std::string& password_, const std::string host_, const std::string& sid_): login_(login_), password_(password_), host_(host_), sid_(sid_) {}
	std::auto_ptr<ksi_client::oracle_session_i> create() {
		*log_ptr << "connection_factory_t::create";
		boost::lock_guard<boost::mutex> g(mutex);
		*log_ptr << "ksi_client::create_session";
		std::auto_ptr<ksi_client::oracle_session_i> session_ptr = ksi_client::create_session();
		session_ptr->connect(login_, password_, host_, sid_);
		return session_ptr;
	}
private:
	std::string login_;
	std::string password_;
	std::string host_;
	std::string sid_;
private:
	mutable boost::mutex  mutex;
	connection_factory_t(const connection_factory_t&);
	connection_factory_t& operator=(const connection_factory_t&);
};

std::auto_ptr<connection_factory_t> factory;

class kitchen_process_t {
public:
	kitchen_process_t(std::auto_ptr<ksi_client::oracle_session_i> session_, ksi_logger::logger_t& log_, base_printer_t& printer_, ksi_cafe::format_date_time_t& format_, ksi_cafe::users_t& users_, std::auto_ptr<ksi_kitchen::container_kitchen_t> container_, bool title_, bool barcode_, bool auto_ready_): session_(session_), log_(log_), printer_(printer_), format_(format_), users_(users_), container_(container_), title_(title_), barcode_(barcode_), auto_ready_(auto_ready_) {}
	size_t run() {
		size_t counter = 0;
		try {
			ksi_kitchen::dish_sync_t dish_sync(session_.get());
			const std::vector<int>& dish_vec = dish_sync.order_id_vec();
			for (size_t i=0; i<dish_vec.size(); ++i) {
				try {
					ksi_cafe::ora_transaction_t trans(session_.get());
					ksi_kitchen::order_t order(session_.get(), dish_vec[i]);
					log_ << "processing order for dish (order_id: #" << dish_vec[i] << ") ...";
					bool res = try_order_register(order, *container_, "linux", auto_ready_);
					log_ << "	... register successful, res = " << res;
					if (res) {
						log_ << "print_kitchen_order";
						std::string xml = print_kitchen_order(format_, users_, *container_, order, title_, barcode_);
						print_xml(&printer_, xml);
						log_ << "	... print successful";
						trans.commit();
						log_ << "	... commit successful";
						++counter;
					}
				}
				catch (exception_t& ex) {
					if (ex.code() == 666) {}
					else if (ex.code() == 777) { log_ << "Can't send receipt to printer"; }
					else if (ex.code() == 776) { log_ << "Invalid response from printer"; }
					else throw;
				}
			}

			
			ksi_kitchen::mod_sync_t mod_sync(session_.get());
			const std::vector<int>& mod_vec = mod_sync.order_id_vec();
				
			for (size_t i=0; i<mod_vec.size(); ++i) {
				try {
					ksi_cafe::ora_transaction_t trans(session_.get());
					ksi_kitchen::order_t order(session_.get(), mod_vec[i]);
					log_ << "processing order for modifier (order_id: #" << mod_vec[i] << ") ...";
					bool res = try_mod_register(order, *container_, "linux", auto_ready_);
					log_ << "	... register successful, res = " << res;
					if (res) {
						std::string xml = print_kitchen_order_for_modifier(format_, users_, *container_, order, title_, barcode_);
						print_xml(&printer_, xml);
						log_ << "	... print successful";
						trans.commit();
						log_ << "	... commit successful";
						++counter;
					}
				}
				catch (exception_t& ex) {
					if (ex.code() == 666) {}
					else if (ex.code() == 777) { log_ << "Can't send receipt to printer"; }
					else if (ex.code() == 776) { log_ << "Invalid response from printer"; }
					else throw;
				}
			}
		}
		catch (ksi_client::connection_problem_exception& ex) {
			log_ << "ksi_client::connection_problem_exception: " << ex.what();
			do {
				try {
					thread_pulse_t pulse(5);
					boost::thread thrd(pulse);
					thrd.join();
					session_ = factory->create();
					container_->setNewSession(session_.get());
					ksi_cafe::session = factory->create();
					log_ << "reconnect successful";
					return counter;
				}
				catch (ksi_client::connection_problem_exception& ex) {
					log_ << "reconnect error. ksi_client::connection_problem_exception: " << ex.what();
				}
				catch (std::exception& ex) {
					log_ << WARNING_IN_LOCATION;
					log_ << ex.what();
				}
				catch (...) {
					log_ << WARNING_IN_LOCATION;
				}
			} while (1);
		}
		catch (exception_t& ex) {
			log_ << WARNING_IN_LOCATION;
			log_ << ex.what();
			throw;
		}
		catch (std::exception& ex) {
			log_ << WARNING_IN_LOCATION;
			log_ << ex.what();
			throw;
		}
		catch (...) {
			log_ << WARNING_IN_LOCATION;
			throw;
		}
		return counter;
	}
private:
	std::auto_ptr<ksi_client::oracle_session_i> session_;
	ksi_logger::logger_t& log_;
	base_printer_t& printer_;
	ksi_cafe::format_date_time_t& format_;
	ksi_cafe::users_t& users_;
	std::auto_ptr<ksi_kitchen::container_kitchen_t> container_;
	bool barcode_;
	bool title_;
	bool auto_ready_;
	bool printMiscCharges_;
private:
	kitchen_process_t(const kitchen_process_t&);
	kitchen_process_t& operator=(const kitchen_process_t&);
};

void save_default_config(const std::string& config_file) {
	std::ofstream out(config_file.c_str());
	out << "<?xml version=\"1.0\" encoding=\"windows-1251\" ?>" << std::endl;
	
	out << "<!--" << std::endl;
	out << "source        - printer. Possible values:" << std::endl;
	out << "	NET: needs remote print server" << std::endl;
	out << "	LOCAL: local receipt printer" << std::endl;
	out << "host          - host of remote print server (required for source=\"NET\")" << std::endl;
	out << "command_set   - local printer command set (required for source=\"LOCAL\"). Possible values:" << std::endl;
	out << "	STAR: for star printer" << std::endl;
	out << "	EPSON: for epson printer" << std::endl;
	out << "	baud_rate - default 9600 (required for source=\"LOCAL\")" << std::endl;
	out << "logo          - print graphic logo  in receipt header. Default 0, set to 0 to disable (not supported)" << std::endl;
	out << "-->" << std::endl;
	
	out << "	<printer1 source=\"LOCAL\" port=\"COM1\" command_set=\"STAR\" baud_rate=\"9600\" logo=\"0\"/>" << std::endl;
	out << "	<log dir=\"/var/local/ksi/log/\" file=\"\"/>" << std::endl;
	out << "	<base host=\"localhost\" port=\"1521\" sid=\"CAFE\"/>" << std::endl;

	out << "<!--" << std::endl;
	out << "items       - process kitchen for printer1" << std::endl;
	out << "-->" << std::endl;

	out << "	<kitchen1 items=\"1 2\"/>" << std::endl;
	
	out << "<!--" << std::endl;
	out << "interval      - synchronization interval per second" << std::endl;
	out << "auto_ready    - mark all printed entrees as ready (for kitchen feedback). Default 1, set to 0 to disable" << std::endl;
	out << "print_title   - print kitchen title in receipt header. Default 1, set to 0 to disable" << std::endl;
	out << "print_barcode - print barcode in receipt header. Default 1, set to 0 to disable (not supported)" << std::endl;
	out << "-->" << std::endl;
	
	out << "	<sync1 interval=\"5\" auto_ready=\"1\" print_title=\"1\" print_barcode=\"0\"/>" << std::endl;
	out << "	<locale name=\"eng\" date_format=\"dd.MM.yy\" time_format=\"HH:mm:ss\"/>" << std::endl;
}


int main(int argc, char* argv[])
{
	app_err << logName;
#ifndef __WIN32__
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = term_handler;
	sigaction(SIGINT, &sa, 0);
	sigaction(SIGTERM, &sa, 0);
#endif //__WIN32__

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
		config_file = "kitchen_informer.xml";
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
		std::string log_dir = config.get_attr("log", "dir") + (config.get_attr("log", "dir") == "" ? "" : "/");
		std::string log_file = config.get_attr("log", "file");
		if (log_file == "") log_file = logName;
		try {
			log_ptr.reset(new ksi_logger::logger_t(new ksi_logger::file_log_t(log_dir + log_file, 10)));
		}
		catch (std::exception& ex) {				
			app_err << ex.what();
			return 3;
		}
		
		*log_ptr << version::product << " v" << version::version;
		*log_ptr << version::copyright;
		
		if (boost::to_lower_copy(config.get_attr("locale", "name")) == "rus") lang = 2;
		else lang = 1;
		*log_ptr << "<locale name=\"" + config.get_attr("locale", "name") + "\"/>";
		
		*log_ptr << "<locale date_format=\"" + config.get_attr("locale", "date_format") + "\"/>";
		*log_ptr << "<locale time_format=\"" + config.get_attr("locale", "time_format") + "\"/>";
		ksi_cafe::format_date_time_t format(config.get_attr("locale", "date_format"), config.get_attr("locale", "time_format"));

		std::string tns = "(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST="
		+ config.get_attr("base", "host")
        + ")(PORT="
		+ config.get_attr("base", "port")
		+ ")))(CONNECT_DATA=(SERVICE_NAME="
		+ config.get_attr("base", "sid")
		+ ")))";
		*log_ptr << "tns: " + tns;
		
		factory.reset(new connection_factory_t("kitchen_ag", "ksi", config.get_attr("base", "host"), config.get_attr("base", "sid")));
		
		*log_ptr << "Connect to base";
		ksi_cafe::session = factory->create();
		*log_ptr << "successful";

		*log_ptr << "create users_t...";
		ksi_cafe::users_t users;
		*log_ptr << "successful";

		std::vector<boost::shared_ptr<com_t> > com_ptr_vec;
		std::vector<boost::shared_ptr<base_printer_t> > printer_ptr_vec;
		//std::vector<boost::shared_ptr<ksi_kitchen::container_kitchen_t> > container_ptr_vec;
		std::vector<bool> auto_ready_vec;
		std::vector<bool> title_vec;
		std::vector<bool> barcode_vec;
		std::vector<int> interval_vec;
		//std::vector<boost::shared_ptr<ksi_client::oracle_session_i> > session_ptr_vec;
		std::vector<boost::shared_ptr<ksi_logger::logger_t> > log_ptr_vec;
		
		std::vector<boost::shared_ptr<kitchen_process_t> > process_vec;
		
		for (size_t i=1; i<11; ++i) {
			std::string i_str = boost::lexical_cast<std::string>(i);
			std::string kitchens = config.get_attr("kitchen"+i_str, "items");
			if (kitchens == "") kitchens = config.get_attr("kitchen"+i_str, "types");
			std::string printer_command_set = boost::to_lower_copy(config.get_attr("printer"+i_str, "command_set"));
			std::string printer_port = config.get_attr("printer"+i_str, "port"); //boost::to_lower_copy();
			std::string printer_baud_rate = config.get_attr("printer"+i_str, "baud_rate");
			std::string printer_type = boost::to_lower_copy(config.get_attr("printer"+i_str, "source"));
			std::string printer_host = config.get_attr("host"+i_str, "host");
			
			if (kitchens == "") continue;
			
			int sync_interval = 5;
			bool auto_ready = true;
			bool print_title = true;
			bool print_barcode = true;
			if (config.get_attr("sync"+i_str, "interval") != "") sync_interval = boost::lexical_cast<int>(config.get_attr("sync"+i_str, "interval"));
			if (config.get_attr("sync"+i_str, "auto_ready") != "1") auto_ready = false;
			if (config.get_attr("sync"+i_str, "print_title") != "1") print_title = false;
			if (config.get_attr("sync"+i_str, "print_barcode") != "1") print_barcode = false;
			
			try {
				std::auto_ptr<ksi_client::oracle_session_i> session_ptr = factory->create();
				boost::shared_ptr<com_t> os_ptr;
				boost::shared_ptr<base_printer_t> printer_ptr;
				if (printer_type == "local") {
					os_ptr.reset(new com_t(printer_port, boost::lexical_cast<int>(printer_baud_rate)));
					if (printer_command_set == "star") printer_ptr.reset(new star_printer_t(*os_ptr));
					else if (printer_command_set == "epson") printer_ptr.reset(new epson_printer_t(*os_ptr));
					else throw std::runtime_error("printer_command_set is " + printer_command_set);
				}
				else if (printer_type == "net" || printer_type == "remote") {
					int hostport = 703;
					if (config.get_attr("printer"+i_str, "hostport") != "")
					hostport = boost::lexical_cast<int>(config.get_attr("printer"+i_str, "hostport"));
					printer_ptr.reset(new net_printer_t(printer_host, hostport));
				}
				else throw std::runtime_error("printer_type is " + printer_type);
				std::auto_ptr<ksi_kitchen::container_kitchen_t> container_ptr (new ksi_kitchen::container_kitchen_t(session_ptr.get(), kitchens));
				com_ptr_vec.push_back(os_ptr);
				printer_ptr_vec.push_back(printer_ptr);
				//container_ptr_vec.push_back(container_ptr);
				auto_ready_vec.push_back(auto_ready);
				title_vec.push_back(print_title);
				barcode_vec.push_back(print_barcode);
				interval_vec.push_back(sync_interval);
				boost::shared_ptr<ksi_logger::logger_t> log (new ksi_logger::logger_t(new ksi_logger::file_log_t(log_dir + "printer_#" + i_str, 10)));
				log_ptr_vec.push_back(boost::shared_ptr<ksi_logger::logger_t> (boost::shared_ptr<ksi_logger::logger_t>(log)));
				process_vec.push_back(boost::shared_ptr<kitchen_process_t> (new kitchen_process_t(session_ptr, *log, *printer_ptr, format, users, container_ptr, print_title, print_barcode, auto_ready)));
			}
			catch (std::exception& ex) {
				*log_ptr << ex.what();
			}
		}
		
		if (process_vec.size() == 0) {
			*log_ptr << "process_vec.size() == 0";
			return 1;
		}
		
		*log_ptr << "application started";
		
		std::vector<boost::shared_ptr<boost::thread> > thread_vec;

		for (size_t i=0; i<process_vec.size(); ++i) {
			boost::function<size_t ()> func = boost::bind(&kitchen_process_t::run, process_vec[i]);
			boost::function<void ()> func_thread = boost::bind(process, interval_vec[i], func);			
			boost::shared_ptr<boost::thread> thread_ptr (new boost::thread(func_thread));
			thread_vec.push_back(thread_ptr);
		}
		
		for (size_t i=0; i<thread_vec.size(); ++i) {
			thread_vec[i]->join();
		}
	}
	catch (exception_t& ex) {
		*log_ptr << ex.what();
	}
	catch (ksi_client::ksi_exception& ex) {
		*log_ptr << ex.what();
	}
	catch (std::exception& ex) {
		*log_ptr << ex.what();
	}
	
	*log_ptr << "application stoped";
	return 0;
}
