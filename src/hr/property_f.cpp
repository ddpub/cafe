#include "property_f.h"

#include <wx/filedlg.h>
#include <wx_util/xml_config.hpp>
#include "lang/xml_lang.h"
#include <serial/com.hpp>
#include <vector>
#include "mcard.h"

extern std::auto_ptr<com_t> com;
extern std::auto_ptr<ksi_cafe::xml_lang_t> local;
extern std::auto_ptr<xml_config_t> config;

extern bool is_valide_com;

HRproperty_f::HRproperty_f(wxWindow *parent) 
: base_property_f(parent, wxID_ANY, local->get("prop_f", "caption", "Settings"))
{

	port_lp->SetLabel(local->get("prop_f", "com_port", "Com port"));
	ck_lang->SetLabel(local->get("prop_f", "lang_ck", "Load lanuage"));
	lb_file->SetLabel(local->get("prop_f", "loc_file", "File name"));
	lb_locale_name->SetLabel(local->get("prop_f", "locale_name", "Locale name"));
	btn_ok->SetLabel(local->get("prop_f", "ok", "Ok"));
	btn_cancel->SetLabel(local->get("prop_f", "cancel", "Cancel"));
	
	//msr_sizer->SetLabel(local->get("prop_f", "msr_setting", "Settings for MSR206"));
	//locale_sizer->SetLabel(local->get("prop_f", "locale_setting", "Locale settings"));
	
	if (config->get("locale", "exist") == "1") 
	{
		ck_lang->SetValue(true);
		cb_locale->Enable();
		ed_lang->Enable();
		btn_load_lang->Enable();
	}
	else
	{
		cb_locale->Disable();
		ed_lang->Disable();
		btn_load_lang->Disable();
	}
	if (config->get("locale", "file") != "") 
	{
		ed_lang->SetValue(to_uc(config->get("locale", "file")));
		cb_locale->Clear();
		std::vector<std::string> locales = local->get_locales(config->get("locale", "file"));
		for (size_t i=0; i<locales.size(); i++)	cb_locale->Append(to_uc(locales[i]));
	}
	if (config->get("locale", "name") != "") 
	{
		cb_locale->SetValue(to_uc(config->get("locale", "name")));
	}	
	Port = config->get("com_port", "port");
	try {port_cb->SetValue(to_uc(Port));} catch(...) {  }
}

void HRproperty_f::OnComChanged(wxCommandEvent &event)
{
	try
	{
		std::string Port1 = to_mb(event.GetString());
		if (Port != Port1) 
		{
			com.reset(new com_t(Port1, 9600/*boost::lexical_cast<int>(config->get("com_port", "baud_rate"))*/, 2));
			Port = Port1;
			is_valide_com = true;
		}
	}
	catch (exception_t& ex) {
		wxMessageBox(to_uc(ex.user_message()));
		is_valide_com = false;
	}
	catch (...) {
		wxMessageBox(_("can't open serial port"));
		is_valide_com = false;
	}
}

void HRproperty_f::OnCheckLang(wxCommandEvent &event)
{
	if (ck_lang->IsChecked())
	{
		cb_locale->Enable();
		ed_lang->Enable();
		btn_load_lang->Enable();
	}
	else
	{
		cb_locale->Disable();
		ed_lang->Disable();
		btn_load_lang->Disable();
	}
	Update();
}

void HRproperty_f::OnLoadFile(wxCommandEvent &event) 
{
	wxFileDialog *dlg = new wxFileDialog(this);
	dlg->SetWildcard(wxT("XML files (*.xml)|*.xml"));
	if (dlg->ShowModal() != wxID_OK) return;
	ed_lang->SetValue(dlg->GetPath());
	return;	
}


void HRproperty_f::OnClose(wxCommandEvent &event) 
{
	if (event.GetId() == ID_BTNOK)
	{
		try
		{
			if (ck_lang->IsChecked())	
			{
				config->set("locale", "exist", "1");
				config->set("locale", "file", to_mb(ed_lang->GetValue()));
				config->set("locale", "name", to_mb(cb_locale->GetValue()));
				local.reset(new ksi_cafe::xml_lang_t(config->get("locale", "file"), config->get("locale", "name"), "hr_manager"));
			}
			else 
			{
				config->set("locale", "exist", "0");
				local.reset(new ksi_cafe::xml_lang_t());
			}
		}
		catch (exception_t& ex) 
		{
			wxMessageBox(to_uc(ex.user_message()));
		}
		catch (...) 
		{
			wxMessageBox(_("locale - unexpected error"));
		}
		try
		{
			config->set("com_port", "port", to_mb(port_cb->GetValue()));
			config->set("com_port", "baud_rate", "9600");
			//com.reset(new com_t(config->get("com_port", "port"), boost::lexical_cast<int>(config->get("com_port", "baud_rate")), 2));
			//res = event.GetDate();
		}
		catch (exception_t& ex) 
		{
			wxMessageBox(to_uc(ex.user_message()));
		}
		catch (...) 
		{
			wxMessageBox(_("can't open serial port"));
		}
		EndModal(1);
	}
	else if (event.GetId() == ID_BTNCANCEL)
	{
		
		try
		{
			if (config->get("locale", "exist") == "1") local.reset(new ksi_cafe::xml_lang_t(config->get("locale", "file"), config->get("locale", "name"), "hr_manager"));
			else local.reset(new ksi_cafe::xml_lang_t());
		}
		catch (exception_t& ex) {
			wxMessageBox(to_uc(ex.user_message()));
			is_valide_com = false;
		}
		catch (...) {
			wxMessageBox(_("locale - unexpected error"));
			is_valide_com = false;
		}
		
		try
		{
			if (config->get("com_port", "port") != to_mb(port_cb->GetValue()))	
			{
				com.reset(new com_t(config->get("com_port", "port"), 9600/*boost::lexical_cast<int>(config->get("com_port", "baud_rate"))*/, 2));
				is_valide_com = true;
			}
		}
		catch (exception_t& ex) {
			wxMessageBox(to_uc(ex.user_message()));
			is_valide_com = false;
		}
		catch (...) {
			wxMessageBox(_("locale - unexpected error"));
			is_valide_com = false;
		}
		EndModal(2);
	}
}

void HRproperty_f::OnLocaleLoad(wxCommandEvent &event)
{
	try
	{
		local.reset(new ksi_cafe::xml_lang_t(to_mb(ed_lang->GetValue()), to_mb(cb_locale->GetValue()),"hr_manager"));
	
		port_lp->SetLabel(local->get("prop_f", "com_port", "Com port"));
		ck_lang->SetLabel(local->get("prop_f", "lang_ck", "Load lanuage"));
		lb_file->SetLabel(local->get("prop_f", "loc_file", "File name"));
		lb_file->SetLabel(local->get("prop_f", "loc_file", "File name"));
		lb_locale_name->SetLabel(local->get("prop_f", "locale_name", "Locale name"));
		btn_ok->SetLabel(local->get("prop_f", "ok", "Ok"));
		btn_cancel->SetLabel(local->get("prop_f", "cancel", "Cancel"));
		
		Update();
	}
	catch (exception_t& ex) 
	{
			wxMessageBox(to_uc(ex.user_message()));
	}
	catch (...) 
	{
		wxMessageBox(_("locale - unexpected error"));
	}
	
}

void HRproperty_f::OnFileChange(wxCommandEvent &event)
{
	cb_locale->Clear();
	std::vector<std::string> locales = local->get_locales(to_mb(ed_lang->GetValue()));
	for (size_t i=0; i<locales.size(); i++)	cb_locale->Append(to_uc(locales[i]));	
}

void HRproperty_f::OnTest(wxCommandEvent &event)
{
	if (!is_valide_com) wxMessageBox(local->get("errors", "com_notvalid", "COM-port is not valid"));
	else if (MSR_test()) wxMessageBox(local->get("prop_f", "good_MSR_test", "MSR-206 has been found"));	
}



