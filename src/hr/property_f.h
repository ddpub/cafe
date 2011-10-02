
#ifndef property_fH
#define property_fH

/*#include <wx/checkbox.h>
#include <wx/datetime.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/dialog.h>
#include <wx/calctrl.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/filedlg.h>*/
#include "base_property_f.h"
//#include <time.h>

class HRproperty_f : public base_property_f
{
private:
	//wxCheckBox *ck_lang;
	//wxTextCtrl *ed_lang;
	//wxButton *btn_ok;
	//wxButton *btn_cancel;
	//wxButton *btn_load_lang;
	//wxComboBox *cb_locale;
	//wxStaticText *lb_locale_name;
	//wxStaticText *port_lp;
	//wxStaticText *lb_file;
	
	//wxComboBox *port_cb;
	int actived_id;
	wxDateTime res;
	std::string file;
	std::string locale;
	
	std::string Port;
protected:
	void OnClose(wxCommandEvent &event);
	void OnComChanged(wxCommandEvent &event);
	void OnCheckLang(wxCommandEvent &event);
	void OnLocaleLoad(wxCommandEvent &event);
	void OnFileChange(wxCommandEvent &event);
	void OnLoadFile(wxCommandEvent &event);
	void OnTest(wxCommandEvent &event);
	
public:
	HRproperty_f(wxWindow *parent);
	//~HRproperty_f();
	int Showdlg() { return ShowModal(); }
	void reload_lang();
	//DECLARE_EVENT_TABLE();
};

#endif
