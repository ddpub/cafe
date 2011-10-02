///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_delayed_order_f__
#define __base_delayed_order_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_delayed_order_f
///////////////////////////////////////////////////////////////////////////////
class base_delayed_order_f : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* main_sizer;
		wxBoxSizer* main_input_item_sizer;
		wxFlexGridSizer* input_item_sizer;
		wxStaticText* firstname_lb;
		wxTextCtrl* fname_ed;
		wxStaticText* lastname_lb;
		wxTextCtrl* lname_ed;
		wxStaticText* street_address_lb;
		wxTextCtrl* address_ed;
		wxStaticText* city_lb;
		wxTextCtrl* city_ed;
		wxStaticText* zip_lb;
		wxTextCtrl* zip_ed;
		wxStaticText* state_lb;
		wxChoice* state_cb;
		wxStaticText* phone_lb;
		wxTextCtrl* phone_ed;
		wxStaticText* yellow_lb;
		wxButton* select_table_btn;
		wxStaticBoxSizer* delivery_pickup_cap;
		wxButton* set_date_btn;
		wxButton* set_time_btn;
		
		wxStaticBoxSizer* prepayment_cap;
		wxButton* prepayment_btn;
		wxButton* cancel_prepayment_btn;
		wxButton* auth_cc_btn;
		wxButton* cancel_auth_cc_btn;
		wxButton* print_slip_btn;
		wxButton* cancel_order_btn;
		wxStaticBoxSizer* note_cap;
		wxTextCtrl* order_notes_ed;
		
		wxButton* save_btn;
		
		wxButton* save_and_print_btn;
		
		wxButton* save_and_hide_btn;
		
		wxButton* cancel_btn;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void fname_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void lname_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void address_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void city_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void zip_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void state_cbChoice( wxCommandEvent& event ){ event.Skip(); }
		virtual void phone_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void select_table_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void set_date_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void set_time_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void prepayment_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_prepayment_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void auth_cc_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_auth_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void print_sleep_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_order_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void order_notes_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void save_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void save_and_print_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void save_and_hide_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_delayed_order_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION );
		~base_delayed_order_f();
	
};

#endif //__base_delayed_order_f__
