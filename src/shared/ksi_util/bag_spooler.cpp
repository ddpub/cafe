
#include "bag_spooler.hpp"
#ifdef _WIN32_BUILD_
#include <winsock2.h>
#include <ws2tcpip.h> 
#endif
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ksi_include/ksi_exceptions.hpp>
#include "url.hpp"
using boost::asio::ip::tcp;
std::string bag_record_t::http_path() const {
	std::string encode_cafe_id = url_encode_t().URLEncode(boost::lexical_cast<std::string>(cafe_id_));
	std::string encode_app_id = url_encode_t().URLEncode(boost::lexical_cast<std::string>(app_id_));
	std::string encode_message = url_encode_t().URLEncode(message_);
	std::string encode_version = url_encode_t().URLEncode(version_);
	std::string tmp = "/service/err.php?cafe_id="+encode_cafe_id+"&app_id="+encode_app_id+"&err_text="+encode_message+"&version="+encode_version;
	return tmp;
}
bag_spooler_t::bag_spooler_t(const std::string& server_): server_(server_), spool_log("bug_spooler.log", 10) {
	spool_log << "bag_spooler started, server: " + server_;
	process_trh_ = boost::thread(boost::bind(&bag_spooler_t::process_queue, this));
}
bag_spooler_t::~bag_spooler_t() {
	try {
		spool_log << "bab_spooler stopping";
		spool_log << compress();
		spool_log << "bag_spooler stopped";
	}
	catch (...) {
	}
}
void bag_spooler_t::send_bag(const bag_record_t& record) {
	spool_log << "push new record";
	queue_.push(record);
}
size_t bag_spooler_t::compress() {
	boost::xtime xt; 
	boost::xtime_get(&xt, boost::TIME_UTC); 
	xt.sec += 5; 
	int	fsize = queue_.size();
	if (fsize == 0) return 0;
	for (;;) {
		boost::thread::sleep(xt);
		int curr_size = queue_.size();
		if (fsize == curr_size) break;
		fsize = curr_size;
	}
	return fsize;
}
void bag_spooler_t::process_queue() {
	for (;;) {
		spool_log << "wait record";
		bag_record_t record = queue_.popWait();
		spool_log << "record get successful";
		spool_log << record.http_path();
		/**
		**/
		try {
			std::string response = send_request(record.http_path());
			spool_log << "record send successful";			
			spool_log << "response: " + response;
		}
		catch (std::exception& ex) {
			spool_log << LLOCATION;
			spool_log << "push old record";
			queue_.push(record);			
		}
		catch (...) {
			spool_log << LLOCATION;
			spool_log << "push old record";
			queue_.push(record);
		}
		/**
		**/
		
	}
}
std::string bag_spooler_t::send_request(const std::string& path_str) {
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(server_, "http");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;
	tcp::socket socket(io_service);
	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error && endpoint_iterator != end) {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
      throw boost::system::system_error(error);
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream << "GET " << path_str << " HTTP/1.0\r\n";
	request_stream << "Host: " << server_ << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: close\r\n\r\n";
	
	boost::asio::write(socket, request);
	
	boost::asio::streambuf response;
	boost::asio::read_until(socket, response, "\r\n");
	
	std::istream response_stream(&response);
	std::string http_version;
	response_stream >> http_version;
	unsigned int status_code;
	response_stream >> status_code;
	std::string status_message;
	std::getline(response_stream, status_message);
	if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
		THROW_MESSAGE(0, "Invalid " + server_ + " response", "void bag_spooler_t::send_request(const std::string&)");
	}
    if (status_code != 200)
    {
		THROW_MESSAGE(1, "Response " +  server_ + " returned with status code " + boost::lexical_cast<std::string>(status_code), "void bag_spooler_t::send_request(const std::string&)");
    }
	boost::asio::read_until(socket, response, "\r\n\r\n");
	std::string header;
	std::string response_str;
	while (std::getline(response_stream, header) && header != "\r") ;;
		//header += header + "\n";
	//response_str += "\n";
    if (response.size() > 0)
		response_str += boost::lexical_cast<std::string>(&response);
    while (boost::asio::read(socket, response,
		boost::asio::transfer_at_least(1), error))
    response_str += boost::lexical_cast<std::string>(&response);
    if (error != boost::asio::error::eof)
		throw boost::system::system_error(error);
	
	return response_str;
}
