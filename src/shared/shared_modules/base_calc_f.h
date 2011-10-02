///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_calc_f__
#define __base_calc_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/gbsizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_calc_f
///////////////////////////////////////////////////////////////////////////////
class base_calc_f : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* main_sizer;
		wxStaticText* cash_tendered_lb;
		wxTextCtrl* cash_tendered_ed;
		wxStaticText* order_total_lb;
		wxTextCtrl* order_total_ed;
		wxStaticText* change_lb;
		wxTextCtrl* change_ed;
		wxButton* exact_amount_btn;
		
		wxButton* process_btn;
		
		wxButton* cancel_btn;
		wxButton* num7_btn;
		wxButton* num8_btn;
		wxButton* num9_btn;
		wxButton* num4_btn;
		wxButton* num5_btn;
		wxButton* num6_btn;
		wxButton* num1_btn;
		wxButton* num2_btn;
		wxButton*  num3_btn;
		wxButton* num0_btn;
		wxButton* num00_btn;
		wxButton* clear_btn;
		wxButton* speed_num100_btn;
		wxButton* speed_num50_btn;
		wxButton* speed_num20_btn;
		wxButton* speed_num10_btn;
		wxButton* speed_num5_btn;
		wxButton* speed_num1_btn;
		wxButton* speed_num05_btn;
		wxButton* speed_num005_btn;
		wxButton* speed_num025_btn;
		wxButton* speed_num01_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onChar( wxKeyEvent& event ){ event.Skip(); }
		virtual void cash_tendered_edChange( wxCommandEvent& event ){ event.Skip(); }
		virtual void exact_amount_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void process_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void num_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void clear_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void speed_num_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_calc_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxTAB_TRAVERSAL|wxWANTS_CHARS );
		~base_calc_f();
	
};

#endif //__base_calc_f__
