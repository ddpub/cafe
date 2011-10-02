
#include "logger.hpp"
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

namespace ksi_logger {

	std::string base_log_t::prefix() const {
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		std::string pref = to_simple_string(now) + " ";
		return pref;
	}

	base_log_t& ostream_log_t::operator<< (const std::string& message) {		
		os_ << std::endl << prefix() << message;
		os_.flush();
		return *this;
	}

	base_log_t& ostream_log_t::continue_put(const std::string& message) {		
		os_ << message;
		os_.flush();
		return *this;
	}
	
	/*
	void rotate(const std::string& filename, int rsize) {
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
	*/
	
	std::string rotate(const std::string& filename) {
		size_t monthNum = boost::posix_time::second_clock::local_time().date().month();
		std::string name = filename + "_" + boost::lexical_cast<std::string>(monthNum) + ".log";
		//std::string name = filename + "_" + boost::gregorian::to_iso_string(boost::posix_time::second_clock::local_time().date()) + ".log";
		return name;
	}

	file_log_t::file_log_t(const std::string& filename_, int rsize_): filename_(filename_), rsize_(rsize_) {
		std::string fname = rotate(filename_);
		std::ofstream os_(fname.c_str(), std::ios_base::out | std::ios_base::app);
		if (os_) {
			os_ << std::endl << prefix() << "touch file";
			os_.flush();
		}
		else {
			throw std::runtime_error("Failed open file: " + filename_);
		}
	}

	base_log_t& file_log_t::operator<< (const std::string& message) {
		std::string fname = rotate(filename_);
		std::ofstream os_(fname.c_str(), std::ios_base::out | std::ios_base::app);
		if (os_) {
			os_ << std::endl << prefix() << message;
			os_.flush();
		}
		else {
			std::cerr << "Failed open file: " << filename_;
		}
		return *this;
	}
	
	base_log_t& file_log_t::continue_put(const std::string& message) {
		std::string finame = rotate(filename_);
		std::ofstream os_(finame.c_str(), std::ios_base::out | std::ios_base::app);
		if (os_) {
			os_ << message;
			os_.flush();
		}
		else {
			std::cerr << "Failed open file: " << filename_ << std::endl;
		}
		return *this;
	}
	
	logger_t::logger_t(base_log_t* ext_log): log_(ext_log), owner_(ext_log) { 
		if (!owner_) log_.reset(new ostream_log_t(std::clog)); 
	}
}
