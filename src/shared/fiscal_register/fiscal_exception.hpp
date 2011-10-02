#ifndef DMITRY_DERGACHEV_28_04_2009_FISCAL_EXCEPTION_H
#define DMITRY_DERGACHEV_28_04_2009_FISCAL_EXCEPTION_H


#include <stdexcept>
#include <string>
#include <boost/lexical_cast.hpp>

namespace fiscal {

class exception : public std::exception {
public:
	exception(int msg_id, const std::string& msg, long code = 0): __msg_id(msg_id), __text(msg), __code(code) {};
	virtual const char* what() const throw() {
		return __text.c_str();
	}
	virtual long msg_id() const throw() { return __msg_id; }
	virtual long code() const throw() { return __code; }
	virtual ~exception() throw() {};
	virtual std::string fmsg() const throw() { 
		boost::lexical_cast<std::string>(__msg_id) + ", " + __text;
	}
private:
	long __msg_id;
	std::string __text;
	long __code;
};
/*
class device_exception : public exception {
public:
	device_exception(long msg_id, const std::string& msg, long code=0): exception(msg_id, msg, code) {};
};

class proto_exception : public exception {
public:
	proto_exception(long msg_id, const std::string& msg, long code=0): exception(msg_id, msg, code) {};
};

class logical_exception : public exception {
public:
	logical_exception(long msg_id, const std::string& msg, long code=0): exception(msg_id, msg, code) {};
};

class registrator_exception : public exception {
public:
	registrator_exception(long msg_id, const std::string& msg, long code=0): exception(msg_id, msg, code) {};
};
*/

/*
class eexception_t: public std::exception {
public:
	eexception_t(size_t id_, const std::string& message_): id_(id_), message_(message_) {
		what_ = boost::lexical_cast<std::string>(id_) + ", " + message_;
	}
	~exception_t() throw() {}
	size_t id() const { return id_; }
	const std::string& message() const { return message_; }
	const char* what() throw() { return what_.c_str(); }
	const std::string& fmsg() const { return what_; }
private:
	size_t id_;
	std::string message_;
	std::string what_;
};
*/

	
};

#endif
