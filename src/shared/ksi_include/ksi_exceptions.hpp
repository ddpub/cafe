#ifndef KSI_EXCEPTIONS_H_DERGACHEV_06_03_2007
#define KSI_EXCEPTIONS_H_DERGACHEV_06_03_2007

#include <exception>
#include <string>
#include <boost/lexical_cast.hpp>


class exception_t: public std::exception
{
public:
// Constructor
	exception_t(int code_, const std::string& message_, const std::string location_)
	: code_(code_), message_(message_), location_(location_) {}
	exception_t(int code_, const std::string location_)
	: code_(code_), message_(""), location_(location_) {}
public:
// Accessors
	long code() const { return code_; }
	const std::string& message() const { return message_; }
	const std::string& location() const { return location_; }
	const std::string& comp_message() const
	{
		text_ = "ERROR #: "
				+ boost::lexical_cast<std::string>(code())
				+ "\n" + location() + "\n" + message();
		return text_;
	}
	virtual const char* what() const throw() {
		comp_message();
		return text_.c_str();
	}
	const std::string user_message() const {
		if (message_ == "") comp_message();
		else text_ = message();
		return text_;
	}
	virtual ~exception_t() throw() {}
private:
	long code_;
	std::string message_;
	std::string location_;
	mutable std::string text_;
};

#define WARNING_IN_LOCATION \
boost::lexical_cast<std::string>("WARNING ") + boost::lexical_cast<std::string>("At ") + boost::lexical_cast<std::string>(__FILE__) + boost::lexical_cast<std::string>(" line ") + boost::lexical_cast<std::string>(__LINE__)

#define LLOCATION \
boost::lexical_cast<std::string>("At ") + boost::lexical_cast<std::string>(__FILE__) + boost::lexical_cast<std::string>(" line ") + boost::lexical_cast<std::string>(__LINE__)

#define ERROR_IN_LOCATION \
boost::lexical_cast<std::string>("ERROR ") + boost::lexical_cast<std::string>("At ") + boost::lexical_cast<std::string>(__FILE__) + boost::lexical_cast<std::string>(" line ") + boost::lexical_cast<std::string>(__LINE__)

#define THROW_MESSAGE(__CODE__, __MESSAGE__STR__, __OPER__STR__) \
throw exception_t(__CODE__, __MESSAGE__STR__, boost::lexical_cast<std::string>("ERROR in ") + boost::lexical_cast<std::string>(__OPER__STR__) + boost::lexical_cast<std::string>(". At ") + boost::lexical_cast<std::string>(__FILE__) + boost::lexical_cast<std::string>(" line ") + boost::lexical_cast<std::string>(__LINE__));

#define THROW_LOCATION(__CODE__, __OPER__STR__) \
throw exception_t(__CODE__, boost::lexical_cast<std::string>("ERROR in ") + boost::lexical_cast<std::string>(__OPER__STR__) + boost::lexical_cast<std::string>(". At ") + boost::lexical_cast<std::string>(__FILE__) + boost::lexical_cast<std::string>(" line ") + boost::lexical_cast<std::string>(__LINE__));

#endif
