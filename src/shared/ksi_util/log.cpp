
#include "log.hpp"
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>


base_log_t& ostream_log_t::operator<< (const std::string& message)
{
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	std::string pref = to_simple_string(now) + " ";
	os_ << pref << message << std::endl;
	os_.flush();
	return *this;
}

void rotate(const std::string& filename, int rsize)
{
	try {
	struct stat statbuf;
	FILE *stream;
	if ((stream = fopen(filename.c_str(), "r+")) == NULL) {
		return;
	}
	stat(filename.c_str(), &statbuf);
	fclose(stream);
	if (statbuf.st_size > rsize*1024*1024) {
		remove(std::string(filename + ".bak").c_str());
		rename(filename.c_str(), std::string(filename + ".bak").c_str());
		stream = fopen(filename.c_str(), "w+");
		fclose(stream);
	}
	}
	catch (...) {
	}
}


file_log_t::file_log_t(const std::string& filename_, int rsize_): filename_(filename_), rsize_(rsize_) {
	rotate(filename_, rsize_);
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	std::string pref = to_simple_string(now) + " ";
	std::ofstream os_(filename_.c_str(), std::ios_base::out | std::ios_base::app);
	if (os_) {
		os_ << pref << "touch file" << std::endl;
		os_.flush();
	}
	else {
		throw std::runtime_error("Failed open file: " + filename_);
	}
}

base_log_t& file_log_t::operator<< (const std::string& message)
{
	rotate(filename_, rsize_);
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	std::string pref = to_simple_string(now) + " ";
	std::ofstream os_(filename_.c_str(), std::ios_base::out | std::ios_base::app);
	if (os_) {
		os_ << pref << message << std::endl;
		os_.flush();
	}
	else {
		std::cerr << "Failed open file: " << filename_ << std::endl;
	}
	return *this;
}
