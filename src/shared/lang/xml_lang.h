
#ifndef _XML_LANG_H
#define _XML_LANG_H

#include <vector>
#include <ksi_include/ksi_exceptions.hpp>
#include <wx_util/wx_util.hpp>
#include <wx/string.h>
#include <math.h>
#include <memory>
#include <wx/xml/xml.h>
#include <wx/msgdlg.h>
#include <boost/shared_ptr.hpp>
#include <ksi_util/date_time.hpp>

#include <ksi_util/log.hpp>

#include <map>
#include <boost/lexical_cast.hpp>

namespace ksi_cafe
{
	
class float_format_t 
{
private:
	wxString decimalseparator;
	wxString thousandseparator;
	wxString negativesign;
public:
	float_format_t() : decimalseparator(wxT(".")), thousandseparator(wxT("'")), negativesign(wxT("-")) {  }
	float_format_t(const wxString& Decimalseparator, const wxString& Thousandseparator, const wxString& Negativesign) :
		decimalseparator(Decimalseparator), thousandseparator(Thousandseparator), negativesign(Negativesign) {  }
	
	int frac(const double& val) const ;
	int trunc(const double& val, int num = 2) const ;
	std::string frac_str(const double& val) const;
	wxString trunc_str(const double& val, int num = 2) const;
	
	wxString frac_format(const double& val) const;
	
	wxString format(const double& val) const;
	
	double reverse(const wxString& str) const;
	wxString get_decimalseparator() const { return decimalseparator; }
	wxString get_thousandseparator() const { return thousandseparator; }
	wxString get_negativesign() const { return negativesign; } 
	
};
//----------------------------------------------------------------------------------------------------------------*/		
class currency_format_t 
{
private:
	wxString symbol;
	int prefix;
	int fractionsize;
public:
	currency_format_t(const wxString& Symbol, const int Prefix, const int Fractionsize) 
	: symbol(Symbol), prefix(Prefix), fractionsize(Fractionsize) { } 
	currency_format_t()
	: symbol(wxT("$")), prefix(1), fractionsize(2) { } 
	
	wxString get_symbol() const { return symbol; }
	bool is_prefix() const { if (prefix == 1) return true; return false; }
	int get_num() const { return fractionsize; }
};
	
//----------------------------------------------------------------------------------------------------------------*/	
class currency_t 
{
private:
	float_format_t float_format;
	currency_format_t currency_format;
				
public:  
	currency_t() : currency_format(currency_format_t()), float_format(float_format_t()) {  }
	currency_t(const currency_format_t& cur_format,  const float_format_t& float_form) 
	: currency_format(cur_format), float_format(float_form) {  }
	
	void load_floatformat(const wxString& Decimalseparator, const wxString& Thousandseparator, const wxString& Negativesign);
	void load_currencyformat(const wxString& Symbol, const int Prefix, const int Fractionsize);		
	wxString format(const double& val) const ;
	wxString format_float(const double& val) const ;
	double reverse(const wxString& str) const ;
	double reverse_float(const wxString& str) const ;

};

class key_t
{
public:
	std::string key;
	wxString value;
	key_t(std::string Key, wxString Value) { key = Key; value = Value; } 
	//key_t(const key_t& Key_) { key = Key_.key; value = Key_.value; } 
};
	
class virtualkeyboard_t
{
private:
	wxString name;
	std::vector<key_t> storage;
public:
	virtualkeyboard_t(const wxString& Name) : name(Name) {  }
	virtualkeyboard_t() { storage.clear(); }
	wxString get(std::string Key); 
	void add(key_t Key) { storage.push_back(Key); }
	wxString get_name() { return name; }
};
//----------------------------------------------------------------------------------------------------------------*/	
//----------------------------------------------------------------------------------------------------------------*/	
class template_t {
public:
private:
};
//----------------------------------------------------------------------------------------------------------------*/	
class control_t
{
private:
	std::string name;
	wxString value;
	std::string ref;
	
	bool loaded_ref;
	bool failed;
public:
	control_t(std::string Name, wxString Value, std::string Ref="") 
	  :name(Name), value(Value), ref(Ref), loaded_ref(false), failed(false) 
	{  }
	//control_t& get() const { return this; }
	std::string get_name() const { return name; }
	wxString get_value();
	std::string get_ref() const { return ref; }
	bool is_loaded_ref() const { return loaded_ref; }
	void set_loaded_ref() { loaded_ref = true; }
	void set_failed() { failed = true; }
	bool is_failed() const { return failed; }
	
	void copy(boost::shared_ptr<control_t> val);
};
//----------------------------------------------------------------------------------------------------------------*/	
class form_t;
	
class form_t
{
private:
	std::string name;
	std::vector< boost::shared_ptr<form_t> > _form_;
	std::vector< boost::shared_ptr<control_t> > _control_;
public:
	form_t(const std::string Name, wxXmlNode *child, bool is_load_ref);
	form_t() : name("Failed") {  }
		
	void add_form(boost::shared_ptr<form_t> Form) { _form_.push_back(Form);  }
	void add_control(boost::shared_ptr<control_t> Control) { _control_.push_back(Control); }
	
	std::string get_name() const { return name; }
	boost::shared_ptr<control_t> get_control(int i);
	boost::shared_ptr<control_t> find_control(std::string Name);
	
	boost::shared_ptr<form_t> get_form(int i);
	boost::shared_ptr<form_t> find_form(std::string Name);
	
	wxString control(std::string Name, std::string Failed);
	wxString get(std::string FName, std::string CName, std::string Failed);
	boost::shared_ptr<form_t> form(std::string Name);
	
	wxString get_parse_ref(std::string sss, std::string Failed);
};

class phonenum_t
{
private:
	std::string mask;
public:
	phonenum_t(std::string Mask) :mask(Mask)
	{  }
	//control_t& get() const { return this; }
	wxString get(std::string num); 
	std::string get_mask() const { return mask; }
};
//<phonenumber template="xxx-xxx-xxxx" />

//----------------------------------------------------------------------------------------------------------------*/	
//----------------------------------------------------------------------------------------------------------------*/	

class xml_lang_t 
{
public:
	explicit xml_lang_t(const std::string& File_, const std::string& Local_name_, const std::string& App_name_);
	xml_lang_t();
	//explicit xml_lang_t(const std::string& File_);
	~xml_lang_t() {  }
	void load(const std::string& File_, const std::string& Local_name_, const std::string& App_name_);
	void load_lang(wxXmlNode *child, const std::string& app_name_);
private:
	std::string file_;
	std::string local_name_;
	//std::vector<std::string> locales;
	std::string app_name_;
	std::auto_ptr<wxXmlDocument> xml_file_;
	std::auto_ptr<currency_t> _currency_;
	std::auto_ptr<virtualkeyboard_t> _virtualkeyboard_;
	std::auto_ptr<form_t> _application_;
	std::auto_ptr<format_date_time_t> _datetime_;
	std::auto_ptr<format_date_time_t> _datetime_short_;
	std::auto_ptr<phonenum_t> _phonenum_;
	std::map<std::string, wxXmlNode*> templates_;
	bool loaded;
private:
	xml_lang_t(const xml_lang_t&);
	xml_lang_t& operator=(const xml_lang_t&);
public:
	wxString get_string(const std::string& templateName, const std::map<std::string, std::string>& mapper) const;
	wxString get_date(const boost::posix_time::ptime& date_); 
	wxString get_time(const boost::posix_time::ptime& date_); 
	wxString get_date_time(const boost::posix_time::ptime& date_); 
	
	wxString get_date_short(const boost::posix_time::ptime& date_); 
	wxString get_time_short(const boost::posix_time::ptime& date_); 
	wxString get_date_time_short(const boost::posix_time::ptime& date_); 
	
	wxString get_currency(double Num);
	wxString get_phone(std::string num);
	double reverse_currency(wxString Num); 
	double reverse_float(wxString Num); 
	wxString get_float(double Num); 
	boost::shared_ptr<form_t> find_form(std::string Name); 
	boost::shared_ptr<form_t> form(std::string Name); 
	wxString get(const std::string& FName, const std::string& CName, const std::string& Failed); 
	std::string get_std(const std::string& FName, const std::string& CName, const std::string& Failed);
	wxString get_key(std::string Name); 
	std::vector<std::string> get_locales(const std::string& File_);
	wxString get_keyboard_name() { return _virtualkeyboard_->get_name(); }
	
	std::string get_file() const { return file_; }
	std::string get_locale_name() const { return local_name_; }
	std::string get_app_name() const { return app_name_; }
	
	void ref_load(boost::shared_ptr<control_t> ctrl);
	boost::shared_ptr<form_t> load_app(const std::string& App_name_);
	
	bool is_loaded() { return loaded; }
};



};

#endif
