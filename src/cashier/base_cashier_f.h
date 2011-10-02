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
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_cashier_f
///////////////////////////////////////////////////////////////////////////////
class base_cashier_f : public wxFrame 
{
	private:
	
	protected:
		wxBoxSizer* main_sizer;
		wxPanel* top_pnl;
		wxBoxSizer* top_bar_sizer;
		wxStaticText* current_dt_lb;
		
		wxStaticText* current_user_lb;
		
		wxButton* open_cash_drawer_btn;
		wxButton* browse_delayed_orders_btn;
		wxButton* logout_btn;
		wxPanel* body_pnl;
		wxPanel* btn_pnl;
		wxPanel* m_panel61;
		wxBoxSizer* r_sizer;
		wxOrderTreeListCtrl* sg;
		wxPanel* total_pnl;
		wxBoxSizer* total_pnl_sizer;
		wxBoxSizer* subtotal_sizer;
		wxStaticText* m_subtotal_lb;
		wxStaticText* m_subtotal_ed;
		wxBoxSizer* tax_sizer;
		wxStaticText* m_sales_tax_lb;
		wxStaticText* m_sales_tax_ed;
		wxBoxSizer* gratuity_sizer;
		wxStaticText* m_gratuity_lb;
		wxStaticText* m_gratuity_ed;
		wxStaticText* m_tools_lb;
		wxStaticText* m_tools_ed;
		wxStaticText* m_total_lb;
		wxStaticText* m_total_ed;
		wxPanel* ctrl_btn_pnl;
		wxBoxSizer* ctrl_sizer;
		wxButton* comment_entree_btn;
		
		wxBitmapButton* plus_btn;
		
		wxBitmapButton* minus_btn;
		
		wxBitmapButton* delete_btn;
		wxGridBagSizer* grid_btnSizer;
		wxButton* for_here_btn;
		wxButton* to_go_btn;
		wxButton* delivery_btn;
		wxButton* pickup_btn;
		wxButton* employee_meal_btn;
		wxButton* donation_btn;
		wxButton* misc_charges_btn;
		wxButton* entree_code_btn;
		wxButton* discount_btn;
		wxButton* save_and_hide_btn;
		wxButton* token_btn;
		wxButton* delayed_order_details_btn;
		wxButton* refund_order_btn;
		wxButton* process_btn;
		wxButton* cash_advance_btn;
		wxButton* shift_btn;
		wxStaticLine* buttom_hline;
		wxPanel* status_pnl;
		wxStaticText* status_lb;
		
		// Virtual event handlers, overide them in your derived class
		virtual void open_cash_drawer_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void browse_delayed_orders_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void logout_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void comment_entree_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void plus_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void minus_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void delete_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void order_type_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void misc_charges_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void entree_code_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void discount_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void save_and_hide_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void token_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void delayed_order_details_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void refund_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void process_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cash_advance_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void shift_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnConfigDbClick( wxMouseEvent& event ){ event.Skip(); }
		
	
	public:
		base_cashier_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024,768 ), long style = 0|wxTAB_TRAVERSAL );
		~base_cashier_f();
	
};

#endif //__base_cashier_f__
