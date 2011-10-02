///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_dlg_f__
#define __base_dlg_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_dlg_f
///////////////////////////////////////////////////////////////////////////////
class base_dlg_f : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* main_sizer;
		wxStaticText* message_ed;
		wxBoxSizer* btn_sizer;
	
	public:
		base_dlg_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION );
		~base_dlg_f();
	
};

#endif //__base_dlg_f__
