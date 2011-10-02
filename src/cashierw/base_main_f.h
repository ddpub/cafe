///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_main_f__
#define __base_main_f__

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
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_main_f
///////////////////////////////////////////////////////////////////////////////
class base_main_f : public wxFrame 
{
	private:
	
	protected:
		wxBoxSizer* main_sizer;
		wxPanel* top_pnl;
		wxBoxSizer* top_sizer;
		wxStaticText* room_lb;
		
		wxStaticText* current_dt_lb;
		
		wxStaticText* current_user_lb;
		
		wxButton* browse_delayed_orders_btn;
		
		wxButton* open_orders_btn;
		
		wxButton* logout_btn;
		wxStaticLine* top_line;
		wxPanel* body_pnl;
		wxBoxSizer* body_sizer;
		wxPanel* btn_pnl;
		wxStaticLine* body_line;
		wxPanel* control_btn_pnl;
		wxBoxSizer* control_btn_sizer;
		wxButton* open_cash_drawer_btn;
		
		wxButton* close_order_operation_btn;
		
		wxButton* cash_advance_btn;
		
		wxButton* create_table_booking_order_btn;
		
		wxButton* create_to_go_order_btn;
		
		wxButton* create_delivery_order_btn;
		
		wxButton* create_pickup_order_btn;
		
		wxButton* create_employee_meal_order_btn;
		wxStaticLine* bottom_line;
		wxPanel* bottom_pnl;
		wxBoxSizer* bottom_sizer;
		wxStaticText* status_lb;
		
		// Virtual event handlers, overide them in your derived class
		virtual void browse_delayed_orders_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void open_orders_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void logout_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void open_cash_drawer_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void close_order_operation_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cash_advance_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void create_table_booking_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void create_to_go_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void create_delivery_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void create_pickup_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void create_employee_meal_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnConfigDbClick( wxMouseEvent& event ){ event.Skip(); }
		
	
	public:
		base_main_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024,768 ), long style = 0|wxTAB_TRAVERSAL );
		~base_main_f();
	
};

#endif //__base_main_f__
