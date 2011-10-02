#include "config_f.h"
#include <wx_util/xml_config.hpp>
#include <memory>
#include <wx_util/wx_util.hpp>
#include <ksi_libpos/pos.hpp>
#include <boost/lexical_cast.hpp>
#include <wx/msgdlg.h>
#include "text_f.h"
#include <serial/com.hpp>
#include <printer/printer.hpp>
#include <boost/lexical_cast.hpp>
#include <ksi_util/log.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <printer/xml_check_printer.hpp>
#include "fin_load_f.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <lang/xml_lang.h>
#include <wx/filedlg.h>
#include <fiscal_register/fiscal.hpp>


extern std::auto_ptr<xml_config_t> config;
extern std::auto_ptr<file_log_t> flog;
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

config_f::config_f( wxWindow* parent, const wxString& title )
:
base_config_f( parent )
{
	SetLabel(title);
	
	fline_lb->SetLabel(locale->get("config_f", "fline_lb", "Line 1"));
	sline_lb->SetLabel(locale->get("config_f", "sline_lb", "Line 2"));
	m_notebook->SetPageText(0, locale->get("config_f", "main_tab", "Main options"));
	m_notebook->SetPageText(1, locale->get("config_f", "cc_tab", "CC Settings"));
	m_notebook->SetPageText(2, locale->get("config_f", "printer_tab", "Printer settings"));
	m_notebook->SetPageText(3, locale->get("config_f", "security_tab", "Security policy"));
	m_notebook->SetPageText(4, locale->get("config_f", "functionalty_tab", "Functionality"));
	m_notebook->SetPageText(5, locale->get("config_f", "other_tab", "Other options"));
	close_btn->SetLabel(locale->get("config_f", "close_btn", "Save and close"));
	cancel_btn->SetLabel(locale->get("config_f", "cancel_btn", "Cancel"));
	lock_timeout_lb->SetLabel(locale->get("config_f", "lock_timeout_lb", "Application lock timeout"));
	server_status_check_lb->SetLabel(locale->get("config_f", "server_status_check_lb", "Server status check"));
	auto_lock_chb->SetLabel(locale->get("config_f", "auto_lock_chb", "Lock application after each action"));
	frac_lb->SetLabel(locale->get("config_f", "frac_lb", "Number of decimals to be typed in cash payment"));
	vfd_chb->SetLabel(locale->get("config_f", "vfd_chb", "Enable VFD customer display"));
	vfd_sizer->GetStaticBox()->SetLabel(locale->get("config_f", "vfd_sizer", "Customer display welcome message"));
	vfd_align_lb->SetLabel(locale->get("config_f", "vfd_align_lb", "Align"));
	vfd_text_lb->SetLabel(locale->get("config_f", "vfd_text_lb", "Text"));
	stay_on_top_chb->SetLabel(locale->get("config_f", "stay_on_top_chb", "Always On Top"));
	auto_resolution_chb->SetLabel(locale->get("config_f", "auto_resolution_chb", "Auto resolution full screen"));
	resolution_sizer->GetStaticBox()->SetLabel(locale->get("config_f", "resolution_sizer", "Window Resolution"));
	terminate_app_btn->SetLabel(locale->get("config_f", "terminate_app_btn", "Terminate application"));
	screen_width_lb->SetLabel(locale->get("config_f", "screen_width_lb", "Width"));
	screen_height_lb->SetLabel(locale->get("config_f", "screen_height_lb", "Height"));
	stay_on_top_chb->SetLabel(locale->get("config_f", "stay_on_top_chb", "Always On Top"));
	resolution_sizer->Layout();
	
	cc_stand_alone_rb->SetLabel(locale->get("config_f", "cc_stand_alone_rb", "Use standalone CC processing device"));
	cc_disable_rb->SetLabel(locale->get("config_f", "cc_disable_rb", "Disable credit card processing"));
	cc_heartland_rb->SetLabel(locale->get("config_f", "cc_processing_chb", "Use Heartland Payment System merchant account"));
	cc_account_sizer->GetStaticBox()->SetLabel(locale->get("config_f", "cc_account_sizer", "Heartland merchant account settings"));
	License_id_lb->SetLabel(locale->get("config_f", "License_id_lb", "License_id"));
	site_id_lb->SetLabel(locale->get("config_f", "site_id_lb", "Site_id"));
	device_id_lb->SetLabel(locale->get("config_f", "device_id_lb", "Device_id"));
	user_lb->SetLabel(locale->get("config_f", "user_lb", "User"));
	password_lb->SetLabel(locale->get("config_f", "password_lb", "Password"));
	test_credentials_btn->SetLabel(locale->get("config_f", "test_credentials_btn", "Test credentials"));
	max_tip_val_lb->SetLabel(locale->get("config_f", "max_tip_val_lb", "Max. tips value:"));
	cc_account_sizer->Layout();
	tip_sizer->Layout();
	bSizer3->Layout();
	
	printer_sizer->GetStaticBox()->SetLabel(locale->get("config_f", "printer_sizer", "Receipt printer"));
	local_printer_chb->SetLabel(locale->get("config_f", "local_printer_chb", "Local printer"));
	local_printer_sizer->GetStaticBox()->SetLabel(locale->get("config_f", "local_printer_sizer", "Hardware settings"));
	printer_port_lb->SetLabel(locale->get("config_f", "printer_port_lb", "Printer port"));
	command_set_lb->SetLabel(locale->get("config_f", "command_set_lb", "Command set"));
	baud_rate_lb->SetLabel(locale->get("config_f", "baud_rate_lb", "Baud rate"));
	net_printer_chb->SetLabel(locale->get("config_f", "net_printer_chb", "Net printer (needs remote print server)"));
	host_settings_sizer->GetStaticBox()->SetLabel(locale->get("config_f", "host_settings_sizer", "Host settings"));
	host_lb->SetLabel(locale->get("config_f", "host_lb", "Host"));
	no_printer_chb->SetLabel(locale->get("config_f", "no_printer_chb", "No printer"));
	printings_settings_sizer->GetStaticBox()->SetLabel(locale->get("config_f", "printings_settings_sizer", "Printing settings"));
	group_by_chb->SetLabel(locale->get("config_f", "group_by_chb", "Group entrees by kitchen groups"));
	print_logo_chb->SetLabel(locale->get("config_f", "print_logo_chb", "Print logo"));
	test_printer_btn->SetLabel(locale->get("config_f", "test_printer_btn", "Test printer"));
	
	securty_sizer->GetStaticBox()->SetLabel(locale->get("config_f", "securty_sizer", "Operations needed manager's authorization"));
	auth_reciept_duplicate_chb->SetLabel(locale->get("config_f", "auth_reciept_duplicate_chb", "Print receipt duplicate"));
	auth_refund_chb->SetLabel(locale->get("config_f", "auth_refund_chb", "Refund order"));
	auth_app_settings_chb->SetLabel(locale->get("config_f", "auth_app_settings_chb", "Access to application settings"));
	auth_misc_charges_chb->SetLabel(locale->get("config_f", "auth_misc_charges_chb", "Create misc charges"));
	auth_reload_all_menus_chb->SetLabel(locale->get("config_f", "auth_reload_all_menus_chb", "Reload all menus"));
	if (config->get("ext", "mode") == "1") auth_shift_chb->Show(true); else auth_shift_chb->Show(false);
	auth_shift_chb->SetLabel(locale->get("config_f", "auth_shift_chb", "Seances"));
	auth_table_booking_chb->SetLabel(locale->get("config_f", "auth_table_booking_chb", "Create table booking"));
	
	order_type_sizer->GetStaticBox()->SetLabel(locale->get("config_f", "order_type_sizer", "Order types"));
	trainig_mode_chb->SetLabel(locale->get("config_f", "trainig_mode_chb", "Traning mode"));
	entree_codes_chb->SetLabel(locale->get("config_f", "entree_codes_chb", "Entree codes"));
	cash_advance_chb->SetLabel(locale->get("config_f", "cash_advance_chb", "Cash advance"));
	comment_entree_chb->SetLabel(locale->get("config_f", "comment_entree_chb", "Comment entree"));
	discount_chb->SetLabel(locale->get("config_f", "discount_chb", "Coupons and discounts"));
	us_mode_chb->SetLabel(locale->get("config_f", "us_mode_chb", "Mandatory zip code field for delayed orders"));
	allow_remove_entrees_chb->SetLabel(locale->get("config_f", "allow_remove_entrees_chb", "Allow to remove entrees which already has been sent to kitchen"));
	misc_charges_chb->SetLabel(locale->get("config_f", "misc_charges_chb", "Misc charges"));	
	tokens_chb->SetLabel(locale->get("config_f", "tokens_chb", "Tokens"));	
	cash_input_chb->SetLabel(locale->get("config_f", "cash_input_chb", "Show cash window upon order payment"));	
	close_order_operation_auto_close_chb->SetLabel(
	locale->get("config_f", "close_order_operation_auto_close_chb", "Close \"Operations with closed orders\" window after first action")
	);
	auto_lock_chb->SetLabel(locale->get("config_f", "auto_lock_chb", "Lock application after each action"));	
	for_here_chb->SetLabel(locale->get("config_f", "for_here_chb", "For here"));
	to_go_chb->SetLabel(locale->get("config_f", "to_go_chb", "To go"));
	delivery_chb->SetLabel(locale->get("config_f", "delivery_chb", "Delivery"));
	pickup_chb->SetLabel(locale->get("config_f", "pickup_chb", "Pickup"));
	employee_meal_chb->SetLabel(locale->get("config_f", "employee_meal_chb", "Employee meal"));
	donation_chb->SetLabel(locale->get("config_f", "donation_chb", "Donation"));
	table_booking_chb->SetLabel(locale->get("config_f", "table_booking_chb", "Table booking"));
	
	if (config->get("functionality", "order_type_for_here") == "0" || to_uc(config->get("functionality", "order_type_for_here")).MakeUpper() == _("OFF")) 
		for_here_chb->SetValue(false);
	else for_here_chb->SetValue(true);
	if (config->get("functionality", "order_type_to_go") == "0" || to_uc(config->get("functionality", "order_type_to_go")).MakeUpper() == _("OFF")) 
		to_go_chb->SetValue(false);
	else to_go_chb->SetValue(true);
	if (config->get("functionality", "order_type_delivery") == "0" || to_uc(config->get("functionality", "order_type_delivery")).MakeUpper() == _("OFF")) 
		delivery_chb->SetValue(false);
	else delivery_chb->SetValue(true);
	if (config->get("functionality", "order_type_pickup") == "0" || to_uc(config->get("functionality", "order_type_pickup")).MakeUpper() == _("OFF"))
		pickup_chb->SetValue(false);
	else pickup_chb->SetValue(true);
	if (config->get("functionality", "order_type_employee_meal") == "0" || to_uc(config->get("functionality", "order_type_employee_meal")).MakeUpper() == _("OFF"))
		employee_meal_chb->SetValue(false);
	else employee_meal_chb->SetValue(true);
	if (config->get("functionality", "order_type_donation") == "0" || to_uc(config->get("functionality", "order_type_donation")).MakeUpper() == _("OFF"))
		donation_chb->SetValue(false);
	else donation_chb->SetValue(false);	
	if (config->get("functionality", "order_type_table_booking") == "0" || to_uc(config->get("functionality", "order_type_table_booking")).MakeUpper() == _("OFF"))
		table_booking_chb->SetValue(false);
	else table_booking_chb->SetValue(true);
	
	if (config->get("functionality", "entree_codes") == "0" || to_uc(config->get("functionality", "entree_codes")).MakeUpper() == _("OFF"))
		entree_codes_chb->SetValue(false);
	else entree_codes_chb->SetValue(true);
	if (config->get("functionality", "misc_charges") == "0" || to_uc(config->get("functionality", "misc_charges")).MakeUpper() == _("OFF"))
		misc_charges_chb->SetValue(false);
	else misc_charges_chb->SetValue(true);
	if (config->get("functionality", "tokens") == "0" || to_uc(config->get("functionality", "tokens")).MakeUpper() == _("OFF"))
		tokens_chb->SetValue(false);
	else tokens_chb->SetValue(true);
	if (config->get("functionality", "cash_input") == "0" || to_uc(config->get("functionality", "cash_input")).MakeUpper() == _("OFF"))
		cash_input_chb->SetValue(false);
	else cash_input_chb->SetValue(true);
	if (config->get("functionality", "comment_entree") == "0" || to_uc(config->get("functionality", "comment_entree")).MakeUpper() == _("OFF"))
		comment_entree_chb->SetValue(false);
	else
		comment_entree_chb->SetValue(true);
	if (config->get("functionality", "cash_advance") == "0" || to_uc(config->get("functionality", "cash_advance")).MakeUpper() == _("OFF"))
		cash_advance_chb->SetValue(false);
	else
		cash_advance_chb->SetValue(true);
	if (config->get("functionality", "dicount") == "0" || to_uc(config->get("functionality", "discount")).MakeUpper() == _("OFF"))
		discount_chb->SetValue(false);
	else
		discount_chb->SetValue(true);

	if (config->get("functionality", "us_mode") == "0" || to_uc(config->get("functionality", "us_mode")).MakeUpper() == _("OFF"))
		us_mode_chb->SetValue(false);
	else us_mode_chb->SetValue(true);
	
	if (config->get("functionality", "allow_remove_kitchen_entrees") == "0" || to_uc(config->get("functionality", "allow_remove_kitchen_entrees")).MakeUpper() == _("OFF"))
		allow_remove_entrees_chb->SetValue(false);
	else allow_remove_entrees_chb->SetValue(true);
	
	if (config->get("functionality", "close_order_operation_f_auto_close") == "0" || to_uc(config->get("functionality", "close_order_operation_f_auto_close")).MakeUpper() == _("OFF"))  {
		close_order_operation_auto_close_chb->SetValue(false);
	}
	else close_order_operation_auto_close_chb->SetValue(true);
	
	
	CentreOnParent();	
	
	if (config->get("security", "print_receipt_duplicate") == "0" || to_uc(config->get("security", "print_receipt_duplicate")).MakeUpper() == _("OFF"))
		auth_reciept_duplicate_chb->SetValue(false); else auth_reciept_duplicate_chb->SetValue(true);
	if (config->get("security", "refund_order") == "0" || to_uc(config->get("security", "refund_order")).MakeUpper() == _("OFF"))
		auth_refund_chb->SetValue(false); else auth_refund_chb->SetValue(true);
	if (config->get("security", "access_to_settings") == "0" || to_uc(config->get("security", "access_to_settings")).MakeUpper() == _("OFF"))
		auth_app_settings_chb->SetValue(false); else auth_app_settings_chb->SetValue(true);	
	if (config->get("security", "create_misc_charges") == "0" || to_uc(config->get("security", "create_misc_charges")).MakeUpper() == _("OFF"))
		auth_misc_charges_chb->SetValue(false); else auth_misc_charges_chb->SetValue(true);	
	if (config->get("security", "reload_all_menus") == "0" || to_uc(config->get("security", "reload_all_menus")).MakeUpper() == _("OFF"))
		auth_reload_all_menus_chb->SetValue(false); else auth_reload_all_menus_chb->SetValue(true);	
	if (config->get("security", "seances") == "0" || to_uc(config->get("security", "seances")).MakeUpper() == _("OFF"))
		auth_shift_chb->SetValue(false); else auth_shift_chb->SetValue(true);
	if (config->get("security", "create_table_booking") == "0" || to_uc(config->get("security", "create_table_booking")).MakeUpper() == _("OFF"))
		auth_table_booking_chb->SetValue(false); else auth_table_booking_chb->SetValue(true);
	
	if (config->get("null", "trainig_mode") == "1" || to_uc(config->get("null", "trainig_mode")).MakeUpper() == _("ON")) trainig_mode_chb->SetValue(true);
	else trainig_mode_chb->SetValue(false);
	printer_port_cb->SetStringSelection(to_uc(config->get("printer", "port")).MakeUpper());
	command_set_cb->SetStringSelection(to_uc(config->get("printer", "command_set")).MakeUpper());
	baud_rate_cb->SetStringSelection(to_uc(config->get("printer", "baud_rate")).MakeUpper());
	printer_host_ed->SetValue(to_uc(config->get("printer", "host")));
	if (to_uc(config->get("printer", "source")).MakeUpper() == _("LOCAL")) {
		local_printer_chb->SetValue(true);
		printer_port_cb->Enable(true);
		command_set_cb->Enable(true);
		baud_rate_cb->Enable(true);
		printer_host_ed->Enable(false);
	}
	else if (to_uc(config->get("printer", "source")).MakeUpper() == _("REMOTE")) {
		printer_port_cb->Enable(false);
		command_set_cb->Enable(false);
		baud_rate_cb->Enable(false);
		printer_host_ed->Enable(true);
		net_printer_chb->SetValue(true);
	}
	else {
		printer_port_cb->Enable(false);
		command_set_cb->Enable(false);
		baud_rate_cb->Enable(false);
		printer_host_ed->Enable(false);
		no_printer_chb->SetValue(true);
	}
	
	if (config->get("reciept", "group_by_kitchen_id") == "1" || to_uc(config->get("reciept", "group_by_kitchen_id")).MakeUpper() == _("ON"))
		group_by_chb->SetValue(true);
	else group_by_chb->SetValue(false);
	
	if (config->get("printer", "logo") == "1" || to_uc(config->get("printer", "logo")).MakeUpper() == _("ON"))
		print_logo_chb->SetValue(true);
	else print_logo_chb->SetValue(false);
	
	if (to_uc(config->get("Credit_Card_SubSystem", "processing")).MakeUpper() == _("HEARTLAND")) {
		cc_heartland_rb->SetValue(true);
		cc_stand_alone_rb->SetValue(false);
		cc_disable_rb->SetValue(false);
		license_ed->Enable(true);
		site_ed->Enable(true);
		device_ed->Enable(true);
		user_ed->Enable(true);
		password_ed->Enable(true);
		max_tips_value_cb->Enable(true);
		test_credentials_btn->Enable(true);
	}
	else if (to_uc(config->get("Credit_Card_SubSystem", "processing")).MakeUpper() == _("STAND_ALONE")) {
		cc_stand_alone_rb->SetValue(true);
		cc_heartland_rb->SetValue(false);
		cc_disable_rb->SetValue(false);
		license_ed->Enable(false);
		site_ed->Enable(false);
		device_ed->Enable(false);
		user_ed->Enable(false);
		password_ed->Enable(false);
		max_tips_value_cb->Enable(false);
		test_credentials_btn->Enable(false);
	}
	else {
		cc_heartland_rb->SetValue(false);
		cc_stand_alone_rb->SetValue(false);
		cc_disable_rb->SetValue(true);
		license_ed->Enable(false);
		site_ed->Enable(false);
		device_ed->Enable(false);
		user_ed->Enable(false);
		password_ed->Enable(false);
		max_tips_value_cb->Enable(false);
		test_credentials_btn->Enable(false);	
	}
	
	license_ed->SetValue(to_uc(config->get("Credit_Card_SubSystem", "license_id")));
	site_ed->SetValue(to_uc(config->get("Credit_Card_SubSystem", "site_id")));
	device_ed->SetValue(to_uc(config->get("Credit_Card_SubSystem", "device_id")));
	user_ed->SetValue(to_uc(config->get("Credit_Card_SubSystem", "user")));
	password_ed->SetValue(to_uc(config->get("Credit_Card_SubSystem", "password")));
	
	max_tips_value_cb->SetStringSelection(to_uc(config->get("Credit_Card_SubSystem", "max_tips_val")));
	lock_timeout_cb->SetStringSelection(to_uc(config->get("app", "lock_timeout")));
	server_status_check_cb->SetStringSelection(to_uc(config->get("app", "check_timeout")));
	frac_cb->SetStringSelection(to_uc(config->get("app", "frac")));
	
	if (config->get("app", "auto_lock") == "1" || to_uc(config->get("app", "auto_lock")).MakeUpper() == _("ON"))
		auto_lock_chb->SetValue(true);
	else
		auto_lock_chb->SetValue(false);
	
	if (config->get("vfd_display", "enable") == "1" || to_uc(config->get("vfd_display", "enable")).MakeUpper() == _("ON"))
		vfd_chb->SetValue(true);
	else
		vfd_chb->SetValue(false);
	
	fline_ed->SetValue(to_uc(config->get("vfd_display", "line1_text")));
	sline_ed->SetValue(to_uc(config->get("vfd_display", "line2_text")));
	
	if (to_uc(config->get("vfd_display", "line1_align")).MakeUpper() == _("LEFT")) {
		fline_choice->SetSelection(0);
		fline_ed->SetWindowStyle(fline_ed->GetWindowStyle() | wxTE_LEFT);
	}
	else if (to_uc(config->get("vfd_display", "line1_align")).MakeUpper() == _("RIGHT")) {
		fline_choice->SetSelection(2);
		fline_ed->SetWindowStyle(fline_ed->GetWindowStyle() | wxTE_RIGHT);
	}
	else {
		fline_choice->SetSelection(1);
		fline_ed->SetWindowStyle(fline_ed->GetWindowStyle() | wxTE_CENTRE);
	}
	//fline_ed->Refresh();
	
	if (to_uc(config->get("vfd_display", "line2_align")).MakeUpper() == _("LEFT")) {
		sline_choice->SetSelection(0);
		sline_ed->SetWindowStyle(sline_ed->GetWindowStyle() | wxTE_LEFT);
	}
	else if (to_uc(config->get("vfd_display", "line2_align")).MakeUpper() == _("RIGHT")) {
		sline_choice->SetSelection(2);
		sline_ed->SetWindowStyle(sline_ed->GetWindowStyle() | wxTE_RIGHT);
	}
	else {
		sline_choice->SetSelection(1);
		sline_ed->SetWindowStyle(sline_ed->GetWindowStyle() | wxTE_CENTRE);
	}
	//sline_ed->Refresh();
	
	if (config->get("resolution", "stay_on_top") == "0" || to_uc(config->get("resolution", "stay_on_top")).MakeUpper() == _("OFF")) {
		stay_on_top_chb->SetValue(false);
	}
	else {
		stay_on_top_chb->SetValue(true);
	}
	
	if (config->get("resolution", "auto") == "0" || to_uc(config->get("resolution", "auto")).MakeUpper() == _("OFF")) {
		auto_resolution_chb->SetValue(false);
		resolution_width_cb->Enable(true);
		resolution_height_cb->Enable(true);
	}
	else {
		auto_resolution_chb->SetValue(true);
		resolution_width_cb->Enable(false);
		resolution_height_cb->Enable(false);
	}
	resolution_width_cb->SetStringSelection(to_uc(config->get("resolution", "width")));
	resolution_height_cb->SetStringSelection(to_uc(config->get("resolution", "height")));
	
	{
		wxString filename = to_uc(config->get("locale", "file"));
		wxString locale_name = to_uc(config->get("locale", "name"));
		file_ed->SetLabel(filename);
	
		if ( !filename.empty() ) {
			// work with the file
			try {
				std::vector<std::string> locales = locale->get_locales(to_mb(filename));
				locale_cb->Clear();
				for (size_t i=0; i<locales.size(); ++i) {
					locale_cb->Append(to_uc(locales[i]));
					//if (to_uc(locales[i]) == locale_name) locale_cb->SetSelection(i);
				}
				locale_cb->SetSelection(0);
				if (config->get("locale", "enable") == "1") load_locale_chb->SetValue(true);
				else load_locale_chb->SetValue(false);
			
				file_lb->Enable(load_locale_chb->GetValue());
				file_ed->Enable(load_locale_chb->GetValue());
				select_file_btn->Enable(load_locale_chb->GetValue());
				locales_lb->Enable(load_locale_chb->GetValue());
				locale_cb->Enable(load_locale_chb->GetValue());
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.user_message()));
			}
		}
	}
}

void config_f::terminate_app_btnClick( wxCommandEvent& event )
{
	// TODO: Implement terminate_app_btnClick
	//wxMessageBox(_("wxExit"));
	EndModal(wxID_ABORT);
}

void config_f::cc_processing_rgClick( wxCommandEvent& event )
{
	// TODO: Implement cc_processing_chbCheck
	license_ed->Enable(cc_heartland_rb->GetValue());
	site_ed->Enable(cc_heartland_rb->GetValue());
	device_ed->Enable(cc_heartland_rb->GetValue());
	user_ed->Enable(cc_heartland_rb->GetValue());
	password_ed->Enable(cc_heartland_rb->GetValue());
	test_credentials_btn->Enable(cc_heartland_rb->GetValue());
	max_tip_val_lb->Enable(cc_heartland_rb->GetValue());
	max_tips_value_cb->Enable(cc_heartland_rb->GetValue());
}

void config_f::auto_resolution_chbCheck( wxCommandEvent& event ) {
	resolution_width_cb->Enable(!auto_resolution_chb->GetValue());
	resolution_height_cb->Enable(!auto_resolution_chb->GetValue());
	
	
	if (auto_resolution_chb->GetValue()) 
		if (config->get("resolution", "auto") == "0" || to_uc(config->get("resolution", "auto")).MakeUpper() == _("OFF")) 
			wxMessageBox(locale->get("config_f", "need_restart_str", "Changes will be applied after application restart"));
}

void config_f::stay_on_top_chbCheck( wxCommandEvent& event ) {
	if (config->get("resolution", "stay_on_top") == "0" || to_uc(config->get("resolution", "stay_on_top")).MakeUpper() == _("OFF")) {
		if (stay_on_top_chb->GetValue())
			wxMessageBox(locale->get("config_f", "need_restart_str", "Changes will be applied after application restart"));
	}
	else {
		if (!stay_on_top_chb->GetValue())
			wxMessageBox(locale->get("config_f", "need_restart_str", "Changes will be applied after application restart"));
	}
}

void config_f::EditFocus( wxFocusEvent& event )
{
	// TODO: Implement EditFocus
}


void config_f::test_credentials_btnClick( wxCommandEvent& event )
{
	// TODO: Implement test_credentials_btnClick
	try {
		*flog << "test credentias";
		int license_id = boost::lexical_cast<int>(to_mb(license_ed->GetValue()));
		int site_id = boost::lexical_cast<int>(to_mb(site_ed->GetValue()));
		int device_id = boost::lexical_cast<int>(to_mb(device_ed->GetValue()));
		std::string user = to_mb(user_ed->GetValue());
		std::string password = to_mb(password_ed->GetValue());
		std::auto_ptr<ksi_client::user_pos_i> client = ksi_client::create_user_pos(license_id, site_id, device_id, user, password);
		*flog << "ksi_client::user_pos_i::test_credentials()";
		/**
		client->test_credentials();
		**/
		ksi_client::pos_exception* except = 0;
		wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), _("Communicating with processing server, please wait.")));				
		boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::test_credentials, client.get());
		int txn_id = 0;
		fin_form->start(boost::bind(cc_processing, &txn_id, &except, f));
		fin_form->ShowModal();
		if (except != 0) throw *except;
		*flog << "Test credentials passed";
		wxMessageBox(_("Test credentials passed"));
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		wxMessageBox(_("wrong settings"));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		wxMessageBox(_("wrong settings"));
	}

}

void config_f::test_printer_btnClick( wxCommandEvent& event )
{
	// TODO: Implement test_printer_btnClick
	*flog << "test printer";
	std::auto_ptr<com_t> com;
	std::auto_ptr<base_printer_t> printer;
	std::auto_ptr<fiscal::registrator_t> registrar;
	if (net_printer_chb->GetValue()) {
		printer.reset(new net_printer_t(to_mb(printer_host_ed->GetValue()), 703));
	}
	else if (local_printer_chb->GetValue()) {
		int rate = 9600;
		try {
			rate = boost::lexical_cast<int>(to_mb(baud_rate_cb->GetStringSelection()));
		}
		catch (std::bad_cast) {
			*flog << "bad_cast";
		}
		try {
			*flog << "create com_t";
			int flag = 0;
			if (command_set_cb->GetStringSelection().MakeUpper() == _("FISCAL REGISTRAR")) flag = 3;
			com.reset(new com_t(to_mb(printer_port_cb->GetStringSelection()), rate, flag));
			*flog << "successful";
		}
		catch (exception_t& ex) {
			*flog << ex.what();
			wxMessageBox(to_uc(ex.what()));
		}
		if (!com.get()) return;

		if (command_set_cb->GetStringSelection().MakeUpper() == _("STAR")) {
			*flog << "create star_printer_t";
			printer.reset(new star_printer_t(*com.get()));
		}
		else if (command_set_cb->GetStringSelection().MakeUpper() == _("EPSON")) {
			*flog << "create epson_printer_t";
			printer.reset(new epson_printer_t(*com.get()));
		}
		else if (command_set_cb->GetStringSelection().MakeUpper() == _("FISCAL REGISTRAR")) {
			*flog << "create fiscal_registrator_t";
			try {
				registrar.reset(new fiscal::registrator_t(*com.get()));
			}
			catch (fiscal::exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
				return;
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.user_message()));
				return;
			}
		}
		*flog << "successful";
	}
	else if (no_printer_chb->GetValue()) {
		printer.reset(new null_printer_t());
	}
	try {
		*flog << "print_xml = <test/>";
		if (printer.get()) print_xml(printer.get(), "<test/>");
		*flog << "successful";
	}
	catch (exception_t& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.user_message()));
	}
	registrar.reset();
	printer.reset();
	com.reset();
}

void config_f::cancel_btnClick( wxCommandEvent& event ) {
	EndModal(wxID_CANCEL);
}

void config_f::close_btnClick( wxCommandEvent& event )
{
	// TODO: Implement close_btnClick
	
	if (auth_reciept_duplicate_chb->GetValue())
		config->set("security", "print_receipt_duplicate", "ON");
	else config->set("security", "print_receipt_duplicate", "OFF");
	if (auth_refund_chb->GetValue())
		config->set("security", "refund_order", "ON");
	else config->set("security", "refund_order", "OFF");
	if (auth_app_settings_chb->GetValue())
		config->set("security", "access_to_settings", "ON");
	else config->set("security", "access_to_settings", "OFF");
	if (auth_misc_charges_chb->GetValue())
		config->set("security", "create_misc_charges", "ON");
	else config->set("security", "create_misc_charges", "OFF");
	if (auth_reload_all_menus_chb->GetValue())
		config->set("security", "reload_all_menus", "ON");
	else config->set("security", "reload_all_menus", "OFF");
	if (auth_shift_chb->GetValue())
		config->set("security", "seances", "ON");
	else config->set("security", "seances", "OFF");
	if (auth_table_booking_chb->GetValue())
		config->set("security", "create_table_booking", "ON");
	else config->set("security", "create_table_booking", "OFF");
	
	if (trainig_mode_chb->GetValue()) config->set("null", "trainig_mode", "ON");
	else config->set("null", "trainig_mode", "OFF");
	
	if (net_printer_chb->GetValue()) {
		config->set("printer", "source", "REMOTE");
		config->set("printer", "host", to_mb(printer_host_ed->GetValue()));
	}
	else if (local_printer_chb->GetValue()) {
		config->set("printer", "source", "LOCAL");
		config->set("printer", "port", to_mb(printer_port_cb->GetStringSelection()));
		config->set("printer", "command_set", to_mb(command_set_cb->GetStringSelection()));
		config->set("printer", "baud_rate", to_mb(baud_rate_cb->GetStringSelection()));
	}
	else if (no_printer_chb->GetValue()) {
		config->set("printer", "source", "NO");
	}
	
	if (group_by_chb->GetValue()) config->set("reciept", "group_by_kitchen_id", "ON");
	else config->set("reciept", "group_by_kitchen_id", "OFF");
	
	if (print_logo_chb->GetValue()) config->set("printer", "logo", "ON");
	else config->set("printer", "logo", "OFF");
		
	if (cc_heartland_rb->GetValue()) {
		config->set("Credit_Card_SubSystem", "processing", "HEARTLAND");
		config->set("Credit_Card_SubSystem", "license_id", to_mb(license_ed->GetValue()));
		config->set("Credit_Card_SubSystem", "site_id", to_mb(site_ed->GetValue()));
		config->set("Credit_Card_SubSystem", "device_id", to_mb(device_ed->GetValue()));
		config->set("Credit_Card_SubSystem", "user", to_mb(user_ed->GetValue()));
		config->set("Credit_Card_SubSystem", "password", to_mb(password_ed->GetValue()));	
		
		config->set("Credit_Card_SubSystem", "max_tips_val", to_mb(max_tips_value_cb->GetStringSelection()));	
	}
	else if (cc_stand_alone_rb->GetValue()) {
		config->set("Credit_Card_SubSystem", "processing", "STAND_ALONE");
	}
	else {
		config->set("Credit_Card_SubSystem", "processing", "OFF");
	}
/*	
	if (cc_processing_chb->GetValue()) {
		config->set("Credit_Card_SubSystem", "processing", "ON");
		config->set("Credit_Card_SubSystem", "license_id", to_mb(license_ed->GetValue()));
		config->set("Credit_Card_SubSystem", "site_id", to_mb(site_ed->GetValue()));
		config->set("Credit_Card_SubSystem", "device_id", to_mb(device_ed->GetValue()));
		config->set("Credit_Card_SubSystem", "user", to_mb(user_ed->GetValue()));
		config->set("Credit_Card_SubSystem", "password", to_mb(password_ed->GetValue()));
	}
	else config->set("Credit_Card_SubSystem", "processing", "OFF");
	config->set("Credit_Card_SubSystem", "max_tips_val", to_mb(max_tips_value_cb->GetStringSelection()));
*/
	config->set("app", "lock_timeout", to_mb(lock_timeout_cb->GetStringSelection()));
	config->set("app", "check_timeout", to_mb(server_status_check_cb->GetStringSelection()));
	config->set("app", "frac", to_mb(frac_cb->GetStringSelection()));
	
	if (auto_lock_chb->GetValue()) config->set("app", "auto_lock", "ON");
	else config->set("app", "auto_lock", "OFF");
	
	if (vfd_chb->GetValue()) config->set("vfd_display", "enable", "ON");
	else config->set("vfd_display", "enable", "OFF");

	config->set("vfd_display", "line1_text", to_mb(fline_ed->GetValue()));
	config->set("vfd_display", "line2_text", to_mb(sline_ed->GetValue()));
	
	if (fline_choice->GetSelection() == 0) config->set("vfd_display", "line1_align", "LEFT");
	else if (fline_choice->GetSelection() == 1) config->set("vfd_display", "line1_align", "CENTER");
	else if (fline_choice->GetSelection() == 2) config->set("vfd_display", "line1_align", "RIGHT");

	if (sline_choice->GetSelection() == 0) config->set("vfd_display", "line2_align", "LEFT");
	else if (sline_choice->GetSelection() == 1) config->set("vfd_display", "line2_align", "CENTER");
	else if (sline_choice->GetSelection() == 2) config->set("vfd_display", "line2_align", "RIGHT");	
	
	if (stay_on_top_chb->GetValue()) config->set("resolution", "stay_on_top", "ON");
	else config->set("resolution", "stay_on_top", "OFF");
	
	if (auto_resolution_chb->GetValue()) config->set("resolution", "auto", "ON");
	else {
		config->set("resolution", "auto", "OFF");
		config->set("resolution", "width", to_mb(resolution_width_cb->GetStringSelection()));
		config->set("resolution", "height", to_mb(resolution_height_cb->GetStringSelection()));
	}
	
	{
		config->set("locale", "file", to_mb(file_ed->GetLabel()));
		config->set("locale", "name", to_mb(locale_cb->GetLabel()));
		config->set("locale", "enable", load_locale_chb->GetValue() ? "1" : "0");
	}

	if (for_here_chb->GetValue()) config->set("functionality", "order_type_for_here", "ON");
	else config->set("functionality", "order_type_for_here", "OFF");
	if (to_go_chb->GetValue()) config->set("functionality", "order_type_to_go", "ON");
	else config->set("functionality", "order_type_to_go", "OFF");
	if (delivery_chb->GetValue()) config->set("functionality", "order_type_delivery", "ON");
	else config->set("functionality", "order_type_delivery", "OFF");
	if (pickup_chb->GetValue()) config->set("functionality", "order_type_pickup", "ON");
	else config->set("functionality", "order_type_pickup", "OFF");
	if (employee_meal_chb->GetValue()) config->set("functionality", "order_type_employee_meal", "ON");
	else config->set("functionality", "order_type_employee_meal", "OFF");
	if (donation_chb->GetValue()) config->set("functionality", "order_type_donation", "ON");
	else config->set("functionality", "order_type_donation", "OFF");
	if (table_booking_chb->GetValue()) config->set("functionality", "order_type_table_booking", "ON");
	else config->set("functionality", "order_type_table_booking", "OFF");
	
	if (entree_codes_chb->GetValue()) config->set("functionality", "entree_codes", "ON");
	else config->set("functionality", "entree_codes", "OFF");
	if (misc_charges_chb->GetValue()) config->set("functionality", "misc_charges", "ON");
	else config->set("functionality", "misc_charges", "OFF");
	if (tokens_chb->GetValue()) config->set("functionality", "tokens", "ON");
	else config->set("functionality", "tokens", "OFF");
	if (cash_input_chb->GetValue()) config->set("functionality", "cash_input", "ON");
	else config->set("functionality", "cash_input", "OFF");
	if (comment_entree_chb->GetValue()) config->set("functionality", "comment_entree", "ON");
	else config->set("functionality", "comment_entree", "OFF");
	if (cash_advance_chb->GetValue()) config->set("functionality", "cash_advance", "ON");
	else config->set("functionality", "cash_advance", "OFF");
	if (discount_chb->GetValue()) config->set("functionality", "discount", "ON");
	else config->set("functionality", "discount", "OFF");
	if (us_mode_chb->GetValue()) config->set("functionality", "us_mode", "ON");
	else config->set("functionality", "us_mode", "OFF");
	if (allow_remove_entrees_chb->GetValue()) config->set("functionality", "allow_remove_kitchen_entrees", "ON");
	else config->set("functionality", "allow_remove_kitchen_entrees", "OFF");
	
	if (close_order_operation_auto_close_chb->GetValue()) config->set("functionality", "close_order_operation_f_auto_close", "ON");
	else config->set("functionality", "close_order_operation_f_auto_close", "OFF");

	EndModal(wxID_OK);
}

void config_f::local_printer_chbCheck( wxCommandEvent& event ) {
	printer_port_cb->Enable(true);
	command_set_cb->Enable(true);
	baud_rate_cb->Enable(true);
	printer_host_ed->Enable(false);
}

void config_f::net_printer_chbCheck( wxCommandEvent& event ) {
	printer_port_cb->Enable(false);
	command_set_cb->Enable(false);
	baud_rate_cb->Enable(false);
	printer_host_ed->Enable(true);
}

void config_f::no_printer_chbCheck( wxCommandEvent& event ) {
	printer_port_cb->Enable(false);
	command_set_cb->Enable(false);
	baud_rate_cb->Enable(false);
	printer_host_ed->Enable(false);
}

void config_f::license_edClick( wxMouseEvent& event ) {
	wx_window_ptr<text_f> form (new text_f(this, _("License"), _(""), 0));
	form->text(license_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	license_ed->SetValue(form->text());
}

void config_f::site_edClick( wxMouseEvent& event ) {
	wx_window_ptr<text_f> form (new text_f(this, _("Site"), _(""), 0));
	form->text(site_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	site_ed->SetValue(form->text());
}

void config_f::device_edClick( wxMouseEvent& event ) {
	wx_window_ptr<text_f> form (new text_f(this, _("Device"), _(""), 0));
	form->text(device_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	device_ed->SetValue(form->text());
}

void config_f::user_edClick( wxMouseEvent& event ) {
	wx_window_ptr<text_f> form (new text_f(this, _("User"), _(""), 0));
	form->text(user_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	user_ed->SetValue(form->text());
}

void config_f::password_edClick( wxMouseEvent& event ) {
	wx_window_ptr<text_f> form (new text_f(this, _("Password"), _(""), 0));
	form->text(password_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	password_ed->SetValue(form->text());
}

void config_f::fline_choiceChange( wxCommandEvent& event ) {
	long lStyle = fline_choice->GetWindowStyle();
	lStyle &= ~( wxTE_LEFT | wxTE_CENTER | wxTE_RIGHT );
	
	if (fline_choice->GetSelection() == 0) { // Left
		
		fline_ed->SetWindowStyle(lStyle | wxTE_LEFT);
	}
	else if (fline_choice->GetSelection() == 1) { // Center
		fline_ed->SetWindowStyle(lStyle | wxTE_CENTRE);
	}
	else if (fline_choice->GetSelection() == 2) { // Right
		fline_ed->SetWindowStyle(lStyle | wxTE_RIGHT);
	}
	fline_ed->Refresh();
}

void config_f::sline_choiceChange( wxCommandEvent& event ) {
	long lStyle = sline_choice->GetWindowStyle();
	lStyle &= ~( wxTE_LEFT | wxTE_CENTER | wxTE_RIGHT );
	
	if (sline_choice->GetSelection() == 0) { // Left
		sline_ed->SetWindowStyle(lStyle | wxTE_LEFT);
	}
	else if (sline_choice->GetSelection() == 1) { // Center
		sline_ed->SetWindowStyle(lStyle | wxTE_CENTRE);
	}
	else if (sline_choice->GetSelection() == 2) { // Right
		sline_ed->SetWindowStyle(lStyle | wxTE_RIGHT);
	}
	sline_ed->Refresh();
}

void config_f::locale_cbClick( wxCommandEvent& event ) {
}

void config_f::select_file_btnClick( wxCommandEvent& event ) {
	wxString filename = wxFileSelector(_("Choose a file to open"), _(""), _(""), _("xml"));
	if ( !filename.empty() )
	{
		// work with the file
		try {
			std::vector<std::string> locales = locale->get_locales(to_mb(filename));
			locale_cb->Clear();
			for (size_t i=0; i<locales.size(); ++i) {
				locale_cb->Append(to_uc(locales[i]));
			}
			locale_cb->SetSelection(0);
			file_ed->SetLabel(filename);
		}
		catch (exception_t& ex) {
			*flog << ex.what();
			wxMessageBox(to_uc(ex.user_message()));
		}
	}
}

void config_f::load_locale_chbCheck( wxCommandEvent& event ) {
	file_lb->Enable(load_locale_chb->GetValue());
	file_ed->Enable(load_locale_chb->GetValue());
	select_file_btn->Enable(load_locale_chb->GetValue());
	locales_lb->Enable(load_locale_chb->GetValue());
	locale_cb->Enable(load_locale_chb->GetValue());
}

void config_f::order_typeCheck( wxCommandEvent& event ) {
	wxCheckBox* chb = dynamic_cast<wxCheckBox*>(event.GetEventObject());
	if (chb == 0) return;
	std::vector<wxCheckBox*> vec;
	vec.push_back(for_here_chb);
	vec.push_back(to_go_chb);
	vec.push_back(delivery_chb);
	vec.push_back(pickup_chb);
	vec.push_back(employee_meal_chb);
	vec.push_back(donation_chb);
	vec.push_back(table_booking_chb);
	size_t checked_count = std::count_if(vec.begin(), vec.end(), boost::bind(&wxCheckBox::GetValue, _1));
	if (checked_count == 0) {
		chb->SetValue(true);
		wxMessageBox(_("At least one order type must be enabled"));
	}
	//wxMessageBox(to_uc(boost::lexical_cast<std::string>(chb->GetValue())) + _(", checked_count = ") + to_uc(boost::lexical_cast<std::string>(checked_count)));
}
