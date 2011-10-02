///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_position_f__
#define __base_position_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/checklst.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_position_f
///////////////////////////////////////////////////////////////////////////////
class base_position_f : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* lb_name;
		wxTextCtrl* tc_posname;
		wxStaticText* lb_role;
		wxCheckListBox* cl_role;
		
		wxButton* btn_ok;
		
		wxButton* btn_cancel;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnEnd( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_position_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Position editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 293,369 ), long style = wxDEFAULT_DIALOG_STYLE );
		~base_position_f();
	
};

#endif //__base_position_f__
