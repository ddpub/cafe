///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 17 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_menu_grid_f__
#define __base_menu_grid_f__

#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_menu_grid_f
///////////////////////////////////////////////////////////////////////////////
class base_menu_grid_f : public wxDialog 
{
	private:
	
	protected:
		wxListBox* menu_list;
		
		// Virtual event handlers, overide them in your derived class
		virtual void menu_click( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_menu_grid_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxPoint( 10,10 ), const wxSize& size = wxSize( -1,-1 ), long style = wxCLOSE_BOX );
		~base_menu_grid_f();
	
};

#endif //__base_menu_grid_f__
