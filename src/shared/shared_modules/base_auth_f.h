///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_auth_f__
#define __base_auth_f__

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/gbsizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class base_auth_f
///////////////////////////////////////////////////////////////////////////////
class base_auth_f : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* main_sizerV;
		wxStaticBitmap* m_cc_bmp;
		wxStaticText* message_lb;
		wxFlexGridSizer* auth_sizerFG;
		wxStaticText* login_lb;
		wxTextCtrl* login_ed;
		wxStaticText* password_lb;
		wxTextCtrl* password_ed;
		wxStaticText* host_lb;
		wxTextCtrl* host_ed;
		wxGridBagSizer* btn_sizeGB;
		wxButton* ok_btn;
		wxButton* show_kbd_btn;
		wxButton* advanced_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onChar( wxKeyEvent& event ){ event.Skip(); }
		virtual void onKeyDown( wxKeyEvent& event ){ event.Skip(); }
		virtual void login_edSetFocus( wxFocusEvent& event ){ event.Skip(); }
		virtual void password_edSetFocus( wxFocusEvent& event ){ event.Skip(); }
		virtual void host_edSetFocus( wxFocusEvent& event ){ event.Skip(); }
		virtual void ok_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void show_kbd_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void advanced_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxButton* cancel_btn;
		base_auth_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("auth"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxSTAY_ON_TOP|wxTAB_TRAVERSAL|wxWANTS_CHARS, const wxString& name = wxT("auth_f") );
		~base_auth_f();
	
};

#endif //__base_auth_f__
