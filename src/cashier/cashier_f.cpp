#include "cashier_f.h"
#include <wx/app.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <ksi_include/algo_functor.hpp>
#include <iterator>
#include <boost/bind.hpp>
#include <ksi_order/menu_container.hpp>
#include <ksi_util/users.hpp>
#include <ksi_order/cafe_container.hpp>
#include <ksi_order/coupon.hpp>
#include <ksi_order/order_interface.hpp>
#include <wx_util/move_up.hpp>
#include <oracle_client/ksi_exception.hpp>
#include <ksi_util/application_register.hpp>
#include <shared_modules/add_modifiers_f.h>
#include <ksi_include/custom_treelistctrl.hpp>
#include <wx_util/order_show.hpp>
#include <shared_modules/cashier_ico.h>
#include <wx/datetime.h>
#include <ksi_include/wx_tree_node.hpp>
#include <shared_modules/btn_list_f.h>
#include <shared_modules/coupon_list_f.h>
#include <shared_modules/wx_authorise.hpp>
#include <shared_modules/payment_f.h>
#include <shared_modules/calc_f.h>
#include <shared_modules/text_f.h>
#include <ksi_libpos/pos.hpp>
#include <shared_modules/card_swipe_f.h>
#include <ksi_util/log.hpp>
#include <boost/lexical_cast.hpp>
#include <ksi_order/order_payment.hpp>
#include <ksi_util/util.hpp>
#include <shared_modules/manual_entry_f.h>
#include <shared_modules/tip_amount_f.h>
#include <shared_modules/close_order_manager_f.h>
#include <serial/com.hpp>
#include <printer/printer.hpp>
#include <wx_util/wx_util.hpp>
#include <wx_util/xml_config.hpp>
#include <shared_modules/config_f.h>
#include <shared_modules/receipt.hpp>
#include <shared_modules/input_num_f.h>
#include <ksi_util/cash_advance.hpp>
#include <shared_modules/advance_f.h>
#include <ksi_util/inet.hpp>
#include <ksi_util/currency.hpp>
#include <shared_modules/delayed_order_f.h>
#include <shared_modules/load_f.h>
#include <ksi_util/loader.hpp>
#include <shared_modules/info_f.h>
#include <drawer/drawer.hpp>
#include <vfd_display/customer_display.hpp>
#include <shared_modules/fin_load_f.h>
#include <ksi_util/bag_spooler.hpp>
#include <lang/xml_lang.h>
#include <shared_modules/dlg_f.h>
#include <fiscal_register/fiscal.hpp>

extern std::string app_name;
extern int app_id;
extern std::string server_host;

namespace version {
	extern std::string version;
	extern std::string compamny;
	extern std::string product;
	extern std::string copyright;
};

std::auto_ptr<ksi_cafe::expense_list_t> expense_list;
std::auto_ptr<ksi_cafe::users_t> users;
std::auto_ptr<ksi_cafe::cafe_container_t> cafe_container;
std::auto_ptr<ksi_cafe::menu_container_t> menu_container;
std::auto_ptr<ksi_cafe::coupon_storage_t> coupon_storage;
std::auto_ptr<ksi_cafe::coupon_in_cafe_t> coupon_in_cafe;
std::auto_ptr<ksi_cafe::menu_manager_t> menu_manager;
std::auto_ptr<ksi_cafe::order_t> order;
std::auto_ptr<move_up_t> move_up;
std::auto_ptr<ksi_cafe::simple_stop_list_t> stop_list;
//std::auto_ptr<ksi_cafe::base_currency_t> currency;
std::auto_ptr<ksi_client::user_pos_i> client;
extern std::auto_ptr<file_log_t> flog;
std::auto_ptr<file_log_t> thflog;
std::auto_ptr<com_t> com;
std::auto_ptr<base_printer_t> printer;
extern std::auto_ptr<xml_config_t> config;
//std::auto_ptr<ksi_cafe::format_date_time_t> format_date_time;
std::auto_ptr<drawer_t> drawer;
std::auto_ptr<customer_display_t> display;
std::auto_ptr<fiscal::registrator_t> registrar;
std::auto_ptr<ksi_cafe::table_container_t> table_container;


extern std::auto_ptr<bag_spooler_t> bag_spooler;
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;
extern std::auto_ptr<ksi_cafe::xml_lang_t> shared_locale;

bool hide_mode_ = true;
bool by_group = false;
bool stand_alone = false;
int lang = 1;

std::string vfd_line1="";
std::string vfd_line2="";
int vfd_align_line1 = 1;
int vfd_align_line2 = 1;


wxString wxOrderTreeListCtrl::OnGetItemText (wxTreeItemData* item_data, long column) const {
	return sg_get_text(item_data, column);
}

void cashier_f::btn_repaint() {	
	const int hot_col_size = 8;
	const int hot_row_size = 5;
	const int btn_width = 68;
	const int btn_height = 68;
	const int btn_space = 0;
	
	int client_height, client_width;
	btn_pnl->GetClientSize( &client_width, &client_height );
	
	bool show_hot_ = true;
	if ( (client_width + btn_space*(hot_col_size-1))/hot_col_size < btn_width || (client_height/(hot_row_size+1) + btn_space*(hot_row_size)) < btn_height) {
		show_hot_ = false;
	}
	
	if (menu_select_btn) {
		menu_select_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_select_btnClick ), NULL, this );
		menu_select_btn->Hide();
		menu_select_btn = 0;
	}
	
	menu_select_btn = new wxButton( btn_pnl, wxID_ANY, locale->get("cashier_f", "menu_select_btn", "Menu select"), wxPoint( 0,0 ), wxSize( btn_width, btn_height ), 0 );
	wrap_label(menu_select_btn);
	menu_select_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	menu_select_btn->SetBackgroundColour( btn_face_color );
	menu_select_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_select_btnClick ), NULL, this );
	wxString title = to_uc(menu_manager->selected()->name());
	menu_select_btn->SetLabel(title);
	wrap_label(menu_select_btn);
	if (!menu_manager->selected()->loaded()) menu_manager->selected()->refresh();

	for (size_t i=0; i<menu_tree_vec.size(); ++i) {
		menu_tree_vec[i]->Hide();
		menu_tree_vec[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_tree_btnClick ), NULL, this );
	}
	menu_tree_vec.clear();
	for (size_t i=0; i<menu_item_vec.size(); ++i) {
		menu_item_vec[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_item_btnClick ), NULL, this );
		menu_item_vec[i]->Hide();
	}
	menu_item_vec.clear();
	for (size_t i=0; i<menu_hot_vec.size(); ++i) {
		menu_hot_vec[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_item_btnClick ), NULL, this );
		menu_hot_vec[i]->Hide();
	}
	menu_hot_vec.clear();
	
	for (;;) {
		int cnt = menu_tree_vec.size() + 1;
		if ( client_width < ( cnt )*( btn_width + btn_space ) + btn_width ) break;
		wxButton* btn;
		btn = new wxButton( btn_pnl, wxID_ANY, _(""), wxPoint( ( cnt )*( btn_width + btn_space ) , 0 ), wxSize( btn_width, btn_height ), 0 );
		btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
		btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
		btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_tree_btnClick ), NULL, this );
		menu_tree_vec.push_back(btn);
	}
	
	if (show_hot_) {
		for (size_t j=0; j<5; ++j){
			for (size_t i=0; i<8; ++i) {
				wxButton* btn;
				btn = new wxButton(btn_pnl, wxID_ANY, _(""), wxPoint(i*(btn_width+btn_space), client_height - (hot_row_size-j)*btn_height - (hot_row_size-1-j)*btn_space), wxSize( btn_width, btn_height ), 0);
				btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
				btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
				btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_item_btnClick ), NULL, this );
				menu_hot_vec.push_back(btn);
			}
		}
	}
	
	int h_pos = btn_height + btn_space + 10;
	for (;;) {
		int cnt = 0;
		if (show_hot_ && h_pos + btn_height > client_height - hot_row_size*btn_height - (hot_row_size-1)*btn_space) break;
		if (!show_hot_ && h_pos + btn_height > client_height) break;
		for (;;) {
			if (client_width <  cnt * ( btn_width + btn_space ) + btn_width) break;
			wxButton* btn;
			btn = new wxButton( btn_pnl, wxID_ANY, _(""), wxPoint(cnt*(btn_width+btn_space), h_pos), wxSize( btn_width, btn_height ), 0);
			btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
			btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
			btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_item_btnClick ), NULL, this );
			menu_item_vec.push_back(btn);
			++cnt;
		}
		h_pos += btn_height + btn_space;
		if (show_hot_ && h_pos + btn_height > client_height - hot_row_size*btn_height - (hot_row_size-1)*btn_space) break;
		if (!show_hot_ && h_pos + btn_height > client_height) break;
	}
	
	if (config->get("functionality", "order_type_for_here") == "0" || to_uc(config->get("functionality", "order_type_for_here")).MakeUpper() == _("OFF")) {
		for_here_btn->Show(false);
		for_here_type_ = false;
	}
	else {
		for_here_btn->Show(true);
		for_here_type_ = true;
	}
	if (config->get("functionality", "order_type_to_go") == "0" || to_uc(config->get("functionality", "order_type_to_go")).MakeUpper() == _("OFF")) { 
		to_go_btn->Show(false);
		to_go_type_ = false;
	}
	else {
		to_go_btn->Show(true);
		to_go_type_ = true;
	}
	if (config->get("functionality", "order_type_delivery") == "0" || to_uc(config->get("functionality", "order_type_delivery")).MakeUpper() == _("OFF")) { 
		delivery_btn->Show(false);
		delivery_type_ = false;
	}
	else {
		delivery_btn->Show(true);
		delivery_type_ = true;
	}
	if (config->get("functionality", "order_type_pickup") == "0" || to_uc(config->get("functionality", "order_type_pickup")).MakeUpper() == _("OFF")) {
		pickup_btn->Show(false);
		pickup_type_ = false;
	}
	else {
		pickup_btn->Show(true);
		pickup_type_ = false;
	}
	if (config->get("functionality", "order_type_employee_meal") == "0" || to_uc(config->get("functionality", "order_type_employee_meal")).MakeUpper() == _("OFF")) {
		employee_meal_btn->Show(false);
		employee_meal_type_ = false;
	}
	else {
		employee_meal_btn->Show(true);
		employee_meal_type_ = true;
	}
	if (config->get("functionality", "order_type_donation") == "0" || to_uc(config->get("functionality", "order_type_donation")).MakeUpper() == _("OFF")) {
		donation_btn->Show(false);
		donation_type_ = false;
	}
	else {
		donation_btn->Show(true);
		donation_type_ = true;
	}
	
	if (config->get("functionality", "entree_codes") == "0" || to_uc(config->get("functionality", "entree_codes")).MakeUpper() == _("OFF")) {
		entree_code_btn->Show(false);
	}
	else {
		entree_code_btn->Show(true);
	}
	if (config->get("functionality", "misc_charges") == "0" || to_uc(config->get("functionality", "misc_charges")).MakeUpper() == _("OFF")) {
		misc_charges_btn->Show(false);
	}
	else {
		misc_charges_btn->Show(true);
	}
	if (config->get("functionality", "tokens") == "0" || to_uc(config->get("functionality", "tokens")).MakeUpper() == _("OFF")) {
		token_btn->Show(false);
	}
	else {
		token_btn->Show(true);
	}
	if (config->get("functionality", "comment_entree") == "0" || to_uc(config->get("functionality", "comment_entree")).MakeUpper() == _("OFF")) {
		comment_entree_btn->Show(false);
	}
	else {
		comment_entree_btn->Show(true);
	}
	if (config->get("functionality", "cash_advance") == "0" || to_uc(config->get("functionality", "cash_advance")).MakeUpper() == _("OFF")) {
		cash_advance_btn->Show(false);
	}
	else {
		cash_advance_btn->Show(true);
	}
	if (config->get("functionality", "discount") == "0" || to_uc(config->get("functionality", "discount")).MakeUpper() == _("OFF")) {
		discount_btn->Show(false);
	}
	else {
		discount_btn->Show(true);
	}
	
	if (config->get("ext", "mode") == "1") {
		shift_btn->Show(true);
	}
	else {
		shift_btn->Show(false);
	}
	
	if (delivery_type_ || pickup_type_) {
		browse_delayed_orders_btnShow(true);
	}
	else browse_delayed_orders_btnShow(false);
	
	move_up.reset(new move_up_t(menu_tree_vec, menu_item_vec, menu_hot_vec));
	move_up->show(menu_manager->selected());
	Centre(wxBOTH);
}

cashier_f::cashier_f( wxWindow* parent )
:
base_cashier_f( parent ), menu_select_btn(0)
, timer_(obj_handler_t<cashier_f>(this, &cashier_f::OnTimer))
, seance_timer_(obj_handler_t<cashier_f>(this, &cashier_f::OnSeanceTimer))
, lock_timer_(obj_handler_t<cashier_f>(this, &cashier_f::OnLockTimer))
{
	for (size_t i=0; i<100; ++i) {
		bag_spooler->send_bag(bag_record_t(456, 11, "asf", "asdfafda"));
	}

	delayed_order_count_ = 0;
	open_cash_advance_count_ = 0;
	
	ksi_cafe::pos_version = boost::lexical_cast<std::string>(app_id) + "-" + version::version;
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	plus_btn->SetBitmapLabel(CashierBitmapsFunc(2));
	plus_btn->SetBitmapDisabled(CashierBitmapsFunc(3));
	minus_btn->SetBitmapLabel(CashierBitmapsFunc(4));
	minus_btn->SetBitmapDisabled(CashierBitmapsFunc(5));
	delete_btn->SetBitmapLabel(CashierBitmapsFunc(6));
	delete_btn->SetBitmapDisabled(CashierBitmapsFunc(7));
	
	sg->AddRoot(_("root"));
	
	SetTitle(to_uc(app_name));
	if (config->get("null", "trainig_mode") == "1" || to_uc(config->get("null", "trainig_mode")).MakeUpper() == _("ON")) hide_mode_ = true;
	else hide_mode_ = false;
	
	if (config->get("resolution", "auto") == "1" || to_uc(config->get("resolution", "auto")).MakeUpper() == _("ON")) {
		wxScreenDC screen;
		SetSize(screen.GetSize().GetWidth(), screen.GetSize().GetHeight());
	}
	else {
		try {
			int width = boost::lexical_cast<int>(config->get("resolution", "width"));
			int height = boost::lexical_cast<int>(config->get("resolution", "height"));
			SetSize(width, height);
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			wxScreenDC screen;
			SetSize(screen.GetSize().GetWidth(), screen.GetSize().GetHeight());			
		}
	}
	
	if (config->get("app", "auto_lock") == "1" || to_uc(config->get("app", "auto_lock")).MakeUpper() == _("ON"))
		auto_lock_ = true;
	else auto_lock_ = false;
	
	
	Centre(wxBOTH);

	std::vector<wxButton*> order_type_group_vec;
	order_type_group_vec.push_back(for_here_btn);
	order_type_group_vec.push_back(to_go_btn);
	order_type_group_vec.push_back(delivery_btn);
	order_type_group_vec.push_back(pickup_btn);
	order_type_group_vec.push_back(employee_meal_btn);
	order_type_group_vec.push_back(donation_btn);

	order_type_toggle_group_ptr.reset
	(
	new toggle_group_t(for_here_btn, order_type_group_vec, wxSystemSettings::GetColour( wxSYS_COLOUR_BTNSHADOW ), wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ))
	);	
	

	*flog << "registrer";
	try_register(app_id);
	std::vector<std::string> mac_vec = get_mac();	
	bool registered = false;
	for (size_t i=0; i<mac_vec.size(); ++i) {
		try {
			*flog << "cashier register";
			*flog << "try mac: " + mac_vec[i];
			try_cashier_register(mac_vec[i]);
			*flog << "successful";
			registered = true;
		}
		catch (...) {
		}
	}
	if (!registered) {
		THROW_MESSAGE(0, to_mb(locale->get("cashier_f", "terminal_not_register_msg", "This terminal is not registered in the shift or shift is not running")), "cashier_f::cashier_f( wxWindow* )");
	}
	
	*flog << "create ksi_cafe::users_t";
	users.reset(new ksi_cafe::users_t());
	*flog << "ksi_cafe::menu_t::container_load()";
	ksi_cafe::menu_t::container_load();
	*flog << "create ksi_cafe::cafe_container_t";
	cafe_container.reset(new ksi_cafe::cafe_container_t());
	*flog << "create ksi_cafe::menu_container_t";
	
	try {
		drawer.reset(new drawer_t());
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
	}
	
	thflog.reset(new file_log_t("cashier_thread.log", 10));
	
	try {
		if (to_uc(config->get("printer", "source")).MakeUpper() == _("REMOTE")) {
			printer.reset(new net_printer_t(config->get("printer", "host"), 703));
		}
		else if (to_uc(config->get("printer", "source")).MakeUpper() == _("LOCAL")) {
			if (to_uc(config->get("printer", "command_set")).MakeUpper() == _("STAR")) {
				com.reset(new com_t(config->get("printer", "port"), boost::lexical_cast<int>(config->get("printer", "baud_rate"))));
				printer.reset(new star_printer_t(*com.get()));
			}
			else if (to_uc(config->get("printer", "command_set")).MakeUpper() == _("EPSON")) {
				com.reset(new com_t(config->get("printer", "port"), boost::lexical_cast<int>(config->get("printer", "baud_rate"))));
				printer.reset(new epson_printer_t(*com.get()));
			}
			else if (to_uc(config->get("printer", "command_set")).MakeUpper() == _("FISCAL REGISTRAR")) {
				com.reset(new com_t(config->get("printer", "port"), boost::lexical_cast<int>(config->get("printer", "baud_rate")), 3));
				registrar.reset(new fiscal::registrator_t(*com.get()));
			}
		}
		else {
			printer.reset(new null_printer_t());
		}
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		//bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (fiscal::exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << "can't open serial port";
		//bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, "can't open serial port"));
		wxMessageBox(_("can't open serial port"));
	}
	if (printer.get() == 0) printer.reset(new null_printer_t());
	if (config->get("reciept", "group_by_kitchen_id") == "1" || to_uc(config->get("reciept", "group_by_kitchen_id")).MakeUpper() == _("ON"))
		by_group = true;
	else
		by_group = false;
	

	menu_container.reset(new ksi_cafe::menu_container_t());
	*flog << "create ksi_cafe::coupon_storage_t";
	coupon_storage.reset(new ksi_cafe::coupon_storage_t());
	int curr_cafe_id = cafe_container->current_cafe()->cafe_id();
	*flog << "curr_cafe_id = " + boost::lexical_cast<std::string>(curr_cafe_id);
	std::vector<ksi_cafe::coupon_t*> coupon_for_cafe = coupon_storage->coupon_for_cafe(curr_cafe_id);
	*flog << "create ksi_cafe::coupon_in_cafe_t";
	coupon_in_cafe.reset(new ksi_cafe::coupon_in_cafe_t(coupon_for_cafe));
	std::vector<ksi_cafe::menu_t*> menu_vec = menu_container->menu_for_cafe(curr_cafe_id);
	menu_manager.reset(new ksi_cafe::menu_manager_t(menu_vec));
	menu_vec = menu_manager->menu_vec();
	*flog << "create ksi_cafe::order_t";
	order.reset(new ksi_cafe::order_t(*cafe_container->current_cafe(), 0));
	*flog << "create sucsessful";
	if (order->sales_tax() == 0 && order->gratuity_tax() == 0) {
		hide_sales_tax();
		hide_gratuity_tax();
		hide_subtotal();
	}
	else if (order->sales_tax() == 0) hide_sales_tax();
	else if (order->gratuity_tax() == 0) hide_gratuity_tax();
	
	order->set_hide(hide_mode_);
	
	if (menu_vec.begin() != menu_vec.end()) {
		menu_manager->select((*menu_vec.begin())->menu_id(), order->table().table_group_id());
	}
	btn_repaint();
	
	if (config->get("vfd_display", "enable") == "1" || to_uc(config->get("vfd_display", "enable")).MakeUpper() == _("ON")) {
		try {
			*flog << "create customer_display_t";
			display.reset(new customer_display_t("COM4"));
			*flog << "successful";
			vfd_line1 = config->get("vfd_display", "line1_text");
			vfd_line2 = config->get("vfd_display", "line2_text");
			
			if (to_uc(config->get("vfd_display", "line1_align")).MakeUpper() == _("LEFT")) {
				vfd_align_line1 = 0;
			}
			else if (to_uc(config->get("vfd_display", "line1_align")).MakeUpper() == _("RIGHT")) {
				vfd_align_line1 = 2;
			}
			else vfd_align_line1 = 1;
			
			if (to_uc(config->get("vfd_display", "line2_align")).MakeUpper() == _("LEFT")) {
				vfd_align_line2 = 0;
			}
			else if (to_uc(config->get("vfd_display", "line2_align")).MakeUpper() == _("RIGHT")) {
				vfd_align_line2 = 2;
			}
			else vfd_align_line2 = 1;
		}
		catch (...) {
			*flog << "Unable to initialize COM4 port. VFD display will be disabled";
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, "Unable to initialize COM4 port. VFD display will be disabled"));
			wxMessageBox(_("Unable to initialize COM4 port. VFD display will be disabled"));
		}
	}

	bool tested_client = false;
	try {
		if (to_uc(config->get("Credit_Card_SubSystem", "processing")).MakeUpper() == _("HEARTLAND")) {
			*flog << "create ksi_client::user_pos_i";
			int license_id = boost::lexical_cast<int>(config->get("Credit_Card_SubSystem", "license_id"));
			int site_id = boost::lexical_cast<int>(config->get("Credit_Card_SubSystem", "site_id"));
			int device_id = boost::lexical_cast<int>(config->get("Credit_Card_SubSystem", "device_id"));
			std::string user = config->get("Credit_Card_SubSystem", "user");
			std::string password = config->get("Credit_Card_SubSystem", "password");
			client = ksi_client::create_user_pos(license_id, site_id, device_id, user, password);
			client->test_credentials();
			*flog << "sucessful";
			tested_client = true;
		}
		else if (to_uc(config->get("Credit_Card_SubSystem", "processing")).MakeUpper() == _("STAND_ALONE")) {
			stand_alone = true;
		}
		else {
			stand_alone = false;
		}
	}
	catch (ksi_client::pos_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
	if (!tested_client) client.reset(0);
	
	try {
		expense_list.reset(new ksi_cafe::expense_list_t(*cafe_container->current_cafe(), 0));
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
	
	if (expense_list.get() && expense_list->open_expense_size() > 0) {
		change_bg_colour(cash_advance_btn, wxColour(255, 0, 0));
		open_cash_advance_count_ = expense_list->open_expense_size();
		cash_advance_btn->SetLabel(locale->get("cashier_f", "cash_advance_btn1", "Cash advance") + _(" (") + to_uc(boost::lexical_cast<std::string>(open_cash_advance_count_)) + _(")") + locale->get("cashier_f", "cash_advance_btn2", " open"));
		wrap_label(cash_advance_btn);
	}
	else {
		open_cash_advance_count_ = 0;
		change_bg_colour(cash_advance_btn, btn_face_color);
		cash_advance_btn->SetLabel(locale->get("cashier_f", "cash_advance_btn1", "Cash advance"));
		wrap_label(cash_advance_btn);
	}
  
	std::string terminal = "";
	try {
		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
		query->create_statement
		(
		"select terminal.name \
		from ksi.terminal \
		left join ksi.seance on seance.terminal_id = terminal.terminal_id \
		where seance.seance_id = ksi.util.get_cur_seance_id"
		);
		query->execute_statement();
		terminal = boost::get<std::string>(query->get(0));
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
	}
	std::string ver_str = version::version;
	status_lb->SetLabel(to_uc(version::copyright) + _(", ") + to_uc(version::product) + _(" ver. ") + to_uc(ver_str) + _(",  terminal: ") + to_uc(terminal));
	refresh_order_list();
	//currency.reset (new ksi_cafe::en_currency_t());
	delayed_order_details_btn->Show(false);
	save_and_hide_btn->Show(false);
	current_user_lb->SetLabel(to_uc(users->user().role_name()) + _(" ") + to_uc(users->user().fio()));
	//format_date_time.reset(new ksi_cafe::format_date_time_t("MM/dd/yyyy", "h:mm:ss"));
	apply_lang();
	// Connect Events
	timer_.start(1);
	//"never" "15 sec" "30 sec" "1 min" "5 min" "10 min"
	std::string lock_timeout_str = config->get("app", "lock_timeout");
	if (lock_timeout_str == "15 sec") lock_timer_.start(15);
	else if (lock_timeout_str == "30 sec") lock_timer_.start(30);
	else if (lock_timeout_str == "45 sec") lock_timer_.start(45);
	else if (lock_timeout_str == "1 min") lock_timer_.start(60);
	else if (lock_timeout_str == "5 min") lock_timer_.start(300);
	else if (lock_timeout_str == "10 min") lock_timer_.start(600);
	//"never" "5 sec" "10 sec" "15 sec" "30 sec" "60 sec"
	std::string check_timeout_str = config->get("app", "check_timeout");
	if (check_timeout_str == "5 sec") seance_timer_.start(5);
	else if (check_timeout_str == "10 sec") seance_timer_.start(10);
	else if (check_timeout_str == "15 min") seance_timer_.start(15);
	else if (check_timeout_str == "30 min") seance_timer_.start(30);
	else if (check_timeout_str == "60 min") seance_timer_.start(60);
	sg->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( cashier_f::sgSelChanged ), NULL, this );
	sg->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( cashier_f::sgSelChanging ), NULL, this );
	
	ctrl_btn_pnl->SetSizer( ctrl_sizer );
	ctrl_btn_pnl->Layout();
	ctrl_sizer->Fit( ctrl_btn_pnl );	
}

cashier_f::~cashier_f() {
	// Disconnect Events
	sg->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( cashier_f::sgSelChanged ), NULL, this );
	sg->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( cashier_f::sgSelChanging ), NULL, this );
	menu_select_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_select_btnClick ), NULL, this );

	for (size_t i=0; i<menu_tree_vec.size(); ++i) {
		menu_tree_vec[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_tree_btnClick ), NULL, this );
	}
	for (size_t i=0; i<menu_item_vec.size(); ++i) {
		menu_item_vec[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_item_btnClick ), NULL, this );
	}
	*flog << "close cashier";
}

void cashier_f::order_type_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	
	if (!ksi_cafe::session_exist()) { logout(); }
	// TODO: Implement order_type_btnClick
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	*flog << "order_type_btnClick";
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (btn == for_here_btn) {
		*flog << "ksi_cafe::order_t::set_type_for_here()";
		discount_btn->Enable(true);
		order->set_type_for_here(0);
		delayed_order_details_btn->Show(false);
		save_and_hide_btn->Show(false);
	}
	else if (btn == to_go_btn) {
		*flog << "ksi_cafe::order_t::set_type_to_go()";
		discount_btn->Enable(true);
		order->set_type_to_go();
		delayed_order_details_btn->Show(false);
		save_and_hide_btn->Show(false);
	}
	else if (btn == delivery_btn || btn == pickup_btn) {
		discount_btn->Enable(true);
		delayed_order_details_btn->Show(true);
		save_and_hide_btn->Show(true);
		save_and_hide_btn->Enable(order->type_delivery() || order->type_pickup());
	}
	else if (btn == employee_meal_btn) {
		*flog << "ksi_cafe::order_t::set_type_employee_meal()";
		int order_type_id = order->type_id();
		
		std::vector<wxString> cap_vec; 
		std::vector<wxColour> color_vec; 
		std::vector<bool> enabled_vec; 
		std::vector<bool> checked_vec; 
		std::vector<bool> wrap_vec;
		
		cap_vec.push_back(locale->get("global", "swipe_employee_card_str", "Swipe employee card")); 
		cap_vec.push_back(locale->get("global", "select_employee_from_list_str", "Select employee from list"));
		color_vec.push_back(btn_face_color); color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true); enabled_vec.push_back(true);
		checked_vec.push_back(false); checked_vec.push_back(false);
		wrap_vec.push_back(true), wrap_vec.push_back(true);
		wxString cap_emp = locale->get("global", "employee_selection_for_employee_mail_str", "Employee selection for employee meal");
		wxString footer_cap = locale->get("global", "employee_form_footer_str", "In order to change employee, press \"Employee meal\" button once again.");
		wx_window_ptr<btn_list_f> form (new btn_list_f(this, cap_emp, footer_cap,cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
		int result = form->ShowModal() - wxID_HIGHEST;
		if (result == 0) {
			try {
				int person_id = wx_person_auth(this);
				if (person_id == 0) return;
				
				*flog << "meal_person_id = " + boost::lexical_cast<std::string>(person_id);
				
				std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
				query->create_statement
				(
				"select count(*) as discounted \
				from ksi.orders \
				where meal_person_id = :person_id \
				and cafe_id = ksi.util.get_cur_cafe_id \
				and seance_id in (select seance_id from ksi.seance where shift_id = ksi.util.get_cur_shift_id) \
				and status_id not in (4,5)"
				);
				query->set("person_id", person_id);
				query->execute_statement();
				int discounted = static_cast<int>(boost::get<double>(query->get(0)));
				if (discounted != 0) {
					wxMessageBox(to_uc(users->user(person_id).fio()) + _(" Employee meal is not available"));
					return;
				}
				order->clear_employee();
				order->clear_coupon();
				*flog << "ksi_cafe::order_t::set_type_employee_meal";
				order->set_type_employee_meal(person_id, "");
				*flog << "successful";
			}
			catch (exception_t& ex) {
				*flog << ex.comp_message();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()));
				return;
			}
			catch (ksi_client::connection_problem_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
				logout();
				return;
			}
			catch (ksi_client::db_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
				return;
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
				return;
			}
			catch (...) {
				*flog << ERROR_IN_LOCATION;
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
				wxMessageBox(to_uc(ERROR_IN_LOCATION));
				return;
			}
		}
		else if (result == 1) {
			try {
				std::vector<wxString> cap_vec; std::vector<wxColour> color_vec; std::vector<bool> enabled_vec; std::vector<bool> checked_vec; std::vector<bool> wrap_vec;
				std::vector<ksi_cafe::user_t> user_vec = users->get_all();
				cap_vec.push_back(locale->get("global", "unregistered_user_str", "<Unregistered user>"));
				color_vec.push_back(btn_face_color);
				enabled_vec.push_back(true);
				checked_vec.push_back(false);
				for (size_t i=0; i<user_vec.size(); ++i) {
					cap_vec.push_back(to_uc(user_vec[i].fio()));
					color_vec.push_back(btn_face_color);
					enabled_vec.push_back(true);
					checked_vec.push_back(false);
					wrap_vec.push_back(true);
				}
				wx_window_ptr<btn_list_f> form (new btn_list_f(this, _(""), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
				int result = form->ShowModal() - wxID_HIGHEST;
				if (result == 0) {
					wx_window_ptr<text_f> form (new text_f(this, locale->get("global", "type_the_employee_name_str", "Type the employee name"), locale->get("global", "type_the_employee_name_str", "Type the employee name"), 0));
					if (form->ShowModal() != wxID_OK) return;
					order->clear_employee();
					order->clear_coupon();
					order->set_type_employee_meal(0, to_mb(form->text()));
				}
				else if (result > 0 && result < user_vec.size()) {
					order->clear_employee();
					order->clear_coupon();
					order->set_type_employee_meal(user_vec[result-1].user_id(), "");
				}
				else return;
			}
			catch (exception_t& ex) {
				*flog << ex.comp_message();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()));
				return;
			}
			catch (ksi_client::connection_problem_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
				logout();
				return;
			}
			catch (ksi_client::db_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
				return;
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
				return;
			}
			catch (...) {
				*flog << ERROR_IN_LOCATION;
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
				wxMessageBox(to_uc(ERROR_IN_LOCATION));
				return;
			}
		}
		else return;		
		
		discount_btn->Enable(false);
		delayed_order_details_btn->Show(false);
		save_and_hide_btn->Show(false);
	}
	order_type_toggle_group_ptr->toggle(event);
	recalc_coupon();
	ctrl_btn_pnl->SetSizer( ctrl_sizer );
	ctrl_btn_pnl->Layout();
	ctrl_sizer->Fit( ctrl_btn_pnl );
}

void cashier_f::menu_select_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	*flog << "menu_select_btnClick";
	try {
		std::vector<ksi_cafe::menu_t*> menu_vec = menu_manager->menu_vec();
		std::vector<wxString> cap_vec;
		std::vector<wxColour> color_vec;
		std::vector<bool> checked_vec;
		std::vector<bool> enabled_vec;
		std::vector<bool> wrap_vec;
		cap_vec.push_back(locale->get("global", "reload_all_menus_str", "reload all menus"));
		color_vec.push_back(ok_color);
		checked_vec.push_back(false);
		enabled_vec.push_back(true);
		wrap_vec.push_back(true);
		for (size_t i=0; i<menu_vec.size(); ++i) {
			cap_vec.push_back(to_uc(menu_vec[i]->name()));
			color_vec.push_back(btn_face_color);
			checked_vec.push_back(menu_manager->selected() == menu_vec[i]);
			enabled_vec.push_back(true);
			wrap_vec.push_back(true);
		}
		wx_window_ptr<btn_list_f> form (new btn_list_f(this, locale->get("global", "select_a_menu_str", "Select a menu"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
		int result = form->ShowModal() - wxID_HIGHEST - 1;
		if (result == -1) {
			if (config->get("security", "reload_all_menus") == "0" || to_uc(config->get("security", "reload_all_menus")).MakeUpper() == _("OFF")) {
			}
			else if (!wx_mng_auth(this)) return;
			bool successful = false;
			try {
				*flog << "ksi_cafe::menu_t::container_load()";
				ksi_cafe::menu_t::container_load();
				*flog << "create ksi_cafe::menu_container_t";
				std::auto_ptr<ksi_cafe::menu_container_t> new_menu_container;
				new_menu_container.reset(new ksi_cafe::menu_container_t());
				menu_container.reset(new_menu_container.release());
				std::vector<ksi_cafe::menu_t*> menu_vec = menu_container->menu_for_cafe(cafe_container->current_cafe()->cafe_id());
				menu_manager.reset(new ksi_cafe::menu_manager_t(menu_vec));
				menu_vec = menu_manager->menu_vec();
				if (menu_vec.begin() != menu_vec.end()) {
					menu_manager->select((*menu_vec.begin())->menu_id(), order->table().table_group_id());
				}
				btn_repaint();
				successful = true;
			}
			catch (exception_t& ex) {
				*flog << ex.comp_message();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()));
			}
			catch (ksi_client::connection_problem_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
				logout();
				return;
			}
			catch (ksi_client::db_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			catch (...) {
				*flog << ERROR_IN_LOCATION;
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
				wxMessageBox(to_uc(ERROR_IN_LOCATION));
			}
			if (!successful) return;
		}
		wx_disable_t disable_window(this);
		wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
		if (result >= 0 && result < menu_vec.size()) {
			ksi_cafe::menu_t* selected_menu = menu_vec[result];	
			*flog << std::string("selected memu: ") + boost::lexical_cast<std::string>(selected_menu->menu_id()) + std::string("/")+selected_menu->name();
			//ksi_cafe::menu_t* selected_menu = form->selected();
			wxString title = to_uc(selected_menu->name());
			menu_select_btn->SetLabel(title);
			wrap_label(menu_select_btn);
			if (!selected_menu->loaded())selected_menu->refresh();
			move_up->show(selected_menu);
			menu_manager->select(selected_menu->menu_id(), order->table().table_group_id());
		}
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
		return;
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}

void cashier_f::menu_tree_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	*flog << "menu_tree_btnClick";
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	try {
		if (btn) move_up->tree_click(btn);
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
		return;
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}

const ksi_cafe::order_dish_t& get_order_dish(wxOrderTreeListCtrl* sg) {
	*flog << "get_order_dish()";
	try {
		wxTreeItemId id = sg->GetSelection();
		if (id.IsOk()) {
			order_node_t* node = dynamic_cast<order_node_t*>(sg->GetItemData(id));
			if (node && node->type() == node_type::order_dish) {
				const ksi_cafe::order_dish_t* odish = reinterpret_cast<const ksi_cafe::order_dish_t*>(node->col1());
				return *odish;
			}
		}
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
		THROW_LOCATION(1, "const ksi_cafe::order_dish_t& get_order_dish()");
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		THROW_LOCATION(2, "const ksi_cafe::order_dish_t& get_order_dish()");
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
		THROW_LOCATION(3, "const ksi_cafe::order_dish_t& get_order_dish()");
	}
	THROW_LOCATION(0, "const ksi_cafe::order_dish_t& get_order_dish()");
}

void cashier_f::plus_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	*flog << "plus_btnClick";
	try {
		const ksi_cafe::order_dish_t& odish = get_order_dish(sg);
		std::string dish_type_check_sum = odish.type_check_sum();
		std::string dish_check_sum = odish.check_sum();
		double dish_price = odish.price();
		std::string dish_name = odish.name();
		order->add_dish(ksi_cafe::order_dish_t(odish, 1), false);
		*flog << "add_dish: " + dish_check_sum;
		recalc_coupon(dish_type_check_sum, dish_check_sum);
		vfd_clear_line(display.get(), 1);
		std::string vfd_prefix = "";
		if (order->order_dish_count(dish_check_sum) != 1) vfd_prefix = boost::lexical_cast<int>(order->order_dish_count(dish_check_sum)) + "x";
		vfd_print(display.get(), 1, vfd_prefix + dish_name, to_mb(locale->get_currency(order->order_dish_count(dish_check_sum)*dish_price)), 0);
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
		return;
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}	
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}

void cashier_f::minus_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	*flog << "minus_btnClick";
		
	try {
		const ksi_cafe::order_dish_t& odish = get_order_dish(sg);
		std::string dish_type_check_sum = odish.type_check_sum();
		std::string dish_check_sum = odish.check_sum();
		int cnt = odish.total_count();
		std::string dish_name = odish.name();
		double dish_price = odish.price();
		if (odish.new_count() > 0 || odish.dish_id() == 0) {
			order->remove_dish(odish);
		}
		else {
			wxString message = locale->get("global", "waste_entree_str", "waste entree products/ingrediends?");
			//wxMessageDialog dlg(this, message, _("Warrning"), wxYES_NO);
			//dlg.Centre(wxBOTH);
			//int res = dlg.ShowModal();
			std::vector<wxString> cap_vec;
			cap_vec.push_back(_("Yes")); cap_vec.push_back(_("No"));
			std::vector<wxColour> color_vec;
			color_vec.push_back(ok_color); color_vec.push_back(close_color);
			wx_window_ptr<dlg_f> form (new dlg_f(this, _("Confirmation"), message, cap_vec, color_vec));
			int res = form->ShowModal();
			if (res == wxID_HIGHEST) {
				order->remove_dish(odish, true);
				*flog << "minus_dish_wasted: " + dish_check_sum;
			}
			else if (res == wxID_HIGHEST+1) {
				order->remove_dish(odish, false);
				*flog << "minus_dish_nowasted: " + dish_check_sum;
			}
			else return;
			
			*flog << "ksi_cafe::order_t::set_accept";
			order->set_accept(0, true);
			*flog << "successful";
			int order_id = order->order_id();
			*flog << "ksi_cafe::order_t::commit";
			if (!order->commit(true)) *flog << "!!! force apply";
			*flog << "successful";
			sg_clear(sg);
			order.reset();
			*flog << "reloadOrder";
			ksi_cafe::order_t* ord_ptr = reloadOrder(this, order_id, *flog, *cafe_container);
			*flog << "reloadOrder.successful";
			order.reset(ord_ptr);
			if (order.get()) {
				order->clear_wasted();
				if (order->type_delivery()) order_type_toggle_group_ptr->toggle(delivery_btn);
				else if (order->type_pickup()) order_type_toggle_group_ptr->toggle(pickup_btn);
				delayed_order_details_btn->Show(true);
				save_and_hide_btn->Show(true);
				save_and_hide_btn->Enable(order->type_delivery() || order->type_pickup());
				recalc_coupon();			
			}
			else {
				resume_wait();
				return;
			}
		}
		vfd_clear_line(display.get(), 1);
		if (cnt > 1) {
			recalc_coupon(dish_type_check_sum, dish_check_sum);
			std::string vfd_prefix = "";
			if (order->order_dish_count(dish_check_sum) != 1) vfd_prefix = boost::lexical_cast<int>(order->order_dish_count(dish_check_sum)) + "x";
			vfd_print(display.get(), 1, vfd_prefix + dish_name, to_mb(locale->get_currency(order->order_dish_count(dish_check_sum)*dish_price)), 0);
		}
		else recalc_coupon("", "");
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
		return;
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}	
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}

void cashier_f::delete_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	*flog << "delete_btnClick";
	try {
		const ksi_cafe::order_dish_t& odish = get_order_dish(sg);
		std::string type_check_sum = odish.type_check_sum();
		std::string check_sum = odish.check_sum();
		int cnt = odish.total_count();
		int new_cnt = odish.new_count();
		if (odish.new_count() > 0) {
			order->remove_new_dish(odish);
			vfd_clear_line(display.get(), 1);
		}
		
		if (cnt > new_cnt) recalc_coupon(type_check_sum, check_sum);
		else recalc_coupon("", "");
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
		return;
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}	
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}

void cashier_f::menu_item_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	*flog << "menu_item_btnClick";
	try {
		if (move_up->get_menu_bar_status(btn) == btn_status::menu_item) move_up->menu_item_click(btn);
		else if (move_up->get_menu_bar_status(btn) == btn_status::parent) move_up->menu_item_click(btn);
		else if ((move_up->get_menu_bar_status(btn) == btn_status::menu_dish) || (move_up->get_hot_bar_status(btn) == btn_status::menu_dish)) {
			stop_list.reset(new ksi_cafe::simple_stop_list_t());
			    
			if (stop_list->find_entry(move_up->get_menu_dish(btn)->dish_id())) {
				const ksi_cafe::stop_list_entry_t& entry = stop_list->get_fined_entry(move_up->get_menu_dish(btn)->dish_id());
				wxString msg = locale->get("global", "entry_in_stop_list_since_str", "Entree in stop list since ") + locale->get_date_time(entry.since_dt()) + locale->get("global", "s_till_s_str", " till ") + locale->get_date_time(entry.till_dt());
				wxMessageBox(msg);
				return;
			}
			
			if (order->type_employee_meal() && !move_up->get_menu_dish(btn)->employee_meal()) {
				wxString message = _("This entree is not allowed to be ordered in employee meal. It can be added to order only with manager authorization.");
				std::vector<wxString> cap_vec; std::vector<wxColour> color_vec;
				cap_vec.push_back(_("Authorize")); cap_vec.push_back(_("Cancel"));
				color_vec.push_back(ok_color); color_vec.push_back(close_color);
				wx_window_ptr<dlg_f> form (new dlg_f(this, _("Confirmation"), message, cap_vec, color_vec));
				if (form->ShowModal() == wxID_HIGHEST && wx_mng_auth(this)) {
				}
				else return;
			}
			
			wx_window_ptr<add_modifiers_f> form (new add_modifiers_f(this, _("Select modifiers"), stop_list.get()));
			if (form->set_modifiers(menu_manager->selected(), move_up->get_menu_dish(btn))) {
				if (form->ShowModal() != wxID_OK) return;
			}
			std::vector<ksi_cafe::order_dish_modifier_t> mods = form->sel_mods();
			ksi_cafe::order_dish_t odish(*move_up->get_menu_dish(btn), mods);
			std::string dish_check_sum = odish.check_sum();
			std::string dish_name = odish.name();
			double dish_price = odish.price();
			order->add_dish(odish, true);
			*flog << "add_dish: " + odish.check_sum();
			recalc_coupon(odish.type_check_sum(), odish.check_sum());
			vfd_clear_line(display.get(), 1);
			std::string vfd_prefix = "";
			if (order->order_dish_count(dish_check_sum) != 1) vfd_prefix = boost::lexical_cast<int>(order->order_dish_count(dish_check_sum)) + "x";
			vfd_print(display.get(), 1, vfd_prefix + dish_name, to_mb(locale->get_currency(order->order_dish_count(dish_check_sum)*dish_price)), 0);
		}
		else if (move_up->get_menu_bar_status(btn) == btn_status::next) move_up->next_page_click(btn);
		else if (move_up->get_menu_bar_status(btn) == btn_status::prev) move_up->prev_page_click(btn);
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
		return;
	}	
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}


void cashier_f::process_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);	
	if (!ksi_cafe::session_exist()) { logout(); }
	*flog << "process_btnClick";
	if ( (!order->type_delivery() && order_type_toggle_group_ptr->current() == delivery_btn) || (!order->type_pickup() && order_type_toggle_group_ptr->current() == pickup_btn) ) {
		wxString msg = locale->get("global", "required_delayed_order_properties_str", "Required delayed order properties are not filled, order processing is not available.");
		wxMessageBox(msg);
		return;
	}
	if (order->real_dish_count() == 0 && order->order_id() == 0) {
		return;
	}
	else if (order->real_dish_count() == 0 && order->order_id() != 0) {
		try {
			order->set_accept(0, true);
			order->set_payment_cash(new ksi_cafe::cash_payment_t(0, 0));
			order->set_simply_close(true);
		}
		catch (ksi_client::connection_problem_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			logout();
			return;
		}
		catch (ksi_client::db_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
			return;
		}
		catch (exception_t& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()));
			return;
		}
		catch (std::exception& ex) {
			*flog << ERROR_IN_LOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			return;
		}
		*flog << "ksi_cafe::order_t::commit(), order_id = " + boost::lexical_cast<std::string>(order->order_id());
		if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
		*flog << "commit successful";
		resume_wait();
		return;
	}
	if (order->type_employee_meal() && order->real_dish_count() == 0) {
		return;
	}
	if (order->type_delivery()  && order->subtotal() < order->delivery_min_subtotal()) {
		wxString message = _("The order subtotal is less than required for delivery (") + locale->get_currency(order->delivery_min_subtotal()) + _("). Create order an anyway?");
		std::vector<wxString> cap_vec;
		cap_vec.push_back(_("Yes")); cap_vec.push_back(_("No"));
		std::vector<wxColour> color_vec;
		color_vec.push_back(ok_color); color_vec.push_back(close_color);
		wx_window_ptr<dlg_f> form (new dlg_f(this, _("Confirmation"), message, cap_vec, color_vec));
		if (form->ShowModal() == wxID_HIGHEST) {
		}
		else return;
		//wxMessageDialog dlg(this, message, _("Warrning"), wxYES_NO);
		//dlg.Centre(wxBOTH);
		//if (dlg.ShowModal() != wxID_YES) return;
	}
	if (order->type_employee_meal() && order->def_subtotal() > order->meal_free_limit()) {
		wxString message = _("Meal free limit has been exceeded on ") + locale->get_currency(order->def_subtotal()-order->meal_free_limit()) + wxT(". Proceed?");
		std::vector<wxString> cap_vec;
		cap_vec.push_back(_("Yes")); cap_vec.push_back(_("No"));
		std::vector<wxColour> color_vec;
		color_vec.push_back(ok_color); color_vec.push_back(close_color);
		wx_window_ptr<dlg_f> form (new dlg_f(this, _("Confirmation"), message, cap_vec, color_vec));
		if (form->ShowModal() == wxID_HIGHEST && wx_mng_auth(this)) {
		}
		else return;
		//wxMessageDialog dlg(this, message, _("Warrning"), wxYES_NO);
		//dlg.Center(wxBOTH);
		//if (dlg.ShowModal() == wxID_YES && wx_mng_auth()) {
		//}
		//else {
		//	return;
		//}
	}
	
	vfd_clear_all(display.get());
	vfd_print(display.get(), 1, "Subtotal", to_mb(locale->get_currency(order->subtotal())), 0);
	vfd_print(display.get(), 2, "Total ", to_mb(locale->get_currency(order->total())), 0);
	
	int payment_type_id = 0;
		
	wx_window_ptr<payment_f> payment_form (new payment_f(this, _("Payment"), order->type_employee_meal(), (stand_alone || client.get()) && order->subtotal() != 0));
	if (!order->type_employee_meal() && !((stand_alone || client.get()) && order->subtotal() != 0)) {
		payment_type_id = 1;
	}
	else {
		if (payment_form->ShowModal() != wxID_OK) return;
		payment_type_id = payment_form->result();
	}
	*flog << "payment, payment_type_id = " + boost::lexical_cast<std::string>(payment_type_id);
	double cash = 0;
	double change = 0;
	
	if (payment_type_id == 1) { // Cash
		wx_window_ptr<calc_f> form (new calc_f(this, locale->get("calc_f", "caption", "Cash payment"), order->total(), false));
		bool cash_input = true;
		if (config->get("functionality", "cash_input") == "0" || to_uc(config->get("functionality", "cash_input")).MakeUpper() == _("OFF")) cash_input = false;
		if (order->total() == 0) {
			cash_input = false;
			cash = 0;
			change = 0;
		}
		if (cash_input) if (form->ShowModal() != wxID_OK) return;
		cash = cash_input ? form->amount() : order->total();
		change = cash - order->total();
		*flog << "payment_cash, cash = " + to_mb(locale->get_currency(cash)) + " , change = " + to_mb(locale->get_currency(change));
		try {
			order->set_payment_cash(new ksi_cafe::cash_payment_t(cash, change));
			*flog << "ksi_cafe::order_t::set_accept";
			order->set_accept(0, true);
			*flog << "successful";
		}
		catch (ksi_client::connection_problem_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			logout();
			return;
		}
		catch (ksi_client::db_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
			return;
		}
		catch (exception_t& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()));
			return;
		}
		catch (std::exception& ex) {
			*flog << ERROR_IN_LOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			return;
		}
	}
	else if (payment_type_id == 2 && stand_alone) { // Credit card    
		try {
			order->set_vpayment_credit_card(new ksi_cafe::credit_vpayment_t(order->total()));
			*flog << "ksi_cafe::order_t::set_accept";
			order->set_accept(0, true);
			*flog << "successful";
		}
		catch (ksi_client::connection_problem_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			logout();
			return;
		}
		catch (ksi_client::db_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
			return;
		}
		catch (exception_t& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()));
			return;
		}
		catch (std::exception& ex) {
			*flog << ERROR_IN_LOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			return;
		}
	}
	else if (payment_type_id == 2 && !stand_alone) { // Credit card		
		int auth_txn_id = 0;
		ksi_cafe::credit_payment_t* payment = 0;
		double total = order->total() - order->prepayment_amount();
		if (!order->payment_credit_card()) {
			std::vector<wxString> cap_vec; std::vector<wxColour> color_vec; std::vector<bool> enabled_vec; std::vector<bool> checked_vec; std::vector<bool> wrap_vec;
			cap_vec.push_back(locale->get("global", "AUTHORIZE_CC_str", "AUTHORIZE CC")); 
			cap_vec.push_back(locale->get("global", "Manual_entry_str", "Manual Entry"));
			color_vec.push_back(btn_face_color); color_vec.push_back(btn_face_color);
			enabled_vec.push_back(true); enabled_vec.push_back(true);
			checked_vec.push_back(false); checked_vec.push_back(false);
			wrap_vec.push_back(true); wrap_vec.push_back(true);
			wx_window_ptr<btn_list_f> form (new btn_list_f(this, locale->get("global", "credit_card_payment_str", "Credit Card payment"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
			int result = form->ShowModal() - wxID_HIGHEST;
			std::string cc_buff_;
			bool swiped_ = true;

			std::string auth_error_msg;
			if (result == 0) { // Authorize cc 
				*flog << "payment_credit_card, authorize cc";
				wx_window_ptr<card_swipe_f> form (new card_swipe_f(this, _(""), locale->get("global", "AUTHORIZE_CC_str", "AUTHORIZE CC")));
				if (form->ShowModal() != wxID_OK) return;
				cc_buff_ = form->buff();
				*flog << "credit_auth, cc_buff_ = " + ksi_cafe::masked_buff(cc_buff_);//cc_buff_;
				try {
					/**
					auth_txn_id = client->credit_auth(total, 0, cc_buff_, true);
					**/
					bool cont;
					do {
						cont = false;
						ksi_client::pos_exception* except = 0;
						*flog << LLOCATION;
						wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), locale->get("global", "communication_with_processing_server_please_wait", "Communicating with processing server, please wait.")));
						*flog << LLOCATION;
						client->set_do_report(true);
						*flog << LLOCATION;
						boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_auth, client.get(), total, 0, cc_buff_);
						*flog << LLOCATION;
						fin_form->start(boost::bind(cc_processing, &auth_txn_id, &except, f));
						*flog << LLOCATION;
						fin_form->ShowModal();
						*flog << LLOCATION;
						try {
							std::auto_ptr<ksi_client::pos_exception> except_aptr;
							except_aptr.reset(except);
							if (except != 0) {
								if (typeid(*except) == typeid(ksi_client::fatal_pos_exception)) 
									throw dynamic_cast<ksi_client::fatal_pos_exception&>(*except);
								throw *except->clone();
							}
							*flog << "successful, auth_txn_id = " + boost::lexical_cast<std::string>(auth_txn_id);
						}
						catch (ksi_client::fatal_pos_exception& ex) {
							*flog << LLOCATION;
							*flog << boost::lexical_cast<std::string>(ex.msg_id()) + std::string(" ") + ex.what();
							auth_error_msg = ex.what();
							bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							//wxMessageBox(to_uc(boost::lexical_cast<std::string>(ex.msg_id())) + _(" ") + to_uc(ex.what()));
							
							std::vector<wxString> cap_vec;
							std::vector<wxColour> color_vec;
							std::vector<bool> enabled_vec;
							std::vector<bool> checked_vec;
							std::vector<bool> wrap_vec;
							
							cap_vec.push_back(_("Retry"));
							color_vec.push_back(ok_color);
							enabled_vec.push_back(true);
							checked_vec.push_back(false);
							wrap_vec.push_back(true);
							wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Communication ERROR."), _("There is an error communicating with CC processing center. Please retry or cancel the operation."), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
							int result = form->ShowModal() - wxID_HIGHEST;
							if (result == 0) {
								*flog << LLOCATION;
								cont = true;
							}
							else {
								*flog << LLOCATION;
								return;
							}
						}
						catch (...) {
							throw;
						}
					} while (cont);
				}
				catch (ksi_client::pos_exception& ex) {
					*flog << boost::lexical_cast<std::string>(ex.msg_id()) + std::string(" ") + ex.what();
					auth_error_msg = ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(boost::lexical_cast<std::string>(ex.msg_id())) + _(" ") + to_uc(ex.what()));
					return;
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
					return;
				}
				catch (...) {
					*flog << ERROR_IN_LOCATION;
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
					wxMessageBox(to_uc(ERROR_IN_LOCATION));
					return;
				}
				swiped_ = true;
			}
			else if (result == 1) { // manual Entry
				*flog << "payment_credit_card, manual entry";
				wx_window_ptr<manual_entry_f> form (new manual_entry_f(this, locale->get("global", "Manual_entry_str", "Manual Entry")));
				if (form->ShowModal() != wxID_OK) return;
				cc_buff_ = ";"+to_mb(form->card_num())+"=";
				*flog << "credit_auth, cc_buff_ = " + ksi_cafe::masked_buff(cc_buff_);//cc_buff_;
				int exp_month = boost::lexical_cast<int>(to_mb(form->exp_month()));
				int exp_year = boost::lexical_cast<int>(to_mb(form->exp_year()));
				try {
					/**
					auth_txn_id = client->credit_auth(total, 0, to_mb(form->card_num()), exp_month, exp_year, to_mb(form->cvv2()), true, true, true);
					*/
					bool cont;
					do {
						cont = false;
						ksi_client::pos_exception* except = 0;
						*flog << LLOCATION;
						wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), locale->get("global", "communication_with_processing_server_please_wait", "Communicating with processing server, please wait.")));
						*flog << LLOCATION;
						client->set_do_report(true);
						*flog << LLOCATION;
						boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_auth, client.get(), total, 0, to_mb(form->card_num()), exp_month, exp_year, to_mb(form->cvv2()), true, true);
						*flog << LLOCATION;
						fin_form->start(boost::bind(cc_processing, &auth_txn_id, &except, f));
						*flog << LLOCATION;
						fin_form->ShowModal();
						*flog << LLOCATION;
						try {
							std::auto_ptr<ksi_client::pos_exception> except_aptr;
							except_aptr.reset(except);
							if (except != 0) {
								if (typeid(*except) == typeid(ksi_client::fatal_pos_exception)) 
									throw dynamic_cast<ksi_client::fatal_pos_exception&>(*except);
								throw *except->clone();
							}
							*flog << "successful, auth_txn_id = " + boost::lexical_cast<std::string>(auth_txn_id);
						}
						catch (ksi_client::fatal_pos_exception& ex) {
							*flog << LLOCATION;
							*flog << boost::lexical_cast<std::string>(ex.msg_id()) + std::string(" ") + ex.what();
							auth_error_msg = ex.what();
							bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							//wxMessageBox(to_uc(boost::lexical_cast<std::string>(ex.msg_id())) + _(" ") + to_uc(ex.what()));
							
							std::vector<wxString> cap_vec;
							std::vector<wxColour> color_vec;
							std::vector<bool> enabled_vec;
							std::vector<bool> checked_vec;
							std::vector<bool> wrap_vec;
							
							cap_vec.push_back(_("Retry"));
							color_vec.push_back(ok_color);
							enabled_vec.push_back(true);
							checked_vec.push_back(false);
							wrap_vec.push_back(true);
							wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Communication ERROR."), _("There is an error communicating with CC processing center. Please retry or cancel the operation."), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
							int result = form->ShowModal() - wxID_HIGHEST;
							if (result == 0) {
								*flog << LLOCATION;
								cont = true;
							}
							else {
								*flog << LLOCATION;
								return;
							}
						}
						catch (...) {
							throw;
						}
					} while (cont);
				}
				catch (ksi_client::pos_exception& ex) {
					*flog << boost::lexical_cast<std::string>(ex.msg_id()) + std::string(" ") + ex.what();
					auth_error_msg = ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(boost::lexical_cast<std::string>(ex.msg_id())) + _(" ") + to_uc(ex.what()));
					return;
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
					return;
				}
				catch (...) {
					*flog << ERROR_IN_LOCATION;
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
					wxMessageBox(to_uc(ERROR_IN_LOCATION));
					return;
				}
				swiped_ = false;
			}
			else return;
			
			std::string cc_type = "";
			std::string cardHolder_fname = "";
			std::string cardHolder_lname = "";
			
			try {
				*flog << "ksi_client::user_pos_i::get_card_type";
				cc_type = client->get_card_type(auth_txn_id);
				*flog << "successful";
				*flog << "ksi_client::user_pos_i::get_card_holder_first_name";
				cardHolder_fname = client->get_card_holder_first_name(auth_txn_id);
				*flog << "successful";
				*flog << "ksi_client::user_pos_i::get_card_holder_last_name";
				cardHolder_lname = client->get_card_holder_last_name(auth_txn_id);
				*flog << "successful";
			}
			catch (ksi_client::pos_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			}
			
			*flog << "ksi_cafe::first6andlast4digits";
			std::string msk_card_num = ksi_cafe::first6andlast4digits(cc_buff_.substr(cc_buff_.find(";")+1, cc_buff_.find("=")-cc_buff_.find(";")-1));
			*flog << "successful";
			ksi_cafe::card_info_t card_info(msk_card_num, cc_type, cardHolder_fname + " " + cardHolder_lname, result == 0);
			ksi_cafe::credit_auth_t auth(card_info, auth_txn_id, client->get_auth_code(auth_txn_id));	
			
			
			if (!auth_txn_id) {
				*flog << LLOCATION;
				std::string xml = print_csleep(*cafe_container->current_cafe(), msk_card_num, auth_error_msg, auth_txn_id, swiped_);
				try {
					print_xml(printer.get(), xml);
				}
				catch (exception_t& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.user_message()));
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
				}
				return;
			}
			payment = new ksi_cafe::credit_payment_t(auth, total);
			
			*flog << "ksi_cafe::order_t::set_accept";
			bool successful_commit = false;
			try {
				order->set_payment_credit_card(payment);
				order->set_accept(0, true);
				*flog << "successful";
				*flog << "auth_payment";
				payment->auth_payment(order->order_id(), true);
				*flog << "successful";
				successful_commit = true;
			}
			catch (ksi_client::connection_problem_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			catch (ksi_client::db_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()));
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			if (!successful_commit) {
				*flog << WARNING_IN_LOCATION;
				return;
			}
			try {
				std::string msg = print_msleep(*cafe_container->current_cafe(), order->order_id(), order->total(), cc_type, msk_card_num, cardHolder_fname + " " + cardHolder_lname, auth_txn_id, swiped_);
				print_xml(printer.get(), msg);
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()));
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}			
		}
		else {
			payment = dynamic_cast<ksi_cafe::credit_payment_t*>(order->cur_payment());
			auth_txn_id = payment->auth_txn_id();
		}
		
		wx_window_ptr<tip_amount_f> form (new tip_amount_f(this, _("Input tips"), total, false));
		int result = form->ShowModal();
		if (result == wxID_OK) { // Ok
			double tip = form->tip();
			bool successful_commit = false;
			try {
				*flog << std::string("add_to_batch. auth_txn_id = ") + boost::lexical_cast<std::string>(auth_txn_id);
				/**
				int txn_id = client->credit_add_to_batch(auth_txn_id, total+tip, tip);
				**/
				int txn_id = 0;
				bool cont;
				do {
					cont = false;
					ksi_client::pos_exception* except = 0;
					wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), locale->get("global", "communication_with_processing_server_please_wait", "Communicating with processing server, please wait.")));
					boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_add_to_batch, client.get(), auth_txn_id, total+tip, tip);
					fin_form->start(boost::bind(cc_processing, &txn_id, &except, f));
					fin_form->ShowModal();
					try {
						std::auto_ptr<ksi_client::pos_exception> except_aptr;
						except_aptr.reset(except);
						if (except != 0) {
							if (typeid(*except) == typeid(ksi_client::fatal_pos_exception))
								throw dynamic_cast<ksi_client::fatal_pos_exception&>(*except);
							throw *except->clone();
						}
						*flog << "successful";
					}
					catch (ksi_client::fatal_pos_exception& ex) {
						*flog << LLOCATION;
						*flog << boost::lexical_cast<std::string>(ex.msg_id()) + std::string(" ") + ex.what();
						bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							//wxMessageBox(to_uc(boost::lexical_cast<std::string>(ex.msg_id())) + _(" ") + to_uc(ex.what()));
						
						std::vector<wxString> cap_vec;
						std::vector<wxColour> color_vec;
						std::vector<bool> enabled_vec;
						std::vector<bool> checked_vec;
						std::vector<bool> wrap_vec;
							
						cap_vec.push_back(_("Retry"));
						color_vec.push_back(ok_color);
						enabled_vec.push_back(true);
						checked_vec.push_back(false);
						wrap_vec.push_back(true);
						wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Communication ERROR."), _("There is an error communicating with CC processing center. Please retry or cancel the operation."), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
						int result = form->ShowModal() - wxID_HIGHEST;
						if (result == 0) {
							*flog << LLOCATION;
							cont = true;
						}
						else {
							*flog << LLOCATION;
						}
					}
				} while (cont);
				int batch_id = 0;
				try {
					batch_id = client->get_current_batch_id();
				}
				catch (ksi_client::pos_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
				}
				catch (...) {
					*flog << ERROR_IN_LOCATION;
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
					wxMessageBox(to_uc(ERROR_IN_LOCATION));
				}
				*flog << std::string("batch_id = ") + boost::lexical_cast<std::string>(batch_id);
				payment->credit_add_to_batch(txn_id, batch_id, total, tip);
				successful_commit = true;
			}
			catch (ksi_client::pos_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			
			if (!successful_commit) {
				int order_id = order->order_id();
				ksi_cafe::table_t table = order->table();
				ksi_cafe::token_t token = order->token();
				bool successful_oper = false;
				try {
					*flog << "ksi_cafe::order_t::commit(), order_id = " + boost::lexical_cast<std::string>(order_id);
					if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
					*flog << "commit successful";
					successful_oper = true;
				}
				catch (ksi_client::connection_problem_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
				}
				catch (ksi_client::db_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
				}
				catch (exception_t& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.user_message()));
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
				}
				if (!successful_oper) {
					*flog << WARNING_IN_LOCATION;
					return;
				}
				//resume_wait();
				return;
			}
			
			successful_commit = false;	
			try {
				*flog << "ksi_cafe::order_t::set_accept";
				order->set_accept(0, true);
				*flog << "successful";
				successful_commit = true;
			}
			catch (ksi_client::connection_problem_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			catch (ksi_client::db_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()));
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			if (!successful_commit) {
				*flog << WARNING_IN_LOCATION;
				return;
			}
		}
		else { // Cancel
			try {
				*flog << "ciredt_void, auth_txn_id = " + boost::lexical_cast<std::string>(auth_txn_id);
				/**
				int txn_id = client->credit_void(auth_txn_id);
				**/
				ksi_client::pos_exception* except = 0;
				wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), locale->get("global", "communication_with_processing_server_please_wait", "Communicating with processing server, please wait.")));
				boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_void, client.get(), auth_txn_id);
				int txn_id = 0;
				fin_form->start(boost::bind(cc_processing, &txn_id, &except, f));
				fin_form->ShowModal();
				if (except != 0) throw *except;	
				*flog << "successful";
				try {
					ksi_cafe::credit_payment_t* curr_payment = dynamic_cast<ksi_cafe::credit_payment_t*>(order->cur_payment());
					if (!curr_payment) {
						*flog << ERROR_IN_LOCATION;
						wxMessageBox(to_uc(ERROR_IN_LOCATION));
						return;
					}
					std::string xml = print_csleep(*cafe_container->current_cafe(), curr_payment->num(), txn_id, auth_txn_id);
					print_xml(printer.get(), xml);
				}
				catch (exception_t& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.user_message()));
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
				}
				wxMessageBox(locale->get("global", "credit_card_auth_has_been_void", "Credit card authorization has been void"));
				bool successful_commit = false;	
				try {
					*flog << std::string("credit_void txn_id = ") + boost::lexical_cast<std::string>(txn_id);
					order->set_payment_empty(txn_id);
					*flog << "ksi_cafe::order_t::set_accept";
					order->set_accept(0, true);
					*flog << "successful";
					bool successful_commit = true;	
				}
				catch (ksi_client::connection_problem_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
				}
				catch (ksi_client::db_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
				}
				catch (exception_t& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.user_message()));
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
				}
				if (!successful_commit) {
					*flog << WARNING_IN_LOCATION;
					return;
				}
				
				int order_id = order->order_id();
				ksi_cafe::table_t table = order->table();
				ksi_cafe::token_t token = order->token();
				successful_commit = false;
				try {
					*flog << "ksi_cafe::order_t::commit(), order_id = " + boost::lexical_cast<std::string>(order_id);
					if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
					*flog << "commit successful";
					successful_commit = true;
				}
				catch (ksi_client::connection_problem_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
				}
				catch (ksi_client::db_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
				}
				catch (exception_t& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.user_message()));
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()));
				}
				if (!successful_commit) {
					*flog << WARNING_IN_LOCATION;
					return;
				}
			}
			catch (ksi_client::pos_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			resume_wait();
			return;
		}
	}
	else if (payment_type_id == 3) { // Check
	}
	else if (payment_type_id == 4) { // dummy
		order->set_payment_dummy(new ksi_cafe::dummy_payment_t());
		bool successful_commit = false;
		try { 
			*flog << "ksi_cafe::order_t::set_accept";
			order->set_accept(0, true);
			*flog << "successful";
			successful_commit = true;
		}
		catch (ksi_client::connection_problem_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			logout();
		}
		catch (ksi_client::db_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
		}
		catch (exception_t& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()));
		}
		catch (std::exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
		}
		if (!successful_commit) {
			*flog << WARNING_IN_LOCATION;
			return;
		}
	}
	
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));	
	bool sucessful_commit = false;
	try {
		*flog << "ksi_cafe::order_t::set_close";
		order->set_close(true);
		*flog << "successful";
		if (registrar.get()) {
			bool print_successful = false;
			try {
				*flog << "print_order";			
				print_order(*order, *registrar, cash);
				print_successful = true;
				*flog << "successful";
			}
			catch (fiscal::exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
			}
			if (!print_successful) {
				wxMessageBox(_("Не удалось напечатать фискальный ресит."));
				resume_wait();
				return;
			}
		}
		
		*flog << "ksi_cafe::order_t::commit(), order_id = " + boost::lexical_cast<std::string>(order->order_id());
		if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
		*flog << "commit successful";
		std::string xml = print_order(*order, by_group, !stand_alone);
		print_xml(printer.get(), xml);
		sucessful_commit = true;
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
	

	if (sucessful_commit) {
		if (printer.get()) printer->open_drawer();
		if (drawer.get()) drawer->open();
		
		std::string order_id_str = boost::lexical_cast<std::string>(order->order_id());
		if (order_id_str.length() > 3) {
			std::string pref_order_id = order_id_str.substr(0, order_id_str.length()-3);
			std::string end_order_id = order_id_str.substr(order_id_str.length()-3, 3);
			order_id_str = pref_order_id + "-" + end_order_id;
		}

		wxString info_lb = 
		locale->get("global", "Order_str", "Order") + _(" # ") 
		+ to_uc(order_id_str) 
		+ locale->get("global", "has_been_successfully created_str", " has been successfully created.") 
		+ _("\n\n") 
		+ locale->get("global", "order_total_is_str", "order total is ") 
		+ locale->get_currency(order->total());
		wxString change_lb = _("");
		if (order->payment_cash()) change_lb = locale->get("global", "change_is_str", "Change is ") + locale->get_currency(change);
		wx_window_ptr<info_f> info (new info_f(this, locale->get("global", "order_created_str", "Order created"), info_lb, change_lb, order->payment_cash()));
	
		resume_wait();
		info->ShowModal();
	}
	else resume_wait();
}

void cashier_f::OnTimer (wxTimerEvent& event) {
	if (event.GetId() == timer_.timer_id()) {
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		current_dt_lb->SetLabel(locale->get_date_time(now));
	}
}

void cashier_f::OnSeanceTimer (wxTimerEvent& event) {
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	refresh_order_list();
}

void cashier_f::OnLockTimer(wxTimerEvent& event) {
	//pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	//pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
}

void cashier_f::sgSelChanged( wxTreeEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	
	wxTreeItemId id = event.GetItem();
	if (!event.GetItem().IsOk()) return;
	if (event.GetItem() == sg->GetRootItem()) return;
	
	order_node_t* node = dynamic_cast<order_node_t*>(sg->GetItemData(event.GetItem()));
	if (!node) return;
	
	if (node->type() == node_type::order_dish)
	{
		comment_entree_btn->Enable(true);
		plus_btn->Enable(true);
		minus_btn->Enable(true);
		delete_btn->Enable(true);		
	}
}

void cashier_f::sgSelChanging( wxTreeEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	
	comment_entree_btn->Enable(false);
	plus_btn->Enable(false);
	minus_btn->Enable(false);
	delete_btn->Enable(false);
}

void cashier_f::recalc_coupon() {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	
	wxTreeItemId sel = sg->GetSelection();
	if (sel.IsOk() && sel != sg->GetRootItem()) {
		try {		
			const ksi_cafe::order_dish_t& sel_dish = get_order_dish(sg);
			recalc_coupon(sel_dish.type_check_sum(), sel_dish.check_sum());
		}
		catch (exception_t& ex) {
			recalc_coupon("", "");
		}
	}
	else recalc_coupon("", "");
}

void cashier_f::sg_show(const std::string& type_check_sum, const std::string& check_sum) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	
	try {
		sg_order_show(sg, *order.get(), type_check_sum, check_sum);
	}
	catch (...) {
		*flog << "... void cashier_f::sg_show(const std::string&, const std::string&)";
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, "... void cashier_f::sg_show(const std::string&, const std::string&)"));
		wxMessageBox(_("... void cashier_f::sg_show(const std::string&, const std::string&)"));
		throw;
	}
	if (order->real_dish_count()) {
		double subtotal = order->type_employee_meal() ? order->def_subtotal() : order->subtotal();
		double tax = order->type_employee_meal() ? order->def_tax() : order->tax();
		double gratuity = order->type_employee_meal() ? order->def_gratuity() : order->gratuity();
		change_subtotal_ed(locale->get_currency(subtotal));
		change_sales_tax_lb(_("Sales tax (") + to_uc(ksi_cafe::percent_t().format(order->sales_tax()*100))+_(")"));
		change_sales_tax_ed(locale->get_currency(tax));
		change_gratuity_lb(_("Gratuity (") + to_uc(ksi_cafe::percent_t().format(order->gratuity_tax()*100))+_(")"));
		change_gratuity_ed(locale->get_currency(gratuity));
		change_total_ed(locale->get_currency(order->total()));
	}
	else {
		change_subtotal_ed(_(""));
		change_sales_tax_lb(_(""));
		change_sales_tax_ed(_(""));
		change_gratuity_lb(_(""));
		change_gratuity_ed(_(""));
		change_total_ed(_(""));
	}
	if (order->type_delivery() && order->delivery_fee() != 0) {
		change_tools_lb(locale->get("cashier_f", "tools_lb_devliery_fee_str", "Delivery fee"));
		change_tools_ed(locale->get_currency(order->delivery_fee()));
	}
	else if (order->type_employee_meal()) {
		change_tools_lb(locale->get("cashier_f", "tools_lb_employee_str", "Employee: "));
		if (order->employee_id() != 0)
			change_tools_ed(to_uc(users->user(order->employee_id()).fio()));
		else
			change_tools_ed(to_uc(order->memo()));
	}
	else {
		change_tools_lb(_(""));
		change_tools_ed(_(""));
	}
}

void cashier_f::base_recalc_coupon(const std::string& type_check_sum, const std::string& check_sum) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	
	if (order->type_employee_meal()) {
		int employee_id = order->employee_id();
		std::string memo = order->memo();
		order->clear_employee();
		order->set_type_employee_meal(employee_id, memo, false);		
		return;
	}
	else
		order->clear_employee();
	
	
	order->save_misc_charges();
	order->save_adjustable_discount();
	order->clear_coupon();
	std::vector<ksi_cafe::coupon_t*> fix_discount;
	
	
	std::vector<ksi_cafe::coupon_t*> selected = coupon_in_cafe->selected();
	std::vector<ksi_cafe::coupon_t*>::iterator end = selected.end();
	for (std::vector<ksi_cafe::coupon_t*>::iterator i=selected.begin(); i!=selected.end(); ++i) {
		int cur_size = 1;
		
		if (!(*i)->fixed_discount()) {
			while ( ((*i)->auto_scalable() == 1 || coupon_in_cafe->coupon_count(*i) >= cur_size)  && apply_coupon(**i, order->all_dish(), coupon_in_cafe->mng_id(*i)) ) {
				++cur_size;
			}
			(*i)->set_apply(cur_size-1);
		}
		else if (apply_coupon(**i, order->all_dish(), coupon_in_cafe->mng_id(*i)) && (*i)->fixed_discount()) {
			fix_discount.push_back(*i);
		}		
	}
	for (std::vector<ksi_cafe::coupon_t*>::iterator i=fix_discount.begin(); i!=fix_discount.end(); ++i) {
		order->add_fixed_discount((*i)->fixed_discount(), (*i)->coupon_id(), (*i)->title(), coupon_in_cafe->mng_id(*i));
	}
	
	order->compress();
	order->load_misc_charges();
	order->load_adjustable_discount();	
}

void cashier_f::recalc_coupon(const std::string& type_check_sum, const std::string& check_sum) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	base_recalc_coupon(type_check_sum, check_sum);
	sg_show(type_check_sum, check_sum);
	vfd_clear_line(display.get(), 2);
	int order_dish_count = 0;
	if (order.get()) order_dish_count = order->dish_count();
	
	if (order_dish_count > 0) 
		vfd_print(display.get(), 2, "Subtotal", to_mb(locale->get_currency(order->subtotal())), 0);
	else if (order_dish_count == 0) {
		vfd_print(display.get(), 1, vfd_line1, "", vfd_align_line1);
		vfd_print(display.get(), 2, vfd_line2, "", vfd_align_line2);
	}

}

void cashier_f::discount_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	try {
		*flog << "discount_btnClick";
		wx_window_ptr<coupon_list_f> form (new coupon_list_f(this, locale->get("cashier_f", "select_coupon_discout_str", "Select a coupon/discount"), locale->get("cashier_f", "coupon_msg1", "Green items are applicable to current order. Red ones are not. Yellow are system discounts."), order.get()));
		sg_clear(sg);
		form->ShowModal();
		recalc_coupon("", "");
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()));
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}	
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		*flog << ERROR_IN_LOCATION;
	}
}

void cashier_f::resume_wait() {
	*flog << "cashier_f::resume_wait()";
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	*flog << "timers paused";
	if (!ksi_cafe::session_exist()) { logout(); }
	sg_clear(sg);
	*flog << "order.reset()";
	order.reset();
	try {
		*flog << "order.reset(new ksi_cafe::order_t(...";
		order.reset(new ksi_cafe::order_t(*cafe_container->current_cafe(), 0));
	}
	catch (exception_t& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
		wxExit();
		return;
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
		return;
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
		wxExit();
		return;
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		wxExit();
		return;
	}
	
	order->set_hide(hide_mode_);
	
	*flog << "recalc_coupon";
	recalc_coupon("", "");
	*flog << "recalc_coupon successful";
	token_btn->SetLabel(locale->get("cashier_f", "token_btn1", "Token")+_(": ")+locale->get("cashier_f", "token_btn2", "not selected"));
	wrap_label(token_btn);
	order_type_toggle_group_ptr->toggle(for_here_btn);
	*flog << "coupon_in_cafe_t::unselect_all()";
	coupon_in_cafe->unselect_all();
	*flog << "successful";
	delayed_order_details_btn->Hide();
	save_and_hide_btn->Hide();
	*flog << "refresh_order_list()";
	refresh_order_list();
	*flog << "cashier_f::resume_wait(). return;";
}

void cashier_f::misc_charges_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	bool auth_ = true;
	if (config->get("security", "create_misc_charges") == "0" || to_uc(config->get("security", "create_misc_charges")).MakeUpper() == _("OFF"))
		auth_ = false;
	try {
		if (auth_ && !wx_mng_auth(this)) return;
		wx_window_ptr<text_f> form (new text_f(this, locale->get("cashier_f", "add_misc_chage_str", "Add a miscellaneous charge"), locale->get("cashier_f", "charge_shot_description_str", "Charge short descrition"), true));
		if (form->ShowModal() != wxID_OK) return;
		const ksi_cafe::order_dish_t& dish = order->add_misc_charges(locale->reverse_currency(form->amount()), to_mb(form->text()));
		recalc_coupon(dish.type_check_sum(), dish.check_sum());
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()));
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));		
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		*flog << ERROR_IN_LOCATION;
	}
}

void cashier_f::comment_entree_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	try {
		const ksi_cafe::order_dish_t& dish = get_order_dish(sg);
		wx_window_ptr<text_f> form (new text_f(this, locale->get("cashier_f", "comment_entree_str", "Comment entry"), _(""), 0));
		form->text(to_uc(dish.description()));
		if (form->ShowModal() != wxID_OK) return;
		const_cast<ksi_cafe::order_dish_t&>(dish).set_description(to_mb(form->text()));
		recalc_coupon(dish.type_check_sum(), dish.check_sum());
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()));
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}

void cashier_f::shift_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	if (registrar.get() == 0) return;
	bool auth_ = true;
	if (config->get("security", "seances") == "0" || to_uc(config->get("security", "seances")).MakeUpper() == _("OFF"))
		auth_ = false;
	if (auth_ && !wx_mng_auth(this)) return;
	try {
		std::vector<wxString> cap_vec;
		std::vector<wxColour> color_vec;
		std::vector<bool> enabled_vec;
		std::vector<bool> checked_vec;
		std::vector<bool> wrap_vec;
		cap_vec.push_back(_("Закрыть фискальную смену"));
		color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true);
		checked_vec.push_back(false);
		wrap_vec.push_back(true);
		wx_window_ptr<btn_list_f> form (new btn_list_f(this, _(""), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
		int result = form->ShowModal() - wxID_HIGHEST;
		if (result == 0) {
			order.reset();
			ksi_cafe::session->rollback();
			ksi_cafe::ora_transaction_t trans(ksi_cafe::session.get());
			ksi_cafe::logical_transaction_t log_trans(ksi_cafe::session.get(), 0);
			std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
			query->create_statement
			(
			"update ksi.seance set standby_time = sysdate where seance_id = ksi.util.get_cur_seance_id and cafe_id = ksi.util.get_cur_cafe_id"
			);
			query->execute_statement();
			query->create_statement
			(
			"update ksi.seance set commited = sysdate where seance_id = ksi.util.get_cur_seance_id and cafe_id = ksi.util.get_cur_cafe_id"
			);
			query->execute_statement();
			query->create_statement
			(
			"select terminal.mac \
			from ksi.seance \
			inner join ksi.terminal on terminal.terminal_id = seance.terminal_id \
			where seance_id = ksi.util.get_cur_seance_id"
			);
			query->execute_statement();
			if (query->next()) THROW_LOCATION(0, "void cashier_f::shift_btnClick(wxCommandEvent&)");
			std::string mac = boost::get<std::string>(query->get(0));
			query->create_statement
			(
			"begin ksi.CREATE_SESSION('" + mac + "'); end;"
			);
			query->execute_statement();
			
			log_trans.logical_end();
			
			registrar->Zreport();
			
			trans.commit(true);
			logout();
		}
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
		logout();
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (fiscal::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	//resume_wait();
}

void cashier_f::refund_order_btnClick( wxCommandEvent& event ) {
	//if (!wx_mng_auth()) return;
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	try {
		wx_window_ptr<close_order_manager_f> form (new close_order_manager_f(this, locale->get("cashier_f", "close_order_management_str", "Closed order management")));
		form->ShowModal();
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
}

void cashier_f::OnConfigDbClick( wxMouseEvent& event ) {
	wx_disable_t disable_window(this);	
	seance_timer_.stop();
	lock_timer_.stop();
	*flog << "cashier_f::OnConfigDbClick(wxMouseEvent&)";
	bool auth_ = true;
	if (config->get("security", "access_to_settings") == "0" || to_uc(config->get("security", "access_to_settings")).MakeUpper() == _("OFF"))
		auth_ = false;
	if (auth_ && !wx_mng_auth(this)) return;
	wx_window_ptr<config_f> form (new config_f(this, locale->get("global", "Settings_str", "Settings")));
	printer.reset();
	com.reset();
	display.reset();
	int result = form->ShowModal();
	if (result == wxID_ABORT) wxExit();
	else {
		if (config->get("locale", "enable") == "1") {
			try {
				locale.reset(new ksi_cafe::xml_lang_t(config->get("locale", "file"), config->get("locale", "name"), "cashier"));
				apply_lang();
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.user_message()));
			}
		}
		else {
			locale.reset(new ksi_cafe::xml_lang_t());
			apply_lang();
		}
		wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
		try {
			if (config->get("vfd_display", "enable") == "1" || to_uc(config->get("vfd_display", "enable")).MakeUpper() == _("ON")) {
				try {
					*flog << "create customer_display_t";
					display.reset(new customer_display_t("COM4"));
					*flog << "successful";
					vfd_line1 = config->get("vfd_display", "line1_text");
					vfd_line2 = config->get("vfd_display", "line2_text");

					if (to_uc(config->get("vfd_display", "line1_align")).MakeUpper() == _("LEFT")) {
						vfd_align_line1 = 0;
					}
					else if (to_uc(config->get("vfd_display", "line1_align")).MakeUpper() == _("RIGHT")) {
						vfd_align_line1 = 2;
					}
					else vfd_align_line1 = 1;
					
					if (to_uc(config->get("vfd_display", "line2_align")).MakeUpper() == _("LEFT")) {
						vfd_align_line2 = 0;
					}
					else if (to_uc(config->get("vfd_display", "line2_align")).MakeUpper() == _("RIGHT")) {
						vfd_align_line2 = 2;
					}
					else vfd_align_line2 = 1;
				}
				catch (...) {
					*flog << "Unable to initialize COM4 port. VFD display will be disabled";
					wxMessageBox(_("Unable to initialize COM4 port. VFD display will be disabled"));
				}
			}
			
			if (config->get("resolution", "auto") == "1" || to_uc(config->get("resolution", "auto")).MakeUpper() == _("ON")) {
				wxScreenDC screen;
				SetSize(screen.GetSize().GetWidth(), screen.GetSize().GetHeight());
				btn_repaint();
				Centre(wxBOTH);
			}
			else {
				try {
					int width = boost::lexical_cast<int>(config->get("resolution", "width"));
					int height = boost::lexical_cast<int>(config->get("resolution", "height"));
					SetSize(width, height);
					btn_repaint();
					Centre(wxBOTH);
				}
				catch (...){
					*flog << "bad cast resolution config, width = " + config->get("resolution", "width") + ", height = " + config->get("resolution", "height");
					wxScreenDC screen;
					SetSize(screen.GetSize().GetWidth(), screen.GetSize().GetHeight());
					btn_repaint();
					Centre(wxBOTH);
				}
			}
			if (config->get("app", "auto_lock") == "1" || to_uc(config->get("app", "auto_lock")).MakeUpper() == _("ON"))
				auto_lock_ = true;
			else auto_lock_ = false;
			if (config->get("reciept", "group_by_kitchen_id") == "1" || to_uc(config->get("reciept", "group_by_kitchen_id")).MakeUpper() == _("ON"))
				by_group = true;
			else
				by_group = false;
			
			bool tested_client = false;
			try {
				client.reset();
				if (to_uc(config->get("Credit_Card_SubSystem", "processing")).MakeUpper() == _("HEARTLAND")) {
					stand_alone = false;
					*flog << "create ksi_client::user_pos_i";
					int license_id = boost::lexical_cast<int>(config->get("Credit_Card_SubSystem", "license_id"));
					int site_id = boost::lexical_cast<int>(config->get("Credit_Card_SubSystem", "site_id"));
					int device_id = boost::lexical_cast<int>(config->get("Credit_Card_SubSystem", "device_id"));
					std::string user = config->get("Credit_Card_SubSystem", "user");
					std::string password = config->get("Credit_Card_SubSystem", "password");
					client = ksi_client::create_user_pos(license_id, site_id, device_id, user, password);
					client->test_credentials();
					*flog << "sucessful";
					tested_client = true;
				}
				else if (to_uc(config->get("Credit_Card_SubSystem", "processing")).MakeUpper() == _("STAND_ALONE")) {
					stand_alone = true;
				}
				else {
					stand_alone = false;
				}
			}
			catch (ksi_client::pos_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			catch (exception_t& ex) {
				*flog << ex.comp_message();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()));
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
			catch (...) {
				*flog << ERROR_IN_LOCATION;
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
				wxMessageBox(to_uc(ERROR_IN_LOCATION));
			}
			if (!tested_client) client.reset(0);
			try {
				if (to_uc(config->get("printer", "source")).MakeUpper() == _("REMOTE")) {
					printer.reset(new net_printer_t(config->get("printer", "host"), 703));
				}
				else if (to_uc(config->get("printer", "source")).MakeUpper() == _("LOCAL")) {
					if (to_uc(config->get("printer", "command_set")).MakeUpper() == _("STAR")) {
						com.reset(new com_t(config->get("printer", "port"), boost::lexical_cast<int>(config->get("printer", "baud_rate"))));
						printer.reset(new star_printer_t(*com.get()));
					}
					else if (to_uc(config->get("printer", "command_set")).MakeUpper() == _("EPSON")) {
						com.reset(new com_t(config->get("printer", "port"), boost::lexical_cast<int>(config->get("printer", "baud_rate"))));
						printer.reset(new epson_printer_t(*com.get()));
					}
					else if (to_uc(config->get("printer", "command_set")).MakeUpper() == _("FISCAL REGISTRAR")) {
						com.reset(new com_t(config->get("printer", "port"), boost::lexical_cast<int>(config->get("printer", "baud_rate")), 3));
						registrar.reset(new fiscal::registrator_t(*com.get()));
					}
				}
				else {
					printer.reset(new null_printer_t());
				}
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				//bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()));
			}
			catch (fiscal::exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
			}
			catch (...) {
				*flog << "can't open serial port";
				//bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, "can't open serial port"));
				wxMessageBox(_("can't open serial port"));
			}
			if (printer.get() == 0) printer.reset(new null_printer_t());	
			
			//"never" "15 sec" "30 sec" "1 min" "5 min" "10 min"
			std::string lock_timeout_str = config->get("app", "lock_timeout");
			if (lock_timeout_str == "15 sec") lock_timer_.start(15);
			else if (lock_timeout_str == "30 sec") lock_timer_.start(30);
			else if (lock_timeout_str == "45 sec") lock_timer_.start(45);
			else if (lock_timeout_str == "1 min") lock_timer_.start(60);
			else if (lock_timeout_str == "5 min") lock_timer_.start(300);
			else if (lock_timeout_str == "10 min") lock_timer_.start(600);
			//"never" "5 sec" "10 sec" "15 sec" "30 sec" "60 sec"
			std::string check_timeout_str = config->get("app", "check_timeout");
			if (check_timeout_str == "5 sec") seance_timer_.start(5);
			else if (check_timeout_str == "10 sec") seance_timer_.start(10);
			else if (check_timeout_str == "15 min") seance_timer_.start(15);
			else if (check_timeout_str == "30 min") seance_timer_.start(30);
			else if (check_timeout_str == "60 min") seance_timer_.start(60);
		}
		catch (ksi_client::ksi_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			wxExit();
		}
		catch (exception_t& ex) {
			*flog << ex.comp_message();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()));
			wxExit();
		}
		catch (std::exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			wxExit();
		}
		catch (...) {
			*flog << ERROR_IN_LOCATION;
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
			wxMessageBox(to_uc(ERROR_IN_LOCATION));
			wxExit();
		}
		if (config->get("functionality", "order_type_for_here") == "0" || to_uc(config->get("functionality", "order_type_for_here")).MakeUpper() == _("OFF")) {
			for_here_btn->Show(false);
			for_here_type_ = false;
		}
		else {
			for_here_btn->Show(true);
			for_here_type_ = true;
		}
		if (config->get("functionality", "order_type_to_go") == "0" || to_uc(config->get("functionality", "order_type_to_go")).MakeUpper() == _("OFF")) { 
			to_go_btn->Show(false);
			to_go_type_ = false;
		}
		else {
			to_go_btn->Show(true);
			to_go_type_ = true;
		}
		if (config->get("functionality", "order_type_delivery") == "0" || to_uc(config->get("functionality", "order_type_delivery")).MakeUpper() == _("OFF")) { 
			delivery_btn->Show(false);
			delivery_type_ = false;
		}
		else {
			delivery_btn->Show(true);
			delivery_type_ = true;
		}
		if (config->get("functionality", "order_type_pickup") == "0" || to_uc(config->get("functionality", "order_type_pickup")).MakeUpper() == _("OFF")) {
			pickup_btn->Show(false);
			pickup_type_ = false;
		}
		else {
			pickup_btn->Show(true);
			pickup_type_ = false;
		}
		if (config->get("functionality", "order_type_employee_meal") == "0" || to_uc(config->get("functionality", "order_type_employee_meal")).MakeUpper() == _("OFF")) {
			employee_meal_btn->Show(false);
			employee_meal_type_ = false;
		}
		else {
			employee_meal_btn->Show(true);
			employee_meal_type_ = true;
		}
		if (config->get("functionality", "order_type_donation") == "0" || to_uc(config->get("functionality", "order_type_donation")).MakeUpper() == _("OFF")) 
		{
			donation_btn->Show(false);
			donation_type_ = false;
		}
		else {
			donation_btn->Show(true);
			donation_type_ = true;
		}
	}
	ctrl_btn_pnl->Fit();
	ctrl_btn_pnl->Layout();
	ctrl_sizer->Fit( ctrl_btn_pnl );
}

void cashier_f::token_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	std::vector<ksi_cafe::token_t> token_vec;
	std::vector<wxString> cap_vec;
	std::vector<wxColour> color_vec;
	std::vector<bool> checked_vec;
	std::vector<bool> enabled_vec;
	std::vector<bool> wrap_vec;
	cap_vec.push_back(_("No Token"));
	color_vec.push_back(btn_face_color);
	if (!order->token().exist()) checked_vec.push_back(true);
	else checked_vec.push_back(false);
	enabled_vec.push_back(true);

	try {
		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
		query->create_statement
		(
		"select token_id, name \
		from ksi.token \
		where cafe_id = ksi.util.get_cur_cafe_id \
		order by upper(name)"
		);
		query->execute_statement();
		
		while (!query->next()) {
			int token_id = static_cast<int>(boost::get<double>(query->get(0)));
			std::string name = boost::get<std::string>(query->get(1));
			token_vec.push_back(ksi_cafe::token_t(token_id, name));
			cap_vec.push_back(to_uc(name));
			color_vec.push_back(btn_face_color);
			if (order->token().exist() && order->token().token_id() == token_id) checked_vec.push_back(true);
			else checked_vec.push_back(false);
			enabled_vec.push_back(true);
			wrap_vec.push_back(true);
		}
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}

	try {
		wx_window_ptr<btn_list_f> form (new btn_list_f(this, locale->get("global", "select_a_token_str", "Select a token"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
		int result = form->ShowModal() - wxID_HIGHEST;
		if (result >= 0 && result < token_vec.size()+1) {
			if (result == 0) {
				order->change_token(ksi_cafe::token_t());
				token_btn->SetLabel(locale->get("cashier_f", "token_btn1", "Token")+_(": ")+locale->get("cashier_f", "token_btn2", "not selected"));
				wrap_label(token_btn);
			}
			else {
				order->change_token(token_vec[result-1]);
				token_btn->SetLabel(to_uc(token_vec[result-1].name()));
				wrap_label(token_btn);
			}
		}
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}

void cashier_f::entree_code_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	wx_window_ptr<input_num_f> form (new input_num_f(this, locale->get("global", "type_entree_code_str", "Type entree code"), ksi_cafe::identical_format_t()));
	if (form->ShowModal() != wxID_OK) return;
	std::string code = to_mb(form->format_text());
	const ksi_cafe::menu_dish_t* dish = 0;
	const ksi_cafe::menu_t* menu = 0;
	std::vector<ksi_cafe::menu_t*> menu_vec = menu_container->menu_for_cafe(cafe_container->current_cafe()->cafe_id());
	for (size_t i=0; i<menu_vec.size(); ++i) {
		const ksi_cafe::menu_dish_t* tmp = menu_vec[i]->find_by_code(code);
		if (tmp) { dish = tmp; menu = menu_vec[i]; }
	}
	
	if (dish == 0 || menu == 0) {
		wxMessageBox(locale->get("global", "entree_not_found_str", "Entree not found"));
		return;
	}
	
	try {
		
		stop_list.reset(new ksi_cafe::simple_stop_list_t());
		if (stop_list->find_entry(dish->dish_id())) {
			const ksi_cafe::stop_list_entry_t& entry = stop_list->get_fined_entry(dish->dish_id());
			wxString msg = _("Entree in stop list since ") + locale->get_date_time(entry.since_dt()) + _(" till ") + locale->get_date_time(entry.till_dt());
			if (lang == 2) msg = _("Блюдо в стоп листе с ") + locale->get_date_time(entry.since_dt()) + _(" по ") + locale->get_date_time(entry.till_dt());
			wxMessageBox(msg);
			return;
		}
		
		wx_window_ptr<add_modifiers_f> mod_form (new add_modifiers_f(this, locale->get("global", "select_modifiers_str", "Select modifiers"), stop_list.get()));
		if (mod_form->set_modifiers(menu_manager->selected(), dish)) {
			if (mod_form->ShowModal() != wxID_OK) return;
		}
		std::vector<ksi_cafe::order_dish_modifier_t> mods = mod_form->sel_mods();
		ksi_cafe::order_dish_t odish(*dish, mods);
		order->add_dish(odish, true);
		*flog << "add_dish: " + odish.check_sum();
		recalc_coupon(odish.type_check_sum(), odish.check_sum());
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}

void cashier_f::cash_advance_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	int manager_id = wx_mng_auth(this);
	if (!manager_id) return;
	try {
		expense_list.reset(new ksi_cafe::expense_list_t(*cafe_container->current_cafe(), 0));
		wx_window_ptr<advance_f> form (new advance_f(this, locale->get("global", "Cash_advance_str", "Cash advance"), manager_id));
	
		if (form->ShowModal() != wxID_OK) return;
		
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}	
	
	if (expense_list.get() && expense_list->open_expense_size() > 0) {
		change_bg_colour(cash_advance_btn, wxColour(255, 0, 0));
		open_cash_advance_count_ = expense_list->open_expense_size();
		cash_advance_btn->SetLabel(locale->get("cashier_f", "cash_advance_btn1", "Cash advance") + _(" (") + to_uc(boost::lexical_cast<std::string>(open_cash_advance_count_)) + _(")") + locale->get("cashier_f", "cash_advance_btn2", " open"));
		wrap_label(cash_advance_btn);
	}
	else {
		open_cash_advance_count_ = 0;
		change_bg_colour(cash_advance_btn, btn_face_color);
		cash_advance_btn->SetLabel(locale->get("cashier_f", "cash_advance_btn1", "Cash advance"));
		wrap_label(cash_advance_btn);
	}
}

void cashier_f::open_cash_drawer_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	if (printer.get()) printer->open_drawer();
	if (drawer.get()) drawer->open();
}

void cashier_f::browse_delayed_orders_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	
	std::vector<wxString> cap_vec; std::vector<wxColour> color_vec; std::vector<bool> enabled_vec; std::vector<bool> checked_vec; std::vector<bool> wrap_vec;
	std::vector<int> order_id_vec;
	query->create_statement
	(
	"select \
		order_id \
		, orders.type_id \
		, order_type.name as type \
		, subtotal \
		, created \
		, tables \
		, orders.table_group_id \
		, table_group.name as table_group_name \
		, order_delivery.delivery_date \
		, (order_delivery.firstname || ' ' || order_delivery.lastname) as fio \
		, order_status.name as order_status \
		, orders.creator_id \
		, (order_delivery.streetaddress || ' ' || order_delivery.city || ' ' || state.shortname || ' ' || to_char(order_delivery.zip)) as address \
		, orders.prepayment_type_id \
		, prepayment_type.name as prepayment_type_name \
		, orders.prepayment \
    from ksi.orders \
    left join ksi.table_group on table_group.table_group_id = orders.table_group_id and table_group.cafe_id = ksi.util.get_cur_cafe_id \
    left join ksi.order_type on order_type.type_id = orders.type_id \
    left join ksi.order_delivery on orders.cafe_id = order_delivery.cafe_id and orders.delivery_id = order_delivery.delivery_id \
    left join ksi.state on state.state_id = order_delivery.state_id \
    left join ksi.order_status on order_status.status_id = orders.status_id \
	left join ksi.payment_type prepayment_type on prepayment_type.payment_type_id = orders.prepayment_type_id \
    where status_id in (1,2,6) and orders.cafe_id = ksi.util.get_cur_cafe_id \
    and orders.type_id in (3, 4) \
    order by orders.order_id"
	);
	
	try {
		query->execute_statement();
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	
	while (!query->next()) {
		int order_id = static_cast<int>(boost::get<double>(query->get(0)));
		int type_id = static_cast<int>(boost::get<double>(query->get(1)));
		std::string type_name = boost::get<std::string>(query->get(2));
		double subtotal = boost::get<double>(query->get(3));
		boost::posix_time::ptime created = boost::get<boost::posix_time::ptime>(query->get(4));
		std::string table_name = boost::get<std::string>(query->get(5));
		int table_group_id = static_cast<int>(boost::get<double>(query->get(6)));
		std::string table_group_name = boost::get<std::string>(query->get(7));
		boost::posix_time::ptime delviery_date = boost::get<boost::posix_time::ptime>(query->get(8));
		std::string customer_fio = boost::get<std::string>(query->get(9));
		std::string order_status = boost::get<std::string>(query->get(10));
		int creator_id = static_cast<int>(boost::get<double>(query->get(11)));
		std::string delivery_address = boost::get<std::string>(query->get(12));
		int prepayment_type_id = static_cast<int>(boost::get<double>(query->get(13)));
		std::string prepayment_type_name = boost::get<std::string>(query->get(14));
		double prepayment = boost::get<double>(query->get(15));
		std::string prepayment_str = "";
		if (prepayment_type_id != 0) prepayment_str = ", prepayment " + to_mb(locale->get_currency(prepayment));
		std::string cap = "Order # " + boost::lexical_cast<std::string>(order_id) + " " + order_status + "\n"
		+ "Created by " + users->user(creator_id).fio() + "\n"
		+ "Accepted on " + to_mb(locale->get_date_time(created))+"\n"
		+ ", Subtotal: " + to_mb(locale->get_currency(subtotal)) + prepayment_str + "\n";
		std::string tmp_cap = "Delivery on " + to_mb(locale->get_date_time(delviery_date)) + " to " + customer_fio + "\n" + delivery_address;
		if (type_id == 4) tmp_cap = "Pick up on " + to_mb(locale->get_date_time(delviery_date)) + " to " + customer_fio + "\n";
		cap_vec.push_back(to_uc(cap+tmp_cap));
		color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true);
		checked_vec.push_back(false);
		wrap_vec.push_back(false);
		order_id_vec.push_back(order_id);
	}
	
	wx_window_ptr<btn_list_f> form (new btn_list_f(this, locale->get("global", "delayed_order_list_str", "Delayed orders list"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
	int result = form->ShowModal() - wxID_HIGHEST;
	if (result >= 0 && result < order_id_vec.size()) {
		int order_id = order_id_vec[result];
		sg_clear(sg);
		order.reset();
		bool catch_exception = false;
		int res = 0;
		ksi_cafe::order_t* ord_ptr = 0;
		try {
			*flog << "load delayed order_t, order_id = " + boost::lexical_cast<std::string>(order_id);
			wx_window_ptr<load_f> form (new load_f(this, locale->get("global", "order_load_str", "Order is loading"), locale->get("global", "please_wait_str", "Please wait.\nIf order is not opened for a long time, it might have been already opened by somebody else.")));
			form->start(boost::bind(load_order2, &ord_ptr, *cafe_container->current_cafe(), order_id, true, false));
			res = form->ShowModal();
		}
		catch (std::exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			if (ord_ptr) {
				*flog << "delete ksi_cafe::order_t";
				delete ord_ptr;
				*flog << "successful";
				ord_ptr = 0;
			}
			catch_exception = true;
		}
		catch (...) {
			*flog << ERROR_IN_LOCATION;
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
			wxMessageBox(to_uc(ERROR_IN_LOCATION));
			if (ord_ptr) {
				*flog << "delete ksi_cafe::order_t";
				delete ord_ptr;
				*flog << "successful";
				ord_ptr = 0;
			}
			catch_exception = true;
		}
		
		if (res == wxID_OK) {
			if (ord_ptr) {
				*flog << "loaded successful, ksi_cafe::order_t::status_id = " + boost::lexical_cast<std::string>(ord_ptr->status_id());
				if (ord_ptr->status_accepted() || ord_ptr->status_ready() || ord_ptr->status_send_to_kitchen()) order.reset(ord_ptr);
				else {
					*flog << "delete ksi_cafe::order_t";
					delete ord_ptr;
					*flog << "successful";
					ord_ptr = 0;
				}
			}
		}
		else {
			*flog << "delete ksi_cafe::order_t";
			delete ord_ptr;
			*flog << "successful";
			ord_ptr = 0;
		}
		if (order.get()) {
			order->clear_wasted();
			if (order->type_delivery()) order_type_toggle_group_ptr->toggle(delivery_btn);
			else if (order->type_pickup()) order_type_toggle_group_ptr->toggle(pickup_btn);
			delayed_order_details_btn->Show(true);
			save_and_hide_btn->Show(true);
			save_and_hide_btn->Enable(order->type_delivery() || order->type_pickup());
			recalc_coupon();
		}
		else {
			resume_wait();
			return;
		}
	}	
	else resume_wait();
	ctrl_btn_pnl->SetSizer( ctrl_sizer );
	ctrl_btn_pnl->Layout();
	ctrl_sizer->Fit( ctrl_btn_pnl );
}

void cashier_f::logout_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	
	wx_hide_t hide_body(main_sizer, body_pnl);
	wx_hide_t hide_status(main_sizer, status_pnl);
	wx_hide_t hide_button_hline(main_sizer, buttom_hline);
	
	*flog << "lock";
	sg_clear(sg);
	order.reset();
	ksi_cafe::session.reset();
	current_user_lbShow(false);
	open_cash_drawer_btnShow(false);
	browse_delayed_orders_btnShow(false);
	vfd_clear_all(display.get());
	vfd_print(display.get(), 1, "next register please", "", 1);
	while (ksi_cafe::session.get() == 0)
	{
		bool successful = false;
		try {
			ksi_cafe::session.reset(wx_unlock(this));
			
			*flog << "registrer";
			try_register(app_id);
			std::vector<std::string> mac_vec = get_mac();	
			bool registered = false;
			for (size_t i=0; i<mac_vec.size(); ++i) {
				try {
					*flog << "cashier register";
					*flog << "try mac: " + mac_vec[i];
					try_cashier_register(mac_vec[i]);
					*flog << "successful";
					registered = true;
				}
				catch (...) {
				}
			}
			if (!registered) {
				THROW_MESSAGE(0, to_mb(locale->get("cashier_f", "terminal_not_register_msg", "This terminal is not registered in the shift or shift is not running")), "cashier_f::logout_btnClick( wxCommandEvent& )");
			}
			successful = true;
		}
		catch (exception_t& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()));
		}
		catch (ksi_client::db_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
		}
		catch (ksi_client::ksi_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
		}
		catch (std::exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
		}
		catch (...) {
			*flog << ERROR_IN_LOCATION;
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
			wxMessageBox(to_uc(ERROR_IN_LOCATION));
		}
		if (!successful) ksi_cafe::session.reset(0);
	}
	try {
		refresh_order_list();
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
	}
	if (delivery_type_ || pickup_type_) {
		browse_delayed_orders_btnShow(true);
	}
	else browse_delayed_orders_btnShow(false);
	open_cash_drawer_btnShow(true);
	current_user_lb->SetLabel(to_uc(users->user().role_name()) + _(" ") + to_uc(users->user().fio()));
	current_user_lbShow(true);
	*flog << "Current USER = " + ksi_cafe::session->username();
	*flog << "unlock";
	vfd_print(display.get(), 1, vfd_line1, "", vfd_align_line1);
	vfd_print(display.get(), 2, vfd_line2, "", vfd_align_line2);
	resume_wait();
}

void cashier_f::delayed_order_details_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	int type_id = 0;
	
	if (order_type_toggle_group_ptr->current() == delivery_btn) {
		type_id = 3;
	}
	else if (order_type_toggle_group_ptr->current() == pickup_btn) {
		type_id = 4;
	}
	else return;
	
	wxString postfix = _(" order");
	if (order->order_id()) postfix = _(" order # ") + to_uc(boost::lexical_cast<std::string>(order->order_id()));
	sg_clear(sg);
	wx_window_ptr<delayed_order_f> form (new delayed_order_f(this, locale->get("global", "delayed_order_details_str1", "Delayed ") + postfix + locale->get("global", "delayed_order_details_str2", " details"), type_id, true, order.release()));
	try {
		int result = form->ShowModal();
		order.reset(form->release());
		if (result == wxID_OK) {
			*flog << "save_delay_order";
			recalc_coupon("", "");
		}
		else if (result == wxID_CLOSE) {
			*flog << "close_delay_order";
			resume_wait();
		}
		else if (result == wxID_UNDO) {
			*flog << "cancel_delay_order";
			resume_wait();
		}
		else recalc_coupon("", "");
		save_and_hide_btn->Enable(order->type_delivery() || order->type_pickup());
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}
	catch (exception_t& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}

void cashier_f::browse_delayed_orders_btnShow(bool show) {
	bool old_val = open_cash_drawer_btn->IsShown();
	open_cash_drawer_btnShow(false);
	browse_delayed_orders_btn->Show(show);
	open_cash_drawer_btnShow(old_val);
}

void cashier_f::open_cash_drawer_btnShow(bool show) {
	bool old_val = current_user_lb->IsShown();
	current_user_lbShow(false);
	open_cash_drawer_btn->Show(show);
	current_user_lbShow(old_val);
}

void cashier_f::current_user_lbShow(bool show) {
	current_user_lb->Show(show);
	top_bar_sizer->Layout();
	//top_pnl->Layout();
}

void cashier_f::refresh_order_list() {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select count(order_id) \
	from ksi.orders \
	where status_id in (1,2,6) and orders.cafe_id = ksi.util.get_cur_cafe_id \
	and orders.type_id in (3, 4)"
	);
	try {
		query->execute_statement();
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		logout();
		return;
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
		return;
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		return;
	}
	if (query->next()) THROW_LOCATION(0, "void cashier_f::refresh_order_list()");
	delayed_order_count_ = static_cast<int>(boost::get<double>(query->get(0)));
	browse_delayed_orders_btn->SetLabel(locale->get("cashier_f", "browse_delayed_orders_btn", "Browse delayed orders") + _(" (" + to_uc(boost::lexical_cast<std::string>(delayed_order_count_)) +_(")")));
	browse_delayed_orders_btn->Enable(delayed_order_count_);
}

void cashier_f::logout() {
	wxCommandEvent event;
	logout_btnClick(event);
}

void cashier_f::save_and_hide_btnClick( wxCommandEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	pause_timer_t<cashier_f> pause_lock_timer(lock_timer_);
	try {
		if (order->real_dish_count() == 0) order->sync_order_created();
		order->change_table(ksi_cafe::table_t(order->table().table_group_id(), order->table().table_group_name()));
		order->set_accept(0, true);
		int order_id = order->order_id();
		*flog << std::string("order commit. order_id = ") + boost::lexical_cast<std::string>(order_id);
		if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
		*flog << "commit successful"; 
		resume_wait();
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
}

void cashier_f::apply_lang() {
	logout_btn->SetLabel(locale->get("cashier_f", "logout_btn", "Log out"));
	browse_delayed_orders_btn->SetLabel(locale->get("cashier_f", "browse_delayed_orders_btn", "Browse delayed orders") + _(" (" + to_uc(boost::lexical_cast<std::string>(delayed_order_count_)) +_(")")));
	open_cash_drawer_btn->SetLabel(locale->get("cashier_f", "open_cash_drawer_btn", "Open cash drawer"));
	top_bar_sizer->Layout();
	
	sg->GetColumn(0).SetText(locale->get("cashier_f", "sg_entree", "Entree"));
	sg->GetColumn(1).SetText(locale->get("cashier_f", "sg_price", "Price"));
	sg->GetColumn(2).SetText(locale->get("cashier_f", "sg_qty", "Qty"));
	sg->GetColumn(3).SetText(locale->get("cashier_f", "sg_amount", "Amount"));
	
	m_subtotal_lb->SetLabel(locale->get("cashier_f", "m_subtotal_lb", "Subtotal:"));
	m_sales_tax_lb->SetLabel(locale->get("cashier_f", "m_sales_tax_lb", "Sales tax:"));
	m_total_lb->SetLabel(locale->get("cashier_f", "m_total_lb", "Total: "));
	
	comment_entree_btn->SetLabel(locale->get("cashier_f", "comment_entree_btn", "Comment entree"));
	wrap_label(comment_entree_btn);
	for_here_btn->SetLabel(locale->get("cashier_f", "for_here_btn", "For here"));
	wrap_label(for_here_btn);
	to_go_btn->SetLabel(locale->get("cashier_f", "to_go_btn", "To go"));
	wrap_label(to_go_btn);
	delivery_btn->SetLabel(locale->get("cashier_f", "delivery_btn", "Delivery"));
	wrap_label(delivery_btn);
	pickup_btn->SetLabel(locale->get("cashier_f", "pickup_btn", "Pickup"));
	wrap_label(pickup_btn);
	employee_meal_btn->SetLabel(locale->get("cashier_f", "employee_meal_btn", "Employee meal"));
	wrap_label(employee_meal_btn);
	donation_btn->SetLabel(locale->get("cashier_f", "donation_btn", "Donation"));
	wrap_label(donation_btn);

	misc_charges_btn->SetLabel(locale->get("cashier_f", "misc_charges_btn", "Misc charges"));
	wrap_label(misc_charges_btn);
	entree_code_btn->SetLabel(locale->get("cashier_f", "entree_code_btn", "Entree code"));
	wrap_label(entree_code_btn);
	discount_btn->SetLabel(locale->get("cashier_f", "discount_btn", "Discounts"));
	wrap_label(discount_btn);
	if (order.get() && order->token().exist())
		token_btn->SetLabel(to_uc(order->token().name()));
	else 
		token_btn->SetLabel(locale->get("cashier_f", "token_btn1", "Token")+_(": ")+locale->get("cashier_f", "token_btn2", "not selected"));
	wrap_label(token_btn);

	save_and_hide_btn->SetLabel(locale->get("cashier_f", "save_and_hide_btn", "Save and hide order"));
	wrap_label(save_and_hide_btn);
	
	delayed_order_details_btn->SetLabel(locale->get("cashier_f", "delayed_order_details_btn", "Delayed order details"));
	wrap_label(delayed_order_details_btn);
	refund_order_btn->SetLabel(locale->get("cashier_f", "refund_order_btn", "Closed orders operations"));
	wrap_label(refund_order_btn);
	shift_btn->SetLabel(locale->get("cashier_f", "shift_btn", "Seances"));
	wrap_label(shift_btn);
	
	if (open_cash_advance_count_ > 0) {
		change_bg_colour(cash_advance_btn, wxColour(255, 0, 0));
		cash_advance_btn->SetLabel(locale->get("cashier_f", "cash_advance_btn1", "Cash advance") + _(" (") + to_uc(boost::lexical_cast<std::string>(open_cash_advance_count_)) + _(")") + locale->get("cashier_f", "cash_advance_btn2", " open"));
		wrap_label(cash_advance_btn);
	}
	else {
		change_bg_colour(cash_advance_btn, btn_face_color);
		cash_advance_btn->SetLabel(locale->get("cashier_f", "cash_advance_btn1", "Cash advance"));
		wrap_label(cash_advance_btn);
	}
	
	process_btn->SetLabel(locale->get("cashier_f", "process_btn", "Process"));
}


