///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_manual_entry_f__
#define __base_manual_entry_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_manual_entry_f
///////////////////////////////////////////////////////////////////////////////
class base_manual_entry_f : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* card_num_ed;
		wxStaticText* m_staticText5;
		wxTextCtrl* exp_month_ed;
		wxStaticText* m_staticText6;
		wxTextCtrl* exp_year_ed;
		wxTextCtrl* cvv2_ed;
		wxButton* num7_btn;
		wxButton* num8_btn;
		wxButton* num9_btn;
		wxButton* num4_btn;
		wxButton* num5_btn;
		wxButton* num6_btn;
		wxButton* num1_btn;
		wxButton* num2_btn;
		wxButton* num3_btn;
		wxButton* num0_btn;
		wxButton* clear_btn;
		wxButton* ok_btn;
		wxButton* cancel_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void card_numFocus( wxFocusEvent& event ){ event.Skip(); }
		virtual void monthFocus( wxFocusEvent& event ){ event.Skip(); }
		virtual void yearFocus( wxFocusEvent& event ){ event.Skip(); }
		virtual void cvv2Focus( wxFocusEvent& event ){ event.Skip(); }
		virtual void num_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void clear_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void ok_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_manual_entry_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION );
		~base_manual_entry_f();
	
};

#endif //__base_manual_entry_f__
