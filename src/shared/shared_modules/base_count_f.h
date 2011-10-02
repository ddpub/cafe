///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_count_f__
#define __base_count_f__

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/spinbutt.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_count_f
///////////////////////////////////////////////////////////////////////////////
class base_count_f : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* val_ed;
		wxSpinButton* spin_btn;
		wxButton* ok_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void spin_btnDown( wxSpinEvent& event ){ event.Skip(); }
		virtual void spin_btnUp( wxSpinEvent& event ){ event.Skip(); }
		virtual void ok_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_count_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION );
		~base_count_f();
	
};

#endif //__base_count_f__
