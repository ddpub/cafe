///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_cashier_f__
#define __base_cashier_f__

class wxOrderTreeListCtrl;

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx_util/treelistctrl/treelistctrl.h>
#ifdef __VISUALC__
#include <wx/link_additions.h>
#endif //__VISUALC__
#include <wx/bmpbuttn.h>
#include <wx/gbsizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_cashier_f
///////////////////////////////////////////////////////////////////////////////
class base_cashier_f : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* main_sizer;
		wxPanel* top_pnl;
		wxBoxSizer* top_bar_sizer;
		wxStaticText* room_lb;
		
		wxStaticText* current_dt_lb;
		
		wxStaticText* current_user_lb;
		
		wxButton* browse_delayed_orders_btn;
		wxButton* open_orders_btn;
		wxButton* logout_btn;
		wxPanel* body_pnl;
		wxPanel* btn_pnl;
		wxPanel* rpanel;
		wxBoxSizer* rsizer;
		wxOrderTreeListCtrl* sg;
		wxPanel* total_pnl;
		wxGridSizer* total_gSizer;
		wxStaticText* m_subtotal_lb;
		wxStaticText* m_subtotal_ed;
		wxStaticText* m_sales_tax_lb;
		wxStaticText* m_sales_tax_ed;
		wxStaticText* m_gratuity_lb;
		wxStaticText* m_gratuity_ed;
		wxStaticText* m_tools_lb;
		wxStaticText* m_tools_ed;
		wxStaticText* m_total_lb;
		wxStaticText* m_total_ed;
		wxPanel* ctrl_btn_pnl;
		wxBoxSizer* ctrl_sizer;
		wxButton* misc_charges_btn;
		
		wxButton* entree_code_btn;
		
		wxButton* comment_entree_btn;
		
		wxBitmapButton* plus_btn;
		
		wxBitmapButton* minus_btn;
		wxButton* order_type_btn;
		wxButton* guest_btn;
		wxButton* discount_btn;
		wxButton* token_btn;
		wxButton* delayed_order_details_btn;
		wxButton* save_changes_btn;
		wxButton* close_order_btn;
		wxButton* cancel_changes_btn;
		wxButton* print_receipt_duplicate_btn;
		wxButton* other_orders_operations_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void browse_delayed_orders_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void open_orders_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void logout_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void misc_charges_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void entree_code_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void comment_entree_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void plus_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void minus_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void order_type_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void guest_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void discount_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void token_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void delayed_order_details_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void save_changes_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void close_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_changes_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void print_receipt_duplicate_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void other_orders_operations_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_cashier_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024,768 ), long style = 0 );
		~base_cashier_f();
	
};

#endif //__base_cashier_f__
