///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_person_f__
#define __base_person_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/radiobut.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/checklst.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define wID_ANY 1000

///////////////////////////////////////////////////////////////////////////////
/// Class base_person_f
///////////////////////////////////////////////////////////////////////////////
class base_person_f : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* lb_login;
		wxTextCtrl* t_login;
		wxStaticText* lb_firstname;
		wxTextCtrl* t_firstname;
		wxStaticText* lb_lastname;
		wxTextCtrl* t_lastname;
		wxStaticText* lb_surname;
		wxTextCtrl* t_surname;
		wxStaticText* lb_position;
		wxComboBox* bx_position;
		wxStaticText* lb_hire;
		wxDatePickerCtrl* dt_hire;
		wxStaticText* lb_ssn;
		wxTextCtrl* t_ssn;
		wxStaticText* lb_birthday;
		wxDatePickerCtrl* dt_birthday;
		wxStaticText* lb_state;
		wxComboBox* bx_state;
		wxStaticText* lb_city;
		wxTextCtrl* t_city;
		wxStaticText* lb_address1;
		wxTextCtrl* t_address1;
		wxStaticText* lb_address2;
		wxTextCtrl* t_address2;
		wxStaticText* lb_zip;
		wxTextCtrl* t_zip;
		wxStaticText* lb_business_mail;
		wxTextCtrl* t_mail_business;
		wxStaticText* lb_private_mail;
		wxTextCtrl* t_mail_private;
		wxStaticText* lb_phone_home;
		wxTextCtrl* t_phone_home;
		wxStaticText* lb_phone_cell;
		wxTextCtrl* t_phone_cell;
		wxStaticText* lb_phone_business;
		wxTextCtrl* t_phone_business;
		wxStaticText* lb_ename;
		wxTextCtrl* t_ename;
		wxStaticText* lb_ephone;
		wxTextCtrl* t_ephone;
		wxStaticText* lb_pwd;
		wxTextCtrl* t_password;
		wxStaticText* lb_confpwd;
		wxTextCtrl* t_confpassword;
		wxCheckBox* t_magncard;
		
		wxRadioButton* rb_hi;
		
		wxRadioButton* rb_low;
		
		wxStaticText* lb_descr;
		wxTextCtrl* t_descr;
		wxStaticText* lb_photo;
		wxStaticBitmap* t_photo;
		wxStaticText* lb_cafe;
		wxCheckListBox* ck_cafe;
		wxStaticText* lb_role;
		wxCheckListBox* ck_role;
		
		
		wxButton* btn_ok;
		
		wxButton* btn_cancel;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnLoginCtrl( wxKeyEvent& event ){ event.Skip(); }
		virtual void OnCheckPos( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPwdCtrl( wxKeyEvent& event ){ event.Skip(); }
		virtual void OnConfPwdCtrl( wxKeyEvent& event ){ event.Skip(); }
		virtual void OnZipCtrl( wxKeyEvent& event ){ event.Skip(); }
		virtual void OnCheckCard( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnClickHi( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnClickLow( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnLoadBmp( wxMouseEvent& event ){ event.Skip(); }
		virtual void OnClearBmp( wxMouseEvent& event ){ event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_person_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Person's details"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxFULL_REPAINT_ON_RESIZE );
		~base_person_f();
	
};

#endif //__base_person_f__
