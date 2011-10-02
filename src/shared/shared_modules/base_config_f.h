///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_config_f__
#define __base_config_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/radiobut.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_config_f
///////////////////////////////////////////////////////////////////////////////
class base_config_f : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook;
		wxPanel* m_panel1;
		wxStaticText* lock_timeout_lb;
		wxChoice* lock_timeout_cb;
		wxStaticText* server_status_check_lb;
		wxChoice* server_status_check_cb;
		wxStaticText* frac_lb;
		wxChoice* frac_cb;
		wxCheckBox* vfd_chb;
		wxStaticBoxSizer* vfd_sizer;
		wxStaticText* vfd_align_lb;
		
		wxStaticText* vfd_text_lb;
		wxChoice* fline_choice;
		wxStaticText* fline_lb;
		wxTextCtrl* fline_ed;
		wxChoice* sline_choice;
		wxStaticText* sline_lb;
		wxTextCtrl* sline_ed;
		wxCheckBox* auto_resolution_chb;
		wxStaticBoxSizer* resolution_sizer;
		wxStaticText* screen_width_lb;
		wxChoice* resolution_width_cb;
		wxStaticText* screen_height_lb;
		wxChoice* resolution_height_cb;
		wxCheckBox* stay_on_top_chb;
		
		wxButton* terminate_app_btn;
		
		wxPanel* m_panel2;
		wxBoxSizer* bSizer3;
		wxRadioButton* cc_heartland_rb;
		wxStaticBoxSizer* cc_account_sizer;
		wxFlexGridSizer* cc_fg_sizer;
		wxStaticText* License_id_lb;
		wxTextCtrl* license_ed;
		wxStaticText* site_id_lb;
		wxTextCtrl* site_ed;
		wxStaticText* device_id_lb;
		wxTextCtrl* device_ed;
		wxStaticText* user_lb;
		wxTextCtrl* user_ed;
		wxStaticText* password_lb;
		wxTextCtrl* password_ed;
		wxButton* test_credentials_btn;
		wxBoxSizer* tip_sizer;
		wxStaticText* max_tip_val_lb;
		wxChoice* max_tips_value_cb;
		wxRadioButton* cc_stand_alone_rb;
		wxRadioButton* cc_disable_rb;
		wxPanel* m_panel3;
		wxStaticBoxSizer* printer_sizer;
		wxRadioButton* local_printer_chb;
		wxStaticBoxSizer* local_printer_sizer;
		wxStaticText* printer_port_lb;
		wxChoice* printer_port_cb;
		wxStaticText* command_set_lb;
		wxChoice* command_set_cb;
		wxStaticText* baud_rate_lb;
		wxChoice* baud_rate_cb;
		wxRadioButton* net_printer_chb;
		wxStaticBoxSizer* host_settings_sizer;
		wxStaticText* host_lb;
		
		wxTextCtrl* printer_host_ed;
		wxRadioButton* no_printer_chb;
		wxStaticBoxSizer* printings_settings_sizer;
		wxCheckBox* group_by_chb;
		wxCheckBox* print_logo_chb;
		wxButton* test_printer_btn;
		
		wxPanel* m_panel5;
		wxStaticBoxSizer* securty_sizer;
		wxCheckBox* auth_reciept_duplicate_chb;
		wxCheckBox* auth_refund_chb;
		wxCheckBox* auth_app_settings_chb;
		wxCheckBox* auth_misc_charges_chb;
		wxCheckBox* auth_reload_all_menus_chb;
		wxCheckBox* auth_table_booking_chb;
		wxCheckBox* auth_shift_chb;
		wxPanel* m_panel6;
		wxCheckBox* entree_codes_chb;
		wxCheckBox* misc_charges_chb;
		wxCheckBox* tokens_chb;
		wxCheckBox* cash_input_chb;
		wxCheckBox* close_order_operation_auto_close_chb;
		wxCheckBox* auto_lock_chb;
		wxCheckBox* cash_advance_chb;
		wxCheckBox* comment_entree_chb;
		wxCheckBox* discount_chb;
		wxCheckBox* trainig_mode_chb;
		wxCheckBox* us_mode_chb;
		wxCheckBox* allow_remove_entrees_chb;
		wxStaticBoxSizer* order_type_sizer;
		wxCheckBox* for_here_chb;
		wxCheckBox* to_go_chb;
		wxCheckBox* delivery_chb;
		wxCheckBox* pickup_chb;
		wxCheckBox* employee_meal_chb;
		wxCheckBox* donation_chb;
		wxCheckBox* table_booking_chb;
		wxPanel* m_panel4;
		wxCheckBox* load_locale_chb;
		wxStaticText* file_lb;
		wxTextCtrl* file_ed;
		wxButton* select_file_btn;
		wxStaticText* locales_lb;
		wxChoice* locale_cb;
		wxButton* close_btn;
		
		wxButton* cancel_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void fline_choiceChange( wxCommandEvent& event ){ event.Skip(); }
		virtual void fline_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void sline_choiceChange( wxCommandEvent& event ){ event.Skip(); }
		virtual void sline_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void auto_resolution_chbCheck( wxCommandEvent& event ){ event.Skip(); }
		virtual void stay_on_top_chbCheck( wxCommandEvent& event ){ event.Skip(); }
		virtual void terminate_app_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cc_processing_rgClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void license_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void EditFocus( wxFocusEvent& event ){ event.Skip(); }
		virtual void site_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void device_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void user_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void password_edClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void test_credentials_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void local_printer_chbCheck( wxCommandEvent& event ){ event.Skip(); }
		virtual void net_printer_chbCheck( wxCommandEvent& event ){ event.Skip(); }
		virtual void no_printer_chbCheck( wxCommandEvent& event ){ event.Skip(); }
		virtual void print_logoCheck( wxCommandEvent& event ){ event.Skip(); }
		virtual void test_printer_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void order_typeCheck( wxCommandEvent& event ){ event.Skip(); }
		virtual void load_locale_chbCheck( wxCommandEvent& event ){ event.Skip(); }
		virtual void select_file_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void locale_chbClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void close_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_config_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,600 ), long style = wxCAPTION );
		~base_config_f();
	
};

#endif //__base_config_f__
