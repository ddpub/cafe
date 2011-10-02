///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_select_time_f__
#define __base_select_time_f__

#include <wx/sizer.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_select_time_f
///////////////////////////////////////////////////////////////////////////////
class base_select_time_f : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* main_sizer;
		wxStaticBoxSizer* hours_sizer;
		wxGridSizer* hgird;
		wxStaticBoxSizer* minutes_sizer;
		wxGridSizer* mgrid;
		wxButton* m00_btn;
		wxButton* m15_btn;
		wxButton* m30_btn;
		wxButton* m45_btn;
		
		wxButton* ok_btn;
		
		wxButton* cancel_btn;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void m_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void ok_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_select_time_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION );
		~base_select_time_f();
	
};

#endif //__base_select_time_f__
