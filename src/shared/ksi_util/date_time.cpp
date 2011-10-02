
#include "date_time.hpp"
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/lexical_cast.hpp>


namespace ksi_cafe {


std::string format_time_t::format(const boost::posix_time::ptime& time) const {
	std::tm tm = boost::posix_time::to_tm(time);
	// HH:mm:ss
	// hh:mm:ss
	// h:mm:ss
	// H:mm:ss
	size_t hour = tm.tm_hour;
	size_t min = tm.tm_min;
	size_t sec = tm.tm_sec;
	std::string result = "";
	bool hour_catched_ = false;
	bool min_catched_ = false;
	bool sec_catched_ = false;
	std::string x_hour24_str = boost::lexical_cast<std::string>(hour);
	std::string x_hour12_str = hour > 12 ? boost::lexical_cast<std::string>(hour-12) : boost::lexical_cast<std::string>(hour);
	if (hour == 0) x_hour12_str = "12";
	std::string x_min_str = boost::lexical_cast<std::string>(min);
	std::string x_sec_str = boost::lexical_cast<std::string>(sec);
	
	std::string xx_hour24_str = x_hour24_str.length() == 1 ? "0"+x_hour24_str : x_hour24_str;
	std::string xx_hour12_str = x_hour12_str.length() == 1 ? "0"+x_hour12_str : x_hour12_str;
	std::string xx_min_str = x_min_str.length() == 1 ? +"0"+x_min_str : x_min_str;
	std::string xx_sec_str = x_sec_str.length() == 1 ? "0"+x_sec_str : x_sec_str;	
	
	std::string simbol_ = "";
	
	for (size_t i=0; i<format_.size(); ++i) {
		const char* curr = &format_[i];
		const char* next = 0; if (i<format_.size()) next = &format_[i+1];
		if (!hour_catched_) {
			if (*curr == 'h') {
				if (!next || *next != 'h') {
					result += x_hour12_str;
					hour_catched_ = true;
					if (hour >= 12) simbol_ = " " + pm_simbol_;
					else simbol_ = " " + am_simbol_;
					continue;
				}
				else if (next && *next == 'h') {
					result += xx_hour12_str;
					hour_catched_ = true;
					if (hour >= 12) simbol_ = " " + pm_simbol_;
					else simbol_ = "  " + am_simbol_;
					++i;
					continue;
				}
			}
			else if (*curr == 'H') {
				if (!next || *next != 'H') {
					result += x_hour24_str;
					hour_catched_ = true;
					continue;
				}
				else if (next && *next == 'H') {
					result += xx_hour24_str;
					hour_catched_ = true;
					++i;
					continue;
				}
			}
		}
		if (!min_catched_) {
			if (*curr == 'm') {
				if (!next || *next != 'm') {
					result += x_min_str;
					min_catched_ = true;
					continue;
				}
				else if (next && *next == 'm') {
					result += xx_min_str;
					min_catched_ = true;
					++i;
					continue;
				}
			}
		}
		if (!sec_catched_) {
			if (*curr == 's') {
				if (!next || *next != 's') {
					//result += x_sec_str + simbol_;
					result += x_sec_str;
					sec_catched_ = true;
					continue;
				}
				else if (next && *next == 's') {
					//result += xx_sec_str + simbol_;
					result += xx_sec_str;
					sec_catched_ = true;
					++i;
					continue;
				}
			}
		}	
		result += format_[i];
	}
	return result+simbol_;
}

std::string format_date_t::format(const boost::posix_time::ptime& date) const {
	// dd.MM.yy
	// MM/dd/yy
	// dd-MM-yy
	// d.m.yy
	std::tm tm = boost::posix_time::to_tm(date);
	size_t year = tm.tm_year + 1900;
	size_t month = tm.tm_mon + 1;
	size_t day = tm.tm_mday;
	
	std::string result = "";
	
	bool day_catched_ = false;
	bool month_catched_ = false;
	bool year_catched_ = false;
	
	std::string x_day_str = boost::lexical_cast<std::string>(day);
	std::string x_month_str = boost::lexical_cast<std::string>(month);
	std::string xxxx_year_str = boost::lexical_cast<std::string>(year);
	std::string xx_year_str = xxxx_year_str;
	if (xxxx_year_str.length() > 2) xx_year_str = xxxx_year_str.substr(xxxx_year_str.length()-2, 2);
	std::string xx_day_str = x_day_str.length() == 1 ? "0"+x_day_str : x_day_str;
	std::string xx_month_str = x_month_str.length() == 1 ? "0"+x_month_str : x_month_str;
	
	for (size_t i=0; i<format_.size(); ++i) {
		const char* curr = &format_[i];
		const char* next = 0; if (i<format_.size()) next = &format_[i];
		const char* next_next = 0; if (i+1<format_.size()) next_next = &format_[i+1];
		const char* next_next_next = 0; if (i+2<format_.size()) next_next_next = &format_[i+2];
		if (!day_catched_) {
			if (*curr == 'd') {
				if (!next || *next != 'd') {
					day_catched_ = true;
					result += x_day_str;
					continue;
				}
				else if (next && *next == 'd') {
					day_catched_ = true;
					result += xx_day_str;
					++i;
					continue;
				}
			}
		}
		if (!month_catched_) {
			if (*curr == 'M') {
				if (!next || *next != 'M') {
					month_catched_ = true;
					result += x_month_str;
					continue;
				}
				else if (next && *next == 'M') {
					month_catched_ = true;
					result += xx_month_str;
					++i;
					continue;
				}
			}
		}
		if (!year_catched_) {
			if (*curr == 'y') {
				if (next && *next == 'y') {
					if (next_next && *next_next == 'y' && next_next_next && *next_next_next == 'y') {
						year_catched_ = true;
						result += xxxx_year_str;
						i += 3;
						continue;
					}
					else {
						year_catched_ = true;
						result += xx_year_str;
						++i;
						continue;
					}
				}
			}
		}
		result += format_[i];
	}
	
	return result;
}


};

