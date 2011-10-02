#ifndef DMITRY_DERGACHEV_27_03_2008_LOG_H
#define DMITRY_DERGACHEV_27_03_2008_LOG_H

#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>



class base_log_t
{
public:
	virtual base_log_t& operator<<(const std::string&) = 0;
	virtual base_log_t& operator<<(int) = 0;
	virtual ~base_log_t() {}
private:
};

class ostream_log_t: public base_log_t
{
public:
	explicit ostream_log_t(std::ostream& os_): os_(os_) {}
	base_log_t& operator<< (const std::string& message);
	base_log_t& operator<< (int message) {
		return operator<< (boost::lexical_cast<std::string>(message));
	}
private:
	std::ostream& os_;
private:
	ostream_log_t();
	ostream_log_t(const ostream_log_t&);
	ostream_log_t& operator=(const ostream_log_t&);
};

void rotate(const std::string& filename, int rsize);

class file_log_t: public base_log_t
{
public:
	file_log_t(const std::string& filename_, int rsize_);
	base_log_t& operator<< (const std::string& message);
	base_log_t& operator<< (int message) {
		return operator<< (boost::lexical_cast<std::string>(message));
	}
private:
	std::string filename_;
	int rsize_;
private:
	file_log_t();
	file_log_t(const file_log_t&);
	file_log_t& operator=(const file_log_t&);
};

#endif
