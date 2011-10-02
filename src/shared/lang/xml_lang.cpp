#include "xml_lang.h"
#include <wx/file.h>

#include <vector>
#include <vector>
//#include <ksi_include/ksi_exceptions.h>

namespace ksi_cafe{

std::vector< boost::shared_ptr<control_t> > ref_load_v;
std::vector< boost::shared_ptr<form_t> > loaded_app;
//----------------------------------------------------------------------------------------------------------------*/	
std::string& get_local(std::string app_name, std::string object_name, const std::string& original_name)
{
	try
	{
		//ksi_lang_local load_local(_LOCALFILE);
		//std::string res = load_local.find(std::string app_name, std::string object_name);
	}
	catch(...)
	{
		//retun res;
	}
}
//----------------------------------------------------------------------------------------------------------------*/	
//----------------------------------------------------------------------------------------------------------------*/	
//----------------------------------------------------------------------------------------------------------------*/	
xml_lang_t::xml_lang_t()
{
	_currency_.reset(new currency_t());
	_virtualkeyboard_.reset(new virtualkeyboard_t());
	_application_.reset(new form_t());
	_datetime_.reset(new format_date_time_t("MM/dd/yy", "hh:mm:ss"));
	_datetime_short_.reset(new format_date_time_t("MM/dd/yy", "hh:mm"));
	_phonenum_.reset(new phonenum_t("xxx-xxx-xxxx"));
	app_name_ = ""; local_name_ = ""; file_ = "";
	ref_load_v.clear();
}
//----------------------------------------------------------------------------------------------------------------*/	
xml_lang_t::xml_lang_t(const std::string& File_, const std::string& Local_name_, const std::string& App_name_)
: file_(File_), local_name_(Local_name_), app_name_(App_name_)
{
	_currency_.reset(new currency_t());
	_virtualkeyboard_.reset(new virtualkeyboard_t());
	_application_.reset(new form_t());
	_datetime_.reset(new format_date_time_t("MM/dd/yy", "hh:mm:ss"));
	_datetime_short_.reset(new format_date_time_t("MM/dd/yy", "hh:mm"));
	_phonenum_.reset(new phonenum_t("xxx-xxx-xxxx"));
	ref_load_v.clear();
	xml_file_.reset(new wxXmlDocument());
	{		
		if (!wxFile::Exists(to_uc(file_).c_str()))
		{
			wxMessageBox(to_uc("file doesn't exist: " + file_));
			//throw exception_t(3001, "file doesn't exist: " + file_)
			loaded = false;
			return;
			//THROW_MESSAGE(0, "don't exist file: " + file_, "xml_lang_t::xml_lang_t(const std::string&)");
			//xml_file_->SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, _("config")));    
		}
		else if (!xml_file_->Load(to_uc(file_))) 
		{
			wxMessageBox(to_uc("file can't open: " + file_));
			//throw exception_t(3002, "file doesn't exist: " + file_)
			loaded = false;
			return;
		}
		//THROW_MESSAGE(0, "can't open file: " + file_, "xml_lang_t::xml_lang_t(const std::string&)");
		//xml_file_->SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, _("locale")));
	}
	loaded = false;
	{
		wxXmlNode *child = xml_file_->GetRoot();
		while (child)
		{
			if (child->GetName() == to_uc("locale"))
			{
				wxString res;
				if (child->GetPropVal(to_uc("name"), &res)) 
					if (res == to_uc(local_name_)) 
					{
						load_lang(child->GetChildren(), app_name_);
						loaded = true;
						return ; 
					}
			}
			child = child->GetNext();
		}
	}

}
//----------------------------------------------------------------------------------------------------------------*/	
//----------------------------------------------------------------------------------------------------------------*/	
void xml_lang_t::ref_load(boost::shared_ptr<control_t> ctrl)
{
	try
	{
		if (ctrl->is_loaded_ref()) 
		{
			ctrl->set_failed();
			return;
		}
		//	throw exception_t(11111, "uncorrect_ref", "xml_lang_t::ref_load(...)");
		ctrl->set_loaded_ref(); 
		boost::shared_ptr<form_t> f_form;  bool fform(false);
		
		std::string sss = ctrl->get_ref();
		std::string key ("::");
		size_t pos = sss.find(key);
		std::string str1;
		
		//if (found!=string::npos)
		//	str.replace (found,key.length(),"seventh");
		if (pos != 0 && pos < sss.length()) 
		{
			str1 = sss.substr(0, pos); 
			sss = sss.substr(pos+2, (sss.length() - pos - 2));
		}
		else str1 = sss;
		
		if (str1 == app_name_) 
		{
			pos = sss.find(key);
			if (pos != 0 && pos < sss.length()) 
			{
				try
				{
					std::string str1 = sss.substr(0, pos); 
					sss = sss.substr(pos+2, (sss.length() - pos - 2));
					f_form = _application_->form(str1);
					fform = true;
					pos = sss.find(key);
					if (pos != 0 && pos < sss.length()) 
					{
						str1 = sss.substr(0, pos); 
						sss = sss.substr(pos+2, (sss.length() - pos - 2));
					}
					else str1 = sss;	
				}
				catch(...) {  }
			}
			else 
			{
				boost::shared_ptr<control_t> new_ptr = _application_->find_control(sss);
				if (new_ptr->get_ref() != "") ref_load(new_ptr);
				ctrl->copy(new_ptr);
				return;
			}
				
		}
		else
		{
			try 
			{ 
				f_form = form(str1); 
				fform = true; 
				pos = sss.find(key);
				if (pos != 0 && pos < sss.length()) 
				{
					str1 = sss.substr(0, pos); 
					sss = sss.substr(pos+2, (sss.length() - pos - 2));
				}
				else str1 = sss;
				
			} catch(...) {  }
			if (!fform)
			{
				try 
				{ 
					boost::shared_ptr<control_t> new_ptr = _application_->find_control(sss);
					if (new_ptr->get_ref() != "") ref_load(new_ptr);
					ctrl->copy(new_ptr);
					return; 
				} 
				catch(...) {  }
				for (int i=0; i<loaded_app.size(); i++)
				{
					if (loaded_app[i]->get_name() == str1)
					{
						f_form = loaded_app[i];
						fform = true;
						pos = sss.find(key);
						if (pos != 0 && pos < sss.length()) 
						{
							str1 = sss.substr(0, pos); 
							sss = sss.substr(pos+2, (sss.length() - pos - 2));
						}
						else str1 = sss;	
						break;
					}
				}
			
				if (!fform) 
				{
					try
					{
						loaded_app.push_back(load_app(str1));
						f_form = loaded_app[loaded_app.size()-1];
						pos = sss.find(key);
						if (pos != 0 && pos < sss.length()) 
						{
							str1 = sss.substr(0, pos); 
							sss = sss.substr(pos+2, (sss.length() - pos - 2));
						}
						else str1 = sss;	
						fform = true;
					}
					catch(exception_t &e)
					{
					}
				}
			}	
		}
		
		if (!fform) f_form = form(str1);
						
		while (pos != 0 && pos < sss.length())
		{
			f_form = f_form->form(str1);
			sss = sss.substr(pos+2, (sss.length() - pos - 2));
			str1 = sss.substr(0, pos);
			pos = sss.find(key);	
		}
		boost::shared_ptr<control_t> new_ptr = f_form->find_control(sss);
		if (new_ptr->get_ref() != "") ref_load(new_ptr);
		ctrl->copy(new_ptr);
			//else return control(sss, Failed);
			
		
	}
	/*catch(std::exception &e) 
	{ 
		if (e.code == 11111) return ref_load(e.message, Failed);
		else return;

	}*/
	catch(exception_t &e) 	{ 	if (e.code() == 11111) ctrl->set_failed(); throw e; }
	catch(...) 	{ 	return; }
}
//----------------------------------------------------------------------------------------------------------------*/	
void control_t::copy(boost::shared_ptr<control_t> val)
{ 
	if (val->is_failed())	set_failed();
	else
	{
		value = val->get_value();
		ref = val->get_ref();
	}
}
wxString control_t::get_value() { if (!failed) return value; throw exception_t(11111, "failed control_name", "control_t::get_value"); }
//----------------------------------------------------------------------------------------------------------------*/	
std::vector<std::string> xml_lang_t::get_locales(const std::string& File_)
{
	std::vector<std::string> locales(0);
	xml_file_.reset(new wxXmlDocument());
	{		
		if (!wxFile::Exists(to_uc(File_).c_str()))
		{
			wxMessageBox(to_uc("file doesn't exist: " + File_));
			return locales;
		}
		else if (!xml_file_->Load(to_uc(File_))) 
		{
			wxMessageBox(to_uc("file can't open: " + File_));
			return locales;
		}
	}
	loaded = false;
	
	//if ((xml_file_->GetRoot()->GetName() == _T("locale")))
	{
		wxXmlNode *child = xml_file_->GetRoot();
		while (child)
		{
			if (child->GetName() == to_uc("locale"))
			{
				wxString res;
				if (child->GetPropVal(to_uc("name"), &res)) locales.push_back(to_mb(res));
			}
			child = child->GetNext();
		}
	}
	return locales;
	//load(local_name_, app_name_);
}
//----------------------------------------------------------------------------------------------------------------*/	
void xml_lang_t::load(const std::string& File_, const std::string& Local_name_, const std::string& App_name_)
{
	_currency_.reset(new currency_t());
	_virtualkeyboard_.reset(new virtualkeyboard_t());
	_application_.reset(new form_t());
	_datetime_.reset(new format_date_time_t("MM/dd/yy", "hh:mm:ss"));
	_datetime_short_.reset(new format_date_time_t("MM/dd/yy", "hh:mm"));
	_phonenum_.reset(new phonenum_t("xxx-xxx-xxxx"));
	xml_file_.reset(new wxXmlDocument());
	file_ = File_;
	local_name_ = Local_name_;
	app_name_ = App_name_;
	{		
		if (!wxFile::Exists(to_uc(file_).c_str()))
		{
			//std::ofstream of (file_.c_str());
			//of << "<config></config>";
			//of.close();
			wxMessageBox(to_uc("file doesn't exist: " + file_));
			return;
			//THROW_MESSAGE(0, "don't exist file: " + file_, "xml_lang_t::xml_lang_t(const std::string&)");
			//xml_file_->SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, _("config")));    
		}
		else if (!xml_file_->Load(to_uc(file_))) 
		{
			wxMessageBox(to_uc("file can't open: " + file_));
			return;
		}
		//THROW_MESSAGE(0, "can't open file: " + file_, "xml_lang_t::xml_lang_t(const std::string&)");
		//xml_file_->SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, _("locale")));
	}
	loaded = false;
	
	wxXmlNode *child = xml_file_->GetRoot()/*->GetChildren()*/;	
	while (child)
	{
		if ((child->GetName() == to_uc("locale")))
		{
			wxString res;
			child->GetPropVal(to_uc("name"), &res);
			if (res == to_uc(local_name_))
			{
				load_lang(child->GetChildren(), app_name_);
				loaded = true;
				return ; //_T("");
			}
		}
		child = child->GetNext();
	}
}
//----------------------------------------------------------------------------------------------------------------*/	
boost::shared_ptr<form_t> xml_lang_t::load_app(const std::string& App_name_)
{
	boost::shared_ptr<form_t> res_form;
	wxXmlNode *child = xml_file_->GetRoot();	
	while (child)
	{
		if ((child->GetName() == to_uc("locale")))
		{
			wxString res;
			child->GetPropVal(to_uc("name"), &res);
			if (res == to_uc(local_name_))
			{
				wxXmlNode *child1 = child->GetChildren();
				while (child1)
				{
					if (child1->GetName() == to_uc("application"))
					{
						wxString Name(_T(""));
						if (!child1->GetPropVal(to_uc("name"), &Name)) Name = _T(""); //throw exception_t(6001, "invalid application_name", "langfile_t::local()");
						if (to_mb(Name) == App_name_) 
						{
							res_form.reset(new form_t(to_mb(Name), child1->GetChildren(), false));
							return res_form;
						}
					}
					child1 = child1->GetNext();
				}
				loaded = true;
				//return ; //_T("");
			}
		}
		if (child) child = child->GetNext();
	}
	throw exception_t(11112, "This locale of application "+App_name_ +" doesn't exist", "xml_lang_t::load_app");
}
//----------------------------------------------------------------------------------------------------------------*/	
void xml_lang_t::load_lang(wxXmlNode *child, const std::string& app_name_)
{
	while (child)
	{
		if (child->GetName() == to_uc("datetime_format"))
		{
			try
			{
				wxString date_f;
				wxString time_f; 
				if (!child->GetPropVal(to_uc("date_format"), &date_f)) date_f = _T("MM/dd/yy"); //throw exception_t(6001, "invalid decimalseparator", "langfile_t::local()");
				if (!child->GetPropVal(to_uc("time_format"), &time_f)) time_f = _T("hh:mm:ss"); //throw exception_t(6001, "invalid thousandseparator", "langfile_t::local()");
				_datetime_.reset(new format_date_time_t(to_mb(date_f), to_mb(time_f)));
			}
			catch(...) {_datetime_.reset(new format_date_time_t("MM/dd/yy", "hh:mm:ss")); }
		}
		if (child->GetName() == to_uc("datetime_format_short"))
		{
			try
			{
				wxString date_f;
				wxString time_f; 
				if (!child->GetPropVal(to_uc("date_format"), &date_f)) date_f = _T("MM/dd/yy"); //throw exception_t(6001, "invalid decimalseparator", "langfile_t::local()");
				if (!child->GetPropVal(to_uc("time_format"), &time_f)) time_f = _T("hh:mm"); //throw exception_t(6001, "invalid thousandseparator", "langfile_t::local()");
				_datetime_short_.reset(new format_date_time_t(to_mb(date_f), to_mb(time_f)));
			}
			catch(...) {_datetime_short_.reset(new format_date_time_t("MM/dd/yy", "hh:mm:ss")); }
		}
		if (child->GetName() == to_uc("currency"))
		{
			try
			{
				wxString Symbol(_T("$"));
				wxString Placement; int Prefix(0);
				wxString Fractionsize_str; double Fractionsize(2);
				if (!child->GetPropVal(to_uc("symbol"), &Symbol)) Symbol = to_uc("$"); //throw exception_t(6001, "invalid decimalseparator", "langfile_t::local()");
				if (!child->GetPropVal(to_uc("placement"), &Placement)) Placement = to_uc("prefix"); //throw exception_t(6001, "invalid thousandseparator", "langfile_t::local()");
				if (Placement == _T("prefix")) Prefix = 1;
				if (!child->GetPropVal(to_uc("fractionsize"), &Fractionsize_str)) Fractionsize_str = to_uc("2"); //throw exception_t(6001, "invalid negativesign", "langfile_t::local()");
				/*Fractionsize = */Fractionsize_str.ToDouble(&Fractionsize);
				_currency_->load_currencyformat(Symbol, Prefix, int(Fractionsize));
			}
			catch(...) {  }
		}
		else if (child->GetName() == to_uc("floatnumber"))
		{
			try
			{
				wxString Decimalseparator(_T("."));
				wxString Thousandseparator(_T(","));
				wxString Negativesign(_T("-"));
				if (!child->GetPropVal(to_uc("decimalseparator"), &Decimalseparator)) Decimalseparator = to_uc("."); //throw exception_t(6001, "invalid decimalseparator", "langfile_t::local()");
				if (!child->GetPropVal(to_uc("thousandseparator"), &Thousandseparator)) Thousandseparator = to_uc(","); //throw exception_t(6001, "invalid thousandseparator", "langfile_t::local()");
				if (!child->GetPropVal(to_uc("negativesign"), &Negativesign)) Negativesign = to_uc("-"); //throw exception_t(6001, "invalid negativesign", "langfile_t::local()");
				_currency_->load_floatformat(Decimalseparator, Thousandseparator, Negativesign);
			}
			catch(...) {  }
		}
		else if (child->GetName() == to_uc("phonenumber"))
		{
			try
			{
				wxString Mask(_T("xxx-xxx-xxxx"));
				if (!child->GetPropVal(to_uc("template"), &Mask)) Mask = to_uc("xxx-xxx-xxxx"); //throw exception_t(6001, "invalid decimalseparator", "langfile_t::local()");
				_phonenum_.reset(new phonenum_t(to_mb(Mask)));
			}
			catch(...) {  }
		}
		else if (child->GetName() == to_uc("virtualkeyboard"))
		{
			try
			{
				wxString Name(_T(""));
				if (!child->GetPropVal(to_uc("name"), &Name)) Name = to_uc(""); //throw exception_t(6001, "invalid decimalseparator", "langfile_t::local()");
				_virtualkeyboard_.reset(new virtualkeyboard_t(Name));
				wxXmlNode *child1 = child->GetChildren();
				while (child1)
				{
					wxString Value(_T(""));
					wxString Key(_T(""));
					if (!child1->GetPropVal(to_uc("name"), &Key)) Key = to_uc(""); //throw exception_t(6001, "invalid decimalseparator", "langfile_t::local()");
					if (!child1->GetPropVal(to_uc("val"), &Value)) Value = to_uc(""); //throw exception_t(6001, "invalid decimalseparator", "langfile_t::local()");
					_virtualkeyboard_->add(key_t(to_mb(Key), Value));
					child1 = child1->GetNext();
				}
				//_virtualkeyboard_->add(key_t("", _T("")))
			}
			catch(...) { }

		}
		else if (child->GetName() == to_uc("application"))
		{
			wxString Name(_T(""));
			if (!child->GetPropVal(to_uc("name"), &Name)) Name = _T(""); //throw exception_t(6001, "invalid application_name", "langfile_t::local()");
			if (to_mb(Name) == app_name_) _application_.reset(new form_t(to_mb(Name), child->GetChildren(), true));
			for (int i=0; i<ref_load_v.size(); i++)
			{
				ref_load(ref_load_v[i]);
			}
			ref_load_v.clear();
			loaded_app.clear();
		}
		else if (child->GetName() == to_uc("template")) {
			wxString name;
			child->GetPropVal(_("name"), &name);
			templates_[to_mb(name)] = child;
		}
		child = child->GetNext();
	}
	
}
//----------------------------------------------------------------------------------------------------------------*/	
wxString processNode(wxXmlNode* curr, const std::map<std::string, std::string>& mapper) {
	if (curr == 0) {
		return _("");
	}
	else if (curr->GetName() == _("text")) {
		return curr->GetNodeContent();
	}
	else if (curr->GetName() == _("if") || curr->GetName() == _("elseif") || curr->GetName() == _("else")) {
		wxString comp;
		curr->GetPropVal(_("comp"), &comp);
		wxString var;
		curr->GetPropVal(_("var"), &var);
		std::map<std::string, std::string>::const_iterator mapperIter = mapper.find(to_mb(var));
		if (mapperIter != mapper.end()) var = to_uc(mapperIter->second);
		wxString val;
		curr->GetPropVal(_("val"), &val);
		wxString type;
		curr->GetPropVal(_("type"), &type);
		if (comp == _("equal")) {
			if (type == _("int")) {
				if (curr->GetName() == _("else") || boost::lexical_cast<int>(to_mb(var)) == boost::lexical_cast<int>(to_mb(val))) {
					wxXmlNode* child = curr->GetChildren();
					wxString currResult;
					while (child) {
						currResult += processNode(child, mapper);
						child = child->GetNext();
					}
					return currResult;
				}
			}
			else if (type == _("str")) {
				if (curr->GetName() == _("else") || to_mb(var) == to_mb(val)) {
					wxXmlNode* child = curr->GetChildren();
					wxString currResult;
					while (child) {
						currResult += processNode(child, mapper);
						child = child->GetNext();
					}
					return currResult;
				}
			}
		}
		else if (comp == _("notequal")) {
			if (type == _("int")) {
				if (curr->GetName() == _("else") || boost::lexical_cast<int>(to_mb(var)) != boost::lexical_cast<int>(to_mb(val))) {
					wxXmlNode* child = curr->GetChildren();
					wxString currResult;
					while (child) {
						currResult += processNode(child, mapper);
						child = child->GetNext();
					}
					return currResult;
				}
			}
			else if (type == _("str")) {
				if (curr->GetName() == _("else") || to_mb(var) != to_mb(val)) {
					wxXmlNode* child = curr->GetChildren();
					wxString currResult;
					while (child) {
						currResult += processNode(child, mapper);
						child = child->GetNext();
					}
					return currResult;
				}
			}
		}
		else if (comp == _("less")) {
			if (type == _("int")) {
				if (curr->GetName() == _("else") || boost::lexical_cast<int>(to_mb(var)) < boost::lexical_cast<int>(to_mb(val))) {
					wxXmlNode* child = curr->GetChildren();
					wxString currResult;
					while (child) {
						currResult += processNode(child, mapper);
						child = child->GetNext();
					}
					return currResult;
				}
			}
			else if (type == _("str")) {
				if (curr->GetName() == _("else") || to_mb(var) < to_mb(val)) {
					wxXmlNode* child = curr->GetChildren();
					wxString currResult;
					while (child) {
						currResult += processNode(child, mapper);
						child = child->GetNext();
					}
					return currResult;
				}
			}
		}
		else if (comp == _("great")) {
			if (type == _("int")) {
				if (curr->GetName() == _("else") || boost::lexical_cast<int>(to_mb(var)) > boost::lexical_cast<int>(to_mb(val))) {
					wxXmlNode* child = curr->GetChildren();
					wxString currResult;
					while (child) {
						currResult += processNode(child, mapper);
						child = child->GetNext();
					}
					return currResult;
				}
			}
			else if (type == _("str")) {
				if (curr->GetName() == _("else") || to_mb(var) > to_mb(val)) {
					wxXmlNode* child = curr->GetChildren();
					wxString currResult;
					while (child) {
						currResult += processNode(child, mapper);
						child = child->GetNext();
					}
					return currResult;
				}
			}
		}
	}
	else {
		wxString name = curr->GetName();
		std::map<std::string, std::string>::const_iterator mapperIter = mapper.find(to_mb(name));
		if (mapperIter != mapper.end()) return to_uc(mapperIter->second);
	}
	return _("");
}

wxString xml_lang_t::get_string(const std::string& templateName, const std::map<std::string, std::string>& mapper) const {
	std::map<std::string, wxXmlNode*>::const_iterator templateIter = templates_.find(templateName);
	if (templateIter == templates_.end()) return _("");
	wxXmlNode* currNode = templateIter->second;
	currNode = currNode->GetChildren();
	wxString result;
	while (currNode) {
		result += processNode(currNode, mapper);
		currNode = currNode->GetNext();
	}
	return result;
}
//----------------------------------------------------------------------------------------------------------------*/	
	wxString xml_lang_t::get_date(const boost::posix_time::ptime& date_) { try{ return to_uc(_datetime_->date(date_)); } catch(...) { return _T(""); } }
	wxString xml_lang_t::get_time(const boost::posix_time::ptime& date_) { return to_uc(_datetime_->time(date_)); }
	wxString xml_lang_t::get_date_time(const boost::posix_time::ptime& date_) { return to_uc(_datetime_->date_time(date_)); }
	
	wxString xml_lang_t::get_date_short(const boost::posix_time::ptime& date_) { try{ return to_uc(_datetime_short_->date(date_)); } catch(...) { return _T(""); } }
	wxString xml_lang_t::get_time_short(const boost::posix_time::ptime& date_) { return to_uc(_datetime_short_->time(date_)); }
	wxString xml_lang_t::get_date_time_short(const boost::posix_time::ptime& date_) { return to_uc(_datetime_short_->date_time(date_)); }
	
	wxString xml_lang_t::get_currency(double Num) { return _currency_->format(Num); }
	double xml_lang_t::reverse_currency(wxString Num) { return _currency_->reverse(Num); }
	double xml_lang_t::reverse_float(wxString Num) { return _currency_->reverse_float(Num); }
	wxString xml_lang_t::get_float(double Num) { return _currency_->format_float(Num); }
	boost::shared_ptr<form_t> xml_lang_t::find_form(std::string Name) { return _application_->find_form(Name); } // for (size_t i=0; i<application_.size(); i++) if (application_[i].name == Name) return application_[i]; throw exception_t(6005, "This form don't exist", "xml_lang_t::find_form"); 
	boost::shared_ptr<form_t> xml_lang_t::form(std::string Name) { return _application_->find_form(Name); }
	wxString xml_lang_t::get(const std::string& FName, const std::string& CName, const std::string& Failed) 	{ return _application_->get(FName, CName, Failed); }
	std::string xml_lang_t::get_std(const std::string& FName, const std::string& CName, const std::string& Failed) { return to_mb(get(FName, CName, Failed)); }
	wxString xml_lang_t::get_phone(std::string num) { return _phonenum_->get(num); }
	wxString xml_lang_t::get_key(std::string Name) { return _virtualkeyboard_->get(Name); }	
//----------------------------------------------------------------------------------------------------------------*/	
//----------------------------------------------------------------------------------------------------------------*/	
//----------------------------------------------------------------------------------------------------------------*/	

form_t::form_t(const std::string Name, wxXmlNode *child, bool is_load_ref = false)
: name(Name) 
{
	while (child)
	{
		if (child->GetName() == to_uc("form"))
		{
			wxString Name;
			if (!child->GetPropVal(to_uc("name"), &Name)) Name = _T("");
			 boost::shared_ptr<form_t> sub_f (new form_t(to_mb(Name), child->GetChildren(), is_load_ref));
			_form_.push_back(sub_f);
		}
		if (child->GetName() == to_uc("control"))
		{
			wxString Name(_T(""));
			wxString Value(_T(""));
			wxString Ref(_T(""));
			if (!child->GetPropVal(to_uc("name"), &Name)) Name = to_uc(""); 
			if (!child->GetPropVal(to_uc("val"), &Value)) Value = to_uc("");
			if (!child->GetPropVal(to_uc("ref"), &Ref)) Ref = to_uc("");
			//control_t ctrl(control_t(to_mb(Name), Value, to_mb(Ref)));
			boost::shared_ptr<control_t> ctrl(new control_t(to_mb(Name), Value, to_mb(Ref)));
			_control_.push_back(ctrl);	
			//cccc (_control_[_control_.size()-1]);	
					
			if ((Ref != to_uc("")) && is_load_ref) 
			{
				std::string nnn1 = to_mb(Name);
				std::string nnn2 = to_mb(Ref);
				std::string nnn3 = to_mb(Value);	
				std::string nnn4 = _control_[_control_.size()-1]->get_ref();
			    ref_load_v.push_back(ctrl);
			}
		}
/*
		if (child->GetName() == to_uc("template")) {
			wxString name;
			child->GetPropVal("name", &name);
			templates_[to_mb(name)] = child;
			
			std::vector<selItem_t> selVec;
			boost::shared_ptr<mapper_t> mapperPtr;
			wxXmlNode* tmp = child->GetChildren();
			while (tmp) {
				if (tmp->GetName() == _("text")) {
					std::string text = to_mb(tmp->GetNodeContent());
					block_t block;
					block.push_back(boost::shared_ptr<item_t>(new text_t(text)));
					selBlock_t selBlock(positive(), block);
					selVec.push_back(selItem_t(selector::IF, selBlock));
				}
				else if (tmp->GetName() == _("if")) {
				}
				else if (tmp->GetName() == _("elseif")) {
				}
				else if (tmp->GetName() == _("else")) {
				}
				else {
					std::string cmd = to_mb(tmp->GetName());
					block_t block;
					block.push_back(boost::shared_ptr<item_t>(new value_t(cmd, mapperPtr)));
					selBlock_t selBlock(positive(), block);
					selVec.push_back(selItem_t(selector::IF, selBlock));
				}
				tmp = tmp->GetNext();
			}
		
		}
*/	
		child = child->GetNext();
	}
}
//----------------------------------------------------------------------------------------------------------------*/		
//----------------------------------------------------------------------------------------------------------------*/		
	
	boost::shared_ptr<control_t> form_t::get_control(int i) { return _control_[i]; }
	boost::shared_ptr<control_t> form_t::find_control(std::string Name)  { for (size_t i=0; i<_control_.size(); i++) if (_control_[i]->get_name() == Name) return _control_[i]; throw exception_t(6005, "This control dose not exist. (" + Name +")", "form_t::find_control"); }
	
	boost::shared_ptr<form_t> form_t::get_form(int i) { return _form_[i]; }
	boost::shared_ptr<form_t> form_t::find_form(std::string Name) { for (size_t i=0; i<_form_.size(); i++) if (_form_[i]->get_name() == Name) return _form_[i]; throw exception_t(6005, "This form don't exist", "form_t::find_form");  }
	
//----------------------------------------------------------------------------------------------------------------*/			
wxString form_t::control(std::string Name, std::string Failed) 
{ 
	try 
	{ 
		return (find_control(Name)->get_value()); 
	}
	catch(exception_t &e) 
	{ 
		//if (e.code() == 11111) throw e;
		return to_uc(Failed); 
	}
	catch(...) 
	{ 
		return to_uc(Failed); 
	}
}
//----------------------------------------------------------------------------------------------------------------*/		
wxString form_t::get(std::string FName, std::string CName, std::string Failed) 
{ 
	try	
	{  
		return find_form(FName)->control(CName, Failed);  
	}
	catch(exception_t &e) 
	{ 
		//if (e.code() == 11111) return get_parse_ref(e.message(), Failed);
		return to_uc(Failed); 
	}
	catch(...) 
	{ 
		return to_uc(Failed); 
	}
}
//----------------------------------------------------------------------------------------------------------------*/		
boost::shared_ptr<form_t> form_t::form(std::string Name) 
{ 
	return find_form(Name); 
	/*try {  return &(find_form(Name));  }	catch(...) { return &(form_t()); }*/
}
	
//----------------------------------------------------------------------------------------------------------------*/		

wxString form_t::get_parse_ref(std::string sss, std::string Failed)
{
	try
	{
	
		boost::shared_ptr<form_t> f_form;
		
		std::string key ("::");
		size_t pos = sss.find(key);
		
		//if (found!=string::npos)
		//	str.replace (found,key.length(),"seventh");
		bool fform(false);
		
		std::string str1;
			
		while (pos != 0 && pos < sss.length())
		{
			str1 = sss.substr(0, pos);
			sss = sss.substr(pos+2, (sss.length() - pos - 2));
			
			if (fform)f_form = f_form->form(str1);
			else { f_form = form(str1); fform = true;}
			
			pos = sss.find(key);
		}
		
		//str1 = sss.substr(1, pos);
		//sss = sss.substr(pos+2, (sss.length() - pos - 2));
		if (fform) return f_form->control(sss, Failed);
		else return control(sss, Failed);
	}
	catch(std::exception &e) 
	{ 
	   //if (e.code == 11111) return get_parse_ref(e.message, Failed);
	   return to_uc(Failed);
	}
	catch(...) 
	{ 	return to_uc(Failed); }
}

//----------------------------------------------------------------------------------------------------------------*/		
//----------------------------------------------------------------------------------------------------------------*/			
	wxString virtualkeyboard_t::get(std::string Key) { for (size_t i=0; i<storage.size(); i++) if (storage[i].key == Key) return storage[i].value; return to_uc(Key); }
	
//----------------------------------------------------------------------------------------------------------------*/		
//----------------------------------------------------------------------------------------------------------------*/			
void currency_t::load_floatformat(const wxString& Decimalseparator, const wxString& Thousandseparator, const wxString& Negativesign)
{
	float_format = float_format_t(Decimalseparator, Thousandseparator, Negativesign);
}
//----------------------------------------------------------------------------------------------------------------*/				
void currency_t::load_currencyformat(const wxString& Symbol, const int Prefix, const int Fractionsize) 
{
	currency_format = currency_format_t(Symbol, Prefix, Fractionsize);
}
//----------------------------------------------------------------------------------------------------------------*/				
wxString currency_t::format(const double& val) const 
{
	if (val >= 0) 
	{
		if (currency_format.is_prefix()) 
			return currency_format.get_symbol() + float_format.frac_format(val) + float_format.get_decimalseparator() + float_format.trunc_str(val, currency_format.get_num());
		else return float_format.frac_format(val) + float_format.get_decimalseparator() + float_format.trunc_str(val, currency_format.get_num()) + currency_format.get_symbol();
	}
	else
	{
		if (currency_format.is_prefix()) 
			return float_format.get_negativesign() + currency_format.get_symbol() + float_format.frac_format(val) + float_format.get_decimalseparator() + float_format.trunc_str(val, currency_format.get_num());
		else return float_format.get_negativesign() + float_format.frac_format(val) + float_format.get_decimalseparator() + float_format.trunc_str(val, currency_format.get_num()) + currency_format.get_symbol();
	}
}
//----------------------------------------------------------------------------------------------------------------*/			
wxString currency_t::format_float(const double& val) const 
{
	return float_format.format(val);
}
//----------------------------------------------------------------------------------------------------------------*/			
double currency_t::reverse(const wxString& str) const 
{
	//std::string str = to_mb(str_u);
	//if (str.Length() < 4) THROW_MESSAGE(0, "missing format value: "+to_mb(str), "double en_currency_t::val(const std::string&)");
	bool positive_ = true;
	wxString tmp = str;
	if (str.Mid(0, float_format.get_negativesign().Length()) == float_format.get_negativesign()) 
	{
		positive_ = false;
		tmp = str.Mid(float_format.get_negativesign().Length(), str.length()-float_format.get_negativesign().Length());
	}
	const wxChar ccc(162);
	if (tmp[0] == ccc) 
	{
	   std::string str_trunc = to_mb(tmp.Mid(1, tmp.Length()-1/*-currency_format.get_num(), currency_format.get_num()*/));
	   //wxString str_frac = tmp.Mid(0, tmp.Length()-(currency_format.get_num()+1));
	   int trunc_ = boost::lexical_cast<int>(str_trunc);
	   int bbuf(1); for (int i=0; i<str_trunc.length(); i++) bbuf *= 10;
	   if (positive_) return double(trunc_)/bbuf;
	   return -1*(double(trunc_)/bbuf);
	}
	else if (str.Length() < 4) THROW_MESSAGE(0, "missing format value: "+to_mb(str), "double en_currency_t::val(const std::string&)");
	
	if (currency_format.is_prefix()) 
	{
		if (tmp.Mid(0, currency_format.get_symbol().Length()) != currency_format.get_symbol()) THROW_MESSAGE(1, "missing format value: "+to_mb(str), "double en_currency_t::val(const std::string&)");
		tmp = tmp.Mid(currency_format.get_symbol().Length(), tmp.Length()-currency_format.get_symbol().Length());
	}
	else 
	{
		if (tmp.Mid(tmp.Length()-currency_format.get_symbol().Length(), currency_format.get_symbol().Length()) != currency_format.get_symbol()) THROW_MESSAGE(1, "missing format value: "+to_mb(str), "double en_currency_t::val(const std::string&)");
		tmp = tmp.Mid(0, tmp.Length()-currency_format.get_symbol().Length());
	}
	
	std::string str_trunc = to_mb(tmp.Mid(tmp.Length()-currency_format.get_num(), currency_format.get_num()));
	wxString str_frac = tmp.Mid(0, tmp.Length()-(currency_format.get_num()+1));
	int trunc_ = boost::lexical_cast<int>(str_trunc);
	
	if (str_frac.Length() > 3) 
	while(str_frac.Find(float_format.get_thousandseparator()) != -1)
	{
		str_frac = str_frac.Mid(0, str_frac.Find(float_format.get_thousandseparator())) + str_frac.Mid(str_frac.Find(float_format.get_thousandseparator())+1, str_frac.Length());
	}
	int frac_ = boost::lexical_cast<int>(to_mb(str_frac));
	int bbuf(1); for (int i=0; i<currency_format.get_num(); i++) bbuf *= 10;
	if (positive_) return frac_ + double(trunc_)/bbuf;
	return -1*(frac_ + double(trunc_)/bbuf);
}
//----------------------------------------------------------------------------------------------------------------*/			
double currency_t::reverse_float(const wxString& str) const 
{
	//std::string str = to_mb(str_u);
	return float_format.reverse(str);
}
//----------------------------------------------------------------------------------------------------------------*/			
//----------------------------------------------------------------------------------------------------------------*/			
int float_format_t::frac(const double& val) const 
{
	double tmp = val > 0 ? 100*val : -100*val;
	tmp += 0.5;
	return int(tmp)/100;
}
//----------------------------------------------------------------------------------------------------------------*/			
int float_format_t::trunc(const double& val, int num) const 
{
	int bbuf(1); for (int i=0; i<num; i++) bbuf *= 10;
	double tmp = val > 0 ? bbuf*val : -bbuf*val;
	tmp += 0.5;
	return int(tmp)-(int(tmp)/bbuf)*bbuf;
}
//----------------------------------------------------------------------------------------------------------------*/		
std::string float_format_t::frac_str(const double& val) const {
	return boost::lexical_cast<std::string>(frac(val));
}
//----------------------------------------------------------------------------------------------------------------*/		
wxString float_format_t::trunc_str(const double& val, int num) const 
{
	//if (trunc(val, num) == 0) return _("");
	std::string res = boost::lexical_cast<std::string>(trunc(val, num));
	while (res.length() < num) res = "0" + res;
	return to_uc(res); //boost::lexical_cast<std::string>(trunc(val, num)/10)+boost::lexical_cast<std::string>(trunc(val)-10*(trunc(val)/10));
}
//----------------------------------------------------------------------------------------------------------------*/			
wxString float_format_t::frac_format(const double& val) const 
{
	std::string tmp = frac_str(val);
	wxString res(wxT(""));
	int rr = tmp.length();
	if (tmp.length() > 3) for (int i=res.length(); i > 0; i-3)
	{			
		res = thousandseparator + to_uc(tmp.substr(i, i+3)) + res;
		rr = i;
	}
	res = to_uc(tmp.substr(0,rr)) + res;	
	return res;
}
//----------------------------------------------------------------------------------------------------------------*/			
wxString float_format_t::format(const double& val) const 
{
	
	wxString res;
	if (val >= 0) res = frac_format(val) + ((trunc(val, 6) != 0) ? (get_decimalseparator() + trunc_str(val, 6)) : to_uc(""));
	else res = get_negativesign() + frac_format(val) + ((trunc(val, 6) != 0) ? (get_decimalseparator() + trunc_str(val, 6)) : to_uc(""));
	if (trunc(val, 6) != 0) while (res.Last() == '0') res = res.RemoveLast(); //.erase(res.find_last_not_of('0')+1);
	return res;
}
//----------------------------------------------------------------------------------------------------------------*/			
double float_format_t::reverse(const wxString& str) const
{
	bool positive_ = true;
	wxString tmp = str;
	if (str.Mid(0, negativesign.Length()) == negativesign)
	{
		positive_ = false;
		tmp = str.Mid(negativesign.Length(), str.Length()-negativesign.Length());
	}
	std::string str_trunc = "";
	wxString str_frac = tmp;
	if (tmp.Find(thousandseparator) != -1)
	{
		str_trunc = to_mb(tmp.Mid(tmp.Find(thousandseparator)+1, tmp.Length()));
		str_frac = tmp.Mid(0, tmp.Find(thousandseparator));
	}
	int trunc_ = boost::lexical_cast<int>(str_trunc);
	
	if (str_frac.Length() > 3) 
	while(str_frac.Find(thousandseparator))
	{
		str_frac = str_frac.Mid(0, str_frac.Find(thousandseparator)) + str_frac.Mid(str_frac.Find(thousandseparator)+1, str_frac.Length());
	}
	int frac_num = str_frac.Length();
	int frac_ = boost::lexical_cast<int>(to_mb(str_frac));
	int bbuf(1); for (int i=0; i<frac_num; i++) bbuf *= 10;
	if (positive_) return frac_ + double(trunc_)/bbuf;
	return -1*(frac_ + double(trunc_)/bbuf);
}
//----------------------------------------------------------------------------------------------------------------*/		
//----------------------------------------------------------------------------------------------------------------*/		
//----------------------------------------------------------------------------------------------------------------*/			

wxString phonenum_t::get(std::string num)
{ 
	std::string res("");
	char _10[] = "0123456789";
	std::string num_("");
	for (int j=0; j<num.length(); j++)
		for (int i=0; i<10; i++) if (_10[i] == num[j]) { num += num[j]; break; }
		
	int cnt = 0;
	for (int i=0; i<mask.length(); i++)
	{
		if( mask[i] == 'x') 
		{
			if (cnt > num_.length()) break;
			res += num_[cnt++];
		}
		else res += mask[i];
	}
		
	if (cnt < num_.length()) res += num_.substr(cnt, num_.length()-cnt);
	return to_uc(res); 
}

};
//----------------------------------------------------------------------------------------------------------------*/	
