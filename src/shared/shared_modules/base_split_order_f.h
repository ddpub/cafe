///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_split_order_f__
#define __base_split_order_f__

class wxSplitOrderTreeListCtrl;

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx_util/treelistctrl/treelistctrl.h>
#ifdef __VISUALC__
#include <wx/link_additions.h>
#endif //__VISUALC__
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/spinbutt.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_split_order_f
///////////////////////////////////////////////////////////////////////////////
class base_split_order_f : public wxDialog 
{
	private:
	
	protected:
		wxStaticBoxSizer* source_size;
		wxSplitOrderTreeListCtrl* source_sg;
		
		wxTextCtrl* source_guests_ed;
		wxSpinButton* m_spinBtn1;
		
		wxButton* source_table_btn;
		
		
		wxButton* move_from_source_btn;
		
		wxButton* move_to_source_btn;
		
		wxStaticBoxSizer* destincation_sizer;
		wxSplitOrderTreeListCtrl* destination_sg;
		
		wxTextCtrl* destination_guests_ed;
		wxSpinButton* m_spinBtn2;
		
		wxButton* destination_table_btn;
		
		
		wxButton* ok_btn;
		
		wxButton* cancel_btn;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void sourceSpinDownClick( wxSpinEvent& event ){ event.Skip(); }
		virtual void sourceSpinUpClick( wxSpinEvent& event ){ event.Skip(); }
		virtual void move_from_source_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void move_to_source_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void destinationSpinDownClick( wxSpinEvent& event ){ event.Skip(); }
		virtual void destionationSpinUpClick( wxSpinEvent& event ){ event.Skip(); }
		virtual void destination_table_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void ok_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_split_order_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_DIALOG_STYLE );
		~base_split_order_f();
	
};

#endif //__base_split_order_f__
