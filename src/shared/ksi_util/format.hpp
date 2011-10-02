#ifndef DMITRY_DERGACHEV_19_09_2008_FORMAT_H
#define DMITRY_DERGACHEV_19_09_2008_FORMAT_H

#include <string>

namespace ksi_cafe {


class base_format_t {
public:
	base_format_t() {}
	virtual ~base_format_t() {}
	virtual std::string format(const std::string&) const =0;
	virtual std::string reverse(const std::string&) const =0;
	virtual base_format_t* clone() const =0;
};

class identical_format_t: public base_format_t {
public:
	identical_format_t() {}
	virtual ~identical_format_t() {}
	virtual std::string format(const std::string& source) const { return source; }
	virtual std::string reverse(const std::string& source) const { return source; }
	virtual base_format_t* clone() const { return new identical_format_t(); }
};

class en_phone_format_t : public base_format_t {
public:
	explicit en_phone_format_t() {}
	virtual ~en_phone_format_t() {}
	virtual std::string format(const std::string&) const;
	virtual std::string reverse(const std::string&) const;
	virtual base_format_t* clone() const { return new en_phone_format_t(); }
};


};

#endif
