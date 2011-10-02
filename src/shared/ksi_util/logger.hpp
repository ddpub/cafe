#ifndef DMITRY_DERGACHEV_27_03_2008_LOGGER_H
#define DMITRY_DERGACHEV_27_03_2008_LOGGER_H

#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <memory>

namespace ksi_logger {
	
	class base_log_t {
	public:
		virtual base_log_t& operator<<(const std::string&) = 0;
		virtual base_log_t& continue_put(const std::string&) = 0;
		virtual ~base_log_t() {}
	protected:
		std::string prefix() const;
	};

	class ostream_log_t: public base_log_t {
	public:
		explicit ostream_log_t(std::ostream& os_): os_(os_) {}
		base_log_t& operator<< (const std::string& message);
		base_log_t& continue_put(const std::string& message);
	private:
		std::ostream& os_;
	private:
		ostream_log_t();
		ostream_log_t(const ostream_log_t&);
		ostream_log_t& operator=(const ostream_log_t&);
	};

	//void rotate(const std::string& filename, int rsize);

	class file_log_t: public base_log_t {
	public:
		file_log_t(const std::string& filename_, int rsize_);
		base_log_t& operator<< (const std::string& message);
		base_log_t& continue_put(const std::string& message);
	private:
		std::string filename_;
		int rsize_;
	private:
		file_log_t();
		file_log_t(const file_log_t&);
		file_log_t& operator=(const file_log_t&);
	};

	inline std::string __convertToString(std::ostream& (*fn)(std::ostream&)) {
		std::ostringstream oss;
		fn(oss);
		return std::string(oss.str());
	}
	
	class continue_t {
	public:
		explicit continue_t(base_log_t& log_): log_(log_) {}
		template<typename T>
		continue_t operator<<(const T& message) {
			return continue_t(log_.continue_put(boost::lexical_cast<std::string>(message)));
		}
		/*
		continue_t operator<<(const std::string& message) {
			return continue_t(log_.continue_put(message));
		}
		*/
		continue_t operator<<(std::ostream& (*fn)(std::ostream&)) {
			return continue_t(log_<<__convertToString(fn));
		}
	private:
		base_log_t& log_;
	};
	
	class logger_t {
	public:
		explicit logger_t(base_log_t* ext_log=0);
		~logger_t() { if (!owner_) log_.release(); }
		void set_log(base_log_t* ext_log) {
			if (!owner_) log_.release();
			log_.reset(ext_log);
		}
		template<typename T>
		continue_t operator<< (const T& message) {
			return continue_t(*log_<<boost::lexical_cast<std::string>(message));
		}
		typedef std::ostream& (*FN)(std::ostream&);
		//template <>
		continue_t operator<< (FN fn) {
			return continue_t(*log_<<__convertToString(fn));
		}
	private:
		std::auto_ptr<base_log_t> log_;
		bool owner_;
	};

}

#endif
