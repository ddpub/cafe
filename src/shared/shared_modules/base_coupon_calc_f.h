///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_coupon_calc_f__
#define __base_coupon_calc_f__

#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_coupon_calc_f
///////////////////////////////////////////////////////////////////////////////
class base_coupon_calc_f : public wxDialog 
{
	private:
	
	protected:
		wxButton* procent_btn;
		
		wxButton* money_btn;
		
		wxTextCtrl* procent_ed;
		
		wxTextCtrl* money_ed;
		
		wxButton* num07_btn;
		wxButton* num08_btn;
		wxButton* num09_btn;
		wxButton* num04_btn;
		wxButton* num05_btn;
		wxButton* num06_btn;
		wxButton* num01_btn;
		wxButton* num02_btn;
		wxButton* num03_btn;
		wxButton* num00_btn;
		wxButton* clear_btn;
		
		wxButton* ok_btn;
		
		wxButton* cancel_btn;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void onChar( wxKeyEvent& event ){ event.Skip(); }
		virtual void type_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void num_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void clear_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void ok_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_coupon_calc_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxTAB_TRAVERSAL|wxWANTS_CHARS );
		~base_coupon_calc_f();
	
};

#endif //__base_coupon_calc_f__
