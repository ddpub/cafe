#ifndef DMITRY_DERGACHEV_19_09_2008_DATE_TIME_H
#define DMITRY_DERGACHEV_19_09_2008_DATE_TIME_H

#include <string>
#include <boost/date_time/posix_time/ptime.hpp>


namespace ksi_cafe {

class format_time_t {
public:
	format_time_t(const std::string& format_, const std::string& am_simbol_, const std::string& pm_simbol_)
	: format_(format_), am_simbol_(am_simbol_), pm_simbol_(pm_simbol_) {}
	std::string format(const boost::posix_time::ptime& time) const;
private:
	std::string format_;
	std::string am_simbol_;
	std::string pm_simbol_;
	format_time_t();
};

class format_date_t {
public:
	explicit format_date_t(const std::string& format_): format_(format_) {}
	std::string format(const boost::posix_time::ptime& date) const;
private:
	std::string format_;
	format_date_t();
};

class format_date_time_t {
public:
	format_date_time_t(const std::string& format_date_str, const std::string& format_time_str, const std::string& am_simbol_="AM", const std::string& pm_simbol_="PM")
	: format_date_(format_date_str), format_time_(format_time_str, am_simbol_, pm_simbol_) {}
	std::string date(const boost::posix_time::ptime& date_) const { return format_date_.format(date_); }
	std::string time(const boost::posix_time::ptime& time_) const { return format_time_.format(time_); }
	std::string date_time(const boost::posix_time::ptime& date_time_) const { return date(date_time_) + " " + time(date_time_); }
private:
	format_date_t format_date_;
	format_time_t format_time_;
};

};


#endif
