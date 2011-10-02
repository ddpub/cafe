///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_info_f__
#define __base_info_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_info_f
///////////////////////////////////////////////////////////////////////////////
class base_info_f : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* main_sizer;
		wxStaticText* label_lb;
		wxStaticText* change_lb;
		wxButton* close_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void close_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_info_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,-1 ), long style = wxCAPTION );
		~base_info_f();
	
};

#endif //__base_info_f__
