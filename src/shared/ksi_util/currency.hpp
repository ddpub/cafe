#ifndef DMITRY_DERGACHEV_27_03_2008_CURRENCY_H
#define DMITRY_DERGACHEV_27_03_2008_CURRENCY_H

#include <string>
#include <boost/lexical_cast.hpp>
#include <ksi_include/ksi_exceptions.hpp>

namespace ksi_cafe {

class base_currency_t {
public:
	base_currency_t() {}
	~base_currency_t() {}
	virtual std::string format(const double&) const =0;
	virtual double reverse(const std::string&) const =0;
	virtual base_currency_t* clone() const=0;
protected:
	int frac(const double& val) const {
		double tmp = val > 0 ? 100*val : -100*val;
		tmp += 0.5;
		return int(tmp)/100;
	}
	int trunc(const double& val) const {
		double tmp = val > 0 ? 100*val : -100*val;
		tmp += 0.5;
		return int(tmp)-(int(tmp)/100)*100;
	}
	std::string frac_str(const double& val) const {
		return boost::lexical_cast<std::string>(frac(val));
	}
	std::string trunc_str(const double& val) const {
		return boost::lexical_cast<std::string>(trunc(val)/10)+boost::lexical_cast<std::string>(trunc(val)-10*(trunc(val)/10));
	}
};

class percent_t: public base_currency_t {
public:
	percent_t() {}
	virtual ~percent_t() {}
	virtual std::string format(const double& val) const {
		return frac_str(val) + "." + trunc_str(val) + "%";
	}
	virtual double reverse(const std::string& str) const {
		if (str.length() < 3) THROW_MESSAGE(0, "missing format value: "+str, "double percent_t::reverse(const std::string&)");
		if (str.substr(str.length()-1, 1) != "%") THROW_MESSAGE(1, "missing format value: "+str, "double percent_t::reverse(const std::string&)");
		if (str.find('.') == std::string::npos) THROW_MESSAGE(2, "missing format value: "+str, "double percent_t::reverse(const std::string&)");
		std::string str_trunc = str.substr(str.length()-3, 2);
		std::string str_frac = str.substr(0, str.length()-4);
		int trunc_ = boost::lexical_cast<int>(str_trunc);
		int frac_ = boost::lexical_cast<int>(str_frac);
		return frac_ + double(trunc_)/100;
	}
	virtual base_currency_t* clone() const { return new percent_t(); }
};

/*
class en_currency_t : public base_currency_t {
public:
	en_currency_t() {}
	virtual ~en_currency_t() {}
	virtual std::string format(const double& val) const {
		if (val >= 0) return "$" + frac_str(val) + "." + trunc_str(val);
		return "-$" + frac_str(val) + "." + trunc_str(val);
	}
	virtual double reverse(const std::string& str) const {
		if (str.length() < 4) THROW_MESSAGE(0, "missing format value: "+str, "double en_currency_t::val(const std::string&)");
		bool positive_ = true;
		std::string tmp = str;
		if (str[0] == '-') {
			positive_ = false;
			tmp = str.substr(1, str.length()-1);
		}
		if (tmp[0] != '$') THROW_MESSAGE(1, "missing format value: "+str, "double en_currency_t::val(const std::string&)");
		tmp = tmp.substr(1, tmp.length()-1);
		std::string str_trunc = tmp.substr(tmp.length()-2, 2);
		std::string str_frac = tmp.substr(0, tmp.length()-3);
		int trunc_ = boost::lexical_cast<int>(str_trunc);
		int frac_ = boost::lexical_cast<int>(str_frac);
		if (positive_) return frac_ + double(trunc_)/100;
		return -1*(frac_ + double(trunc_)/100);
	}
	virtual base_currency_t* clone() const { return new en_currency_t(); }
};

class ru_currency_t : public base_currency_t {
public:
	ru_currency_t() {}
	virtual ~ru_currency_t() {}
	virtual std::string format(const double& val) const {
		if (val >= 0) return frac_str(val) + "," + trunc_str(val) + "ð.";
		return "-" + frac_str(val) + "," + trunc_str(val) + "ð.";
	}
	virtual double reverse(const std::string& str) const {
		if (str.length() < 5) THROW_MESSAGE(0, "missing format value: "+str, "double ru_currency_t::val(const std::string&)");
		bool positive_ = true;
		std::string tmp = str;
		if (str[0] == '-') {
			positive_ = false;
			tmp = str.substr(1, str.length()-1);
		}
		if (tmp.substr(tmp.length()-2, 2) != "ð.") THROW_MESSAGE(1, "missing format value: " + str, "double ru_currency_t::val(const std::string&)");
		tmp = tmp.substr(0, tmp.length()-2);
		std::string str_trunc = tmp.substr(tmp.length()-2, 2);
		std::string str_frac = tmp.substr(0, tmp.length()-3);
		int trunc_ = boost::lexical_cast<int>(str_trunc);
		int frac_ = boost::lexical_cast<int>(str_frac);
		if (positive_) return frac_ + double(trunc_)/100;
		return -1*(frac_ + double(trunc_)/100);		
	}
	virtual base_currency_t* clone() const { return new ru_currency_t(); }
};
*/
};

#endif
