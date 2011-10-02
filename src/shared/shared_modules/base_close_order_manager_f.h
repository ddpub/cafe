///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_close_order_manager_f__
#define __base_close_order_manager_f__

class wxRefundTreeListCtrl;

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
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_close_order_manager_f
///////////////////////////////////////////////////////////////////////////////
class base_close_order_manager_f : public wxDialog 
{
	private:
	
	protected:
		wxRefundTreeListCtrl* sg;
		wxBoxSizer* buttom_bar_sizer;
		
		wxButton* find_order_btn;
		
		wxButton* print_receipt_duplicate_btn;
		
		wxButton* refund_order_btn;
		
		wxButton* change_payment_type_btn;
		
		wxButton* reopen_order_btn;
		
		wxButton* close_btn;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void find_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void print_receipt_duplicate_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void refund_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void change_payment_type_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void reopen_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void close_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_close_order_manager_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxCAPTION );
		~base_close_order_manager_f();
	
};

#endif //__base_close_order_manager_f__
