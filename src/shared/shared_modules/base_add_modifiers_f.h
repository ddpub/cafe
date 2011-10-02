///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_add_modifiers_f__
#define __base_add_modifiers_f__

class wxModifierTreeListCtrl;

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx_util/treelistctrl/treelistctrl.h>
#ifdef __VISUALC__
#include <wx/link_additions.h>
#endif //__VISUALC__
#include <wx/bmpbuttn.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_add_modifiers_f
///////////////////////////////////////////////////////////////////////////////
class base_add_modifiers_f : public wxDialog 
{
	private:
	
	protected:
		wxPanel* btn_pnl;
		
		wxButton* prev_page_btn;
		
		wxButton* next_page_btn;
		
		wxPanel* m_panel21;
		wxModifierTreeListCtrl* sg;
		wxPanel* m_panel4;
		
		wxBitmapButton* plus_btn;
		
		wxBitmapButton* minus_btn;
		
		wxBitmapButton* delete_btn;
		
		wxButton* ok_btn;
		
		wxButton* cancel_btn;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void prev_page_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void next_page_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void plus_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void minus_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void delete_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void ok_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_add_modifiers_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_DIALOG_STYLE );
		~base_add_modifiers_f();
	
};

#endif //__base_add_modifiers_f__
