#ifndef BAG_SPOOLER_H_DERGACHEV_06_11_2008
#define BAG_SPOOLER_H_DERGACHEV_06_11_2008

#include <string>
#include "log.hpp"
#include <ksi_include/monitor.hpp>
#include <boost/thread.hpp>

class bag_record_t {
public:
	bag_record_t(int cafe_id_, int app_id_, const std::string& version_, const std::string& message_)
	: cafe_id_(cafe_id_), app_id_(app_id_), version_(version_), message_(message_) {}
	int cafe_id() const { return cafe_id_; }
	int app_id() const { return app_id_; }
	const std::string& version() const { return version_; }
	const std::string& message() const { return message_; }
	std::string http_path() const;
private:
	int cafe_id_;
	int app_id_;
	std::string version_;
	std::string message_;
};

class bag_spooler_t {
public:
	explicit bag_spooler_t(const std::string& server_);
	~bag_spooler_t();
	void send_bag(const bag_record_t& record);
private:
	void process_queue();
	std::string send_request(const std::string& path_str);
	size_t compress();
private:
	std::string server_;
	boost::mutex raw_mutex_;
	MonitorQueue<bag_record_t>  queue_;
	boost::thread process_trh_;
private:
	file_log_t spool_log;
};


#endif
