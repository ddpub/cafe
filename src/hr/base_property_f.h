///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_property_f__
#define __base_property_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_BTNOK 1000
#define ID_BTNCANCEL 1001

///////////////////////////////////////////////////////////////////////////////
/// Class base_property_f
///////////////////////////////////////////////////////////////////////////////
class base_property_f : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* port_lp;
		wxComboBox* port_cb;
		wxButton* btn_test;
		wxCheckBox* ck_lang;
		wxStaticText* lb_file;
		
		wxTextCtrl* ed_lang;
		wxButton* btn_load_lang;
		
		wxStaticText* lb_locale_name;
		
		wxComboBox* cb_locale;
		
		wxButton* btn_ok;
		
		wxButton* btn_cancel;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnComChanged( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTest( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckLang( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnFileChange( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnLoadFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnLocaleLoad( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_property_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 301,222 ), long style = wxDEFAULT_DIALOG_STYLE );
		~base_property_f();
	
};

#endif //__base_property_f__
