///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_tip_amount_f__
#define __base_tip_amount_f__

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
/// Class base_tip_amount_f
///////////////////////////////////////////////////////////////////////////////
class base_tip_amount_f : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* mainbSizer;
		wxStaticText* amount_lb;
		wxTextCtrl* amount_ed;
		wxStaticText* tip_lb;
		wxTextCtrl* tip_ed;
		wxStaticText* total_lb;
		wxTextCtrl* total_ed;
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
		
		wxButton* hide_btn;
		
		wxButton* cancel_btn;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void clear_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void ok_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void hide_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_tip_amount_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 455,412 ), long style = wxDEFAULT_DIALOG_STYLE );
		~base_tip_amount_f();
	
};

#endif //__base_tip_amount_f__
