///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_select_date_f__
#define __base_select_date_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_select_date_f
///////////////////////////////////////////////////////////////////////////////
class base_select_date_f : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* main_sizer;
		wxGridSizer* grid;
		wxStaticText* su_st;
		wxStaticText* mo_st;
		wxStaticText* tu_st;
		wxStaticText* we_st;
		wxStaticText* th_st;
		wxStaticText* ft_st;
		wxStaticText* sa_st;
		
		wxButton* ok_btn;
		
		wxButton* cancel_btn;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void ok_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_select_date_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION );
		~base_select_date_f();
	
};

#endif //__base_select_date_f__
