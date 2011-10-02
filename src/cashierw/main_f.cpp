
#include <wx/app.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include "main_f.h"
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
#include <shared_modules/cashier_f.h>
#include <ksi_include/algo_functor.hpp>
#include <boost/shared_ptr.hpp>
#include <shared_modules/split_order_f.h>
#include <shared_modules/dlg_f.h>

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
std::auto_ptr<ksi_client::user_pos_i> client;
extern std::auto_ptr<file_log_t> flog;
std::auto_ptr<file_log_t> thflog;
std::auto_ptr<com_t> com;
std::auto_ptr<base_printer_t> printer;
extern std::auto_ptr<xml_config_t> config;
std::auto_ptr<drawer_t> drawer;
std::auto_ptr<customer_display_t> display;
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

void main_f::btn_repaint() {
	//const int hot_col_size = 10;
	//const int hot_row_size = 10;
	const int btn_width = 126;
	const int btn_height = 126;
	const int btn_space = 1;
	int client_height, client_width;
	btn_pnl->GetClientSize(&client_width, &client_height);
	
	for (size_t i=0; i<table_vec_.size(); ++i) {
		table_vec_[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( main_f::tableClick ), NULL, this );
		table_vec_[i]->Hide();
	}
	table_vec_.clear();

	int h_pos = 0;
	for (;;) {
		int cnt = 0;
		if (h_pos + btn_height > client_height) break;
		for (;;) {
			if (client_width <  cnt * ( btn_width + btn_space ) + btn_width) break;
			wxButton* btn;
			btn = new wxButton( btn_pnl, wxID_ANY, _(""), wxPoint(cnt*(btn_width+btn_space), h_pos), wxSize( btn_width, btn_height ), 0);
			btn->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Arial Narrow") ) );
			//btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
			btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( main_f::tableClick ), NULL, this );
			table_vec_.push_back(btn);
			++cnt;
		}
		h_pos += btn_height + btn_space;
		if (h_pos + btn_height > client_height) break;
	}
	int level_ = 0; 
	if (table_show_.get()) level_ = table_show_->level();
	table_show_.reset(new table_show_t(table_vec_, table_container.get(), content::REGULAR));
	table_show_->down(level_);
}

main_f::main_f( wxWindow* parent )
: base_main_f( parent )
, timer_(obj_handler_t<main_f>(this, &main_f::OnTimer))
, seance_timer_(obj_handler_t<main_f>(this, &main_f::OnSeanceTimer))
, lock_timer_(obj_handler_t<main_f>(this, &main_f::OnLockTimer))
{
	delayed_order_count_ = 0;
	open_cash_advance_count_ = 0;
	open_order_count_ = 0;
	ksi_cafe::pos_version = boost::lexical_cast<std::string>(app_id) + "-" + version::version;
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	SetTitle(to_uc(app_name));
	current_dt_lb->SetLabel(_("                   "));
	
	if (config->get("null", "trainig_mode") == "1" || to_uc(config->get("null", "trainig_mode")).MakeUpper() == _("ON")) hide_mode_ = true;
	else hide_mode_ = false;

	if (config->get("resolution", "auto") == "1" || to_uc(config->get("resolution", "auto")).MakeUpper() == _("ON")) {
		wxScreenDC screen;
		SetSize(screen.GetSize().GetWidth(), screen.GetSize().GetHeight());
		this->Move(0, 0);
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
			this->Move(0, 0);
		}
	}
	if (config->get("app", "auto_lock") == "1" || to_uc(config->get("app", "auto_lock")).MakeUpper() == _("ON"))
		auto_lock_ = true;
	else auto_lock_ = false;

	*flog << "registrer";
	try_register(app_id);
	std::vector<std::string> mac_vec = get_mac();	
	bool registered = false;
	for (size_t i=0; i<mac_vec.size(); ++i) {
		if (registered) continue;
		try {
			*flog << "cashier register";
			*flog << "try mac: " + mac_vec[i];
			try_cashierW_register(mac_vec[i]);
			*flog << "successful";
			registered = true;
		}
		catch (exception_t& ex) {
			*flog << ex.what();
			if (i+1 >= mac_vec.size()) {
				throw;
			}
			else if (ex.code() != 2) {
				throw;
			}
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			throw;
		}
	}
	if (!registered) {
		THROW_MESSAGE(0, to_mb(locale->get("cashier_f", "terminal_not_register_msg", "This terminal is not registered in the shift or shift is not running")), "main_f::main_f( wxWindow* )");
	}
	*flog << "register successful";

	*flog << "create ksi_cafe::users_t";
	users.reset(new ksi_cafe::users_t());
	*flog << "ksi_cafe::menu_t::container_load()";
	ksi_cafe::menu_t::container_load();
	*flog << "create ksi_cafe::cafe_container_t";
	cafe_container.reset(new ksi_cafe::cafe_container_t());

	try {
		drawer.reset(new drawer_t());
	}
	catch (std::exception& ex) {
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
			com.reset(new com_t(config->get("printer", "port"), boost::lexical_cast<int>(config->get("printer", "baud_rate"))));
			if (to_uc(config->get("printer", "command_set")).MakeUpper() == _("STAR")) printer.reset(new star_printer_t(*com.get()));
			else if (to_uc(config->get("printer", "command_set")).MakeUpper() == _("EPSON")) printer.reset(new epson_printer_t(*com.get()));
			else com.reset();
		}
		else {
			printer.reset(new null_printer_t());
		}
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
	if (printer.get() == 0) printer.reset(new null_printer_t());
	if (config->get("reciept", "group_by_kitchen_id") == "1" || to_uc(config->get("reciept", "group_by_kitchen_id")).MakeUpper() == _("ON"))
		by_group = true;
	else
		by_group = false;
	
	*flog << "create ksi_cafe::menu_container_t";
	menu_container.reset(new ksi_cafe::menu_container_t());
	*flog << "create ksi_cafe::coupon_storage_t";
	coupon_storage.reset(new ksi_cafe::coupon_storage_t());
	int curr_cafe_id = cafe_container->current_cafe()->cafe_id();
	*flog << "curr_cafe_id = " + boost::lexical_cast<std::string>(curr_cafe_id);
	std::vector<ksi_cafe::coupon_t*> coupon_for_cafe = coupon_storage->coupon_for_cafe(curr_cafe_id);
	*flog << "create ksi_cafe::coupon_in_cafe_t";
	coupon_in_cafe.reset(new ksi_cafe::coupon_in_cafe_t(coupon_for_cafe));
	*flog << "crate ksi_cafe::table_container_t";
	table_container.reset(new ksi_cafe::table_container_t());

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
		catch (std::exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(_("Unable to initialize COM4 port. VFD display will be disabled"), _("Message"), wxOK, this);
		}
		catch (...) {
			*flog << "Unable to initialize COM4 port. VFD display will be disabled";
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, "Unable to initialize COM4 port. VFD display will be disabled"));
			wxMessageBox(_("Unable to initialize COM4 port. VFD display will be disabled"), _("Message"), wxOK, this);
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
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
	if (!tested_client) client.reset(0);

	try {
		expense_list.reset(new ksi_cafe::expense_list_t(*cafe_container->current_cafe(), 0));
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
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
	current_user_lb->SetLabel(to_uc(users->user().role_name()) + _(" ") + to_uc(users->user().fio()));
	apply_lang();
	needLock = false;
	resume_wait();
	needLock = true;
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
}

void main_f::tableClick( wxCommandEvent& event ) {
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	needLock = true;
	*flog << "main_f::tableClick";
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));
	
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	btn_status::status_t cur_status = table_show_->get_status(btn);
	if (cur_status == btn_status::prev) {
		table_show_->left();
	}
	else if (cur_status == btn_status::next) {
		table_show_->right();
	}
	else if (cur_status == btn_status::table) {
		wxString rollBackLabel = room_lb->GetLabel();
		//boost::function<void ()> func = boost::bind(&wxStaticText::SetLabel, room_lb, rollBackLabel);
		destroyProcessor_t<boost::function<void ()> > rollBackRoomLabel(boost::function<void ()>(boost::bind(&wxStaticText::SetLabel, room_lb, rollBackLabel)));
		ksi_cafe::table_t table = table_show_->get_table(btn);
		room_lb->SetLabel(_(" ") + to_uc(table.table_group_name()) + _(" / ") + to_uc(table.name()));
		top_sizer->Layout();
		std::vector<wxString> cap_vec;
		std::vector<wxColour> color_vec;
		std::vector<bool> checked_vec;
		std::vector<bool> enabled_vec;
		std::vector<bool> wrap_vec;
		
		size_t orderCount = 0;
		{
			std::string sqlStr = "";
			if (users->user().manager() || users->user().superwaiter()) {
				sqlStr = "SELECT COUNT(*) "
				" FROM KSI.ORDERS "
				" INNER JOIN KSI.SEANCE ON SEANCE.SEANCE_ID = ORDERS.SEANCE_ID "
						" AND SEANCE.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
						" AND SEANCE.SHIFT_ID = KSI.UTIL.GET_CUR_SHIFT_ID "
				" WHERE STATUS_ID IN (1,2,6) AND ORDERS.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
				" AND TABLES=:tables AND TABLE_GROUP_ID = :table_group_id";
			}
			else {
				sqlStr = "SELECT COUNT(*) "
				" FROM KSI.ORDERS "
				" INNER JOIN KSI.SEANCE ON SEANCE.SEANCE_ID = ORDERS.SEANCE_ID "
						" AND SEANCE.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
						" AND SEANCE.SHIFT_ID = KSI.UTIL.GET_CUR_SHIFT_ID "
				" WHERE STATUS_ID IN (1,2,6) AND ORDERS.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
				" AND CREATOR_ID = KSI.UTIL.GET_CUR_USER_ID "
				" AND TABLES=:tables AND TABLE_GROUP_ID = :table_group_id";
			}
			try {
				std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
				query->create_statement(sqlStr);
				query->set("tables", table.name());
				query->set("table_group_id", table.table_group_id());
				query->execute_statement();
				orderCount = static_cast<size_t>(boost::get<double>(query->get(0)));
			}
			catch (ksi_client::connection_problem_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				logout();
				return;
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				return;
			}
		}

		if (config->get("functionality", "order_type_for_here") == "0" || to_uc(config->get("functionality", "order_type_for_here")).MakeUpper() == _("OFF")) {
		}
		else {
			cap_vec.push_back(locale->get("action_f", "new_order_btn", "Create new order"));
			color_vec.push_back(btn_face_color);
			checked_vec.push_back(false);
			enabled_vec.push_back(true);
			wrap_vec.push_back(true);
		}
		
		if (orderCount) {
			cap_vec.push_back(locale->get("action_f", "exist_order_btn", "Manage open orders") + _(" (") + to_uc(boost::lexical_cast<std::string>(orderCount)) + _(")"));
			color_vec.push_back(btn_face_color);
			checked_vec.push_back(false);
			enabled_vec.push_back(true);
			wrap_vec.push_back(true);
		}
		if (orderCount) {
			cap_vec.push_back(locale->get("action_f", "split_order_btn", "Split order") + _(" (") + to_uc(boost::lexical_cast<std::string>(orderCount)) + _(")"));
			color_vec.push_back(btn_face_color);
			checked_vec.push_back(false);
			enabled_vec.push_back(true);
			wrap_vec.push_back(true);
		}			
		if (orderCount) {
			cap_vec.push_back(locale->get("action_f", "move_order_btn", "Transfer order to another table") + _(" (") + to_uc(boost::lexical_cast<std::string>(orderCount)) + _(")"));
			color_vec.push_back(btn_face_color);
			checked_vec.push_back(false);
			enabled_vec.push_back(true);
			wrap_vec.push_back(true);
		}
		if (orderCount) {
			cap_vec.push_back(locale->get("action_f", "move_person_btn", "Transfer order to another waiter") + _(" (") + to_uc(boost::lexical_cast<std::string>(orderCount)) + _(")"));
			color_vec.push_back(btn_face_color);
			checked_vec.push_back(false);
			enabled_vec.push_back(true);
			wrap_vec.push_back(true);
		}
		
		if (cap_vec.size() == 0) return;
		
		wxString caption = locale->get("action_f", "caption1", "table ") + to_uc(table.name()) + locale->get("action_f", "caption2", " operations with orders");
		wx_window_ptr<btn_list_f> form (new btn_list_f(this, caption, _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
		int modal_code = form->ShowModal() - wxID_HIGHEST;
		if (modal_code == 0) { // create new order
			*flog << "create new order";
			try {
				std::auto_ptr<ksi_cafe::order_t> order (new ksi_cafe::order_t(*cafe_container->current_cafe(), table, 0, false, false));
				order->set_guest_count(1);
				wx_hide_t hide_main(this);
				wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
				int res = form->ShowModal();
				hide_main.force();
				if (res == wxID_HIGHEST+1) {
					browse_delayed_orders_btnClick(event);
				}
				else if (res == wxID_HIGHEST+2) {
					open_orders_btnClick(event);
				}
				else if (res == wxID_HIGHEST+3) {
					logout();
				}
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			}
		}
		else if (modal_code == 1 || modal_code == 2 || modal_code == 3 || modal_code == 4) {
			std::vector<wxString> cap_vec;
			std::vector<wxColour> color_vec;
			std::vector<bool> checked_vec;
			std::vector<bool> enabled_vec;
			std::vector<bool> wrap_vec;
			std::vector<int> order_id_vec;
			std::string sqlStr = "";
			if (users->user().manager() || users->user().superwaiter()) {
				sqlStr = "SELECT ORDER_ID, ORDER_TYPE.NAME, SUBTOTAL, CREATOR_ID, "
				" CREATED, GUEST_COUNT, ORDER_STATUS.NAME "
				" FROM KSI.ORDERS "
				" INNER JOIN KSI.ORDER_TYPE ON ORDER_TYPE.TYPE_ID = ORDERS.TYPE_ID "
				" INNER JOIN KSI.ORDER_STATUS ON ORDER_STATUS.STATUS_ID = ORDERS.STATUS_ID "
				" INNER JOIN KSI.SEANCE ON SEANCE.SEANCE_ID = ORDERS.SEANCE_ID "
						" AND SEANCE.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
						" AND SEANCE.SHIFT_ID = KSI.UTIL.GET_CUR_SHIFT_ID "
				" WHERE STATUS_ID IN (1,2,6) AND ORDERS.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
				" AND TABLES=:tables AND TABLE_GROUP_ID = :table_group_id";
			}
			else {
				sqlStr = "SELECT ORDER_ID, ORDER_TYPE.NAME, SUBTOTAL, CREATOR_ID, "
				" CREATED, GUEST_COUNT, ORDER_STATUS.NAME "
				" FROM KSI.ORDERS "
				" INNER JOIN KSI.ORDER_TYPE ON ORDER_TYPE.TYPE_ID = ORDERS.TYPE_ID "
				" INNER JOIN KSI.ORDER_STATUS ON ORDER_STATUS.STATUS_ID = ORDERS.STATUS_ID "
				" INNER JOIN KSI.SEANCE ON SEANCE.SEANCE_ID = ORDERS.SEANCE_ID "
						" AND SEANCE.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
						" AND SEANCE.SHIFT_ID = KSI.UTIL.GET_CUR_SHIFT_ID "
				" WHERE STATUS_ID IN (1,2,6) AND ORDERS.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
				" AND CREATOR_ID = KSI.UTIL.GET_CUR_USER_ID "
				" AND TABLES=:tables AND TABLE_GROUP_ID = :table_group_id";
			}
			try {
				std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
				query->create_statement(sqlStr);
				query->set("tables", table.name());
				query->set("table_group_id", table.table_group_id());
				query->execute_statement();
				while (!query->next()) {
					int order_id = static_cast<int>(boost::get<double>(query->get(0)));
					std::string typeName = boost::get<std::string>(query->get(1));
					double subtotal = boost::get<double>(query->get(2));
					int creator_id = static_cast<int>(boost::get<double>(query->get(3)));
					boost::posix_time::ptime created = boost::get<boost::posix_time::ptime>(query->get(4));
					int guestCount = static_cast<int>(boost::get<double>(query->get(5)));
					std::string statusName = boost::get<std::string>(query->get(6));

					std::map<std::string, std::string> mapper;
					mapper["endl"] = "\n";
					mapper["null"] = "";
					mapper["order_id"] = boost::lexical_cast<std::string>(order_id);
					mapper["order_status"] = statusName;
					mapper["creator"] = users->user(creator_id).fio();
					mapper["created"] = to_mb(locale->get_date_time(created));
					mapper["type_name"] = typeName;
					mapper["gross"] = to_mb(locale->get_currency(subtotal));
					mapper["guest_count"] = boost::lexical_cast<std::string>(guestCount);
		
					std::string cap = to_mb(locale->get_string("order_list", mapper));
					
					if (cap == "") {
						cap = "Order # " + boost::lexical_cast<std::string>(order_id) + " " + statusName + "\n"
						+ "Created by " + users->user(creator_id).fio() + "\n"
						+ "Accepted on " + to_mb(locale->get_date_time(created))+"\n"
						+ "Guests: " + boost::lexical_cast<std::string>(guestCount)
						+ ", Subtotal: " + to_mb(locale->get_currency(subtotal));
						/*cap = _("Order # ") + to_uc(boost::lexical_cast<std::string>(order_id)) + _(" ") + to_uc(typeName) + _(" ") + to_uc(statusName) + _("\n")
						+ _("\n")
						+ _("Being served by ") + to_uc(users->user(creator_id).fio()) + _("\n")
						+ _("Accepted on ") + locale->get_date_time(created) + _("\n")
						+ _("Guests: ") + to_uc(boost::lexical_cast<std::string>(guestCount)) + _(", Subtotal: ") + locale->get_currency(subtotal);*/
					}

					cap_vec.push_back(to_uc(cap));
					color_vec.push_back(btn_face_color);
					checked_vec.push_back(false);
					enabled_vec.push_back(true);
					wrap_vec.push_back(false);
					order_id_vec.push_back(order_id);
				}
			}
			catch (ksi_client::connection_problem_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				logout();
				return;
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				return;
			}
		
			wxString caption = locale->get("order_list_f", "caption1", "table ") + to_uc(table.name()) + locale->get("order_list_f", "caption2", " order list");
			wx_window_ptr<btn_list_f> form (new btn_list_f(this, caption, _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
			int result = form->ShowModal() - wxID_HIGHEST;
			if (result >= 0 && result < order_id_vec.size()) {
				int order_id = order_id_vec[result];
				bool catch_exception = false;
				int res = 0;
				ksi_cafe::order_t* ord_ptr = 0;
				try {
					*flog << "load order_t, order_id = " + boost::lexical_cast<std::string>(order_id);
					wx_window_ptr<load_f> form (new load_f(this, locale->get("global", "order_load_str", "Order is loading"), locale->get("global", "please_wait_str", "Please wait.\nIf order is not opened for a long time, it might have been already opened by somebody else.")));
					form->start(boost::bind(load_order2, &ord_ptr, *cafe_container->current_cafe(), order_id, true, false));
					res = form->ShowModal();
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
					if (ord_ptr) {
						*flog << "delete ksi_cafe::order_t";
						delete ord_ptr;
						*flog << "successful";
						ord_ptr = 0;
					}
					catch_exception = true;
				}
				
				std::auto_ptr<ksi_cafe::order_t> order;
				if (res == wxID_OK) {
					if (ord_ptr) {
						*flog << "loaded successful, ksi_cafe::order_t::status_id = " + boost::lexical_cast<std::string>(ord_ptr->status_id());
						order.reset(ord_ptr);
					}
				}
				else {
					*flog << "delete ksi_cafe::order_t";
					delete ord_ptr;
					*flog << "successful";
					ord_ptr = 0;
				}
				if (order.get()) {
				}
				else return;

				if (modal_code == 1) { // Manage order
					*flog << "Manage order, order# " + boost::lexical_cast<std::string>(order->order_id());
					try {
						wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
						wx_hide_t hide_main(this);
						int res = form->ShowModal();
						hide_main.force();
						if (res == wxID_HIGHEST+1) {
							browse_delayed_orders_btnClick(event);
						}
						else if (res == wxID_HIGHEST+2) {
							open_orders_btnClick(event);
						}
						else if (res == wxID_HIGHEST+3) {
							logout();
						}
					}
					catch (std::exception& ex) {
						*flog << ex.what();
						wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
						return;
					}
				}
				else if (modal_code == 2) { // split order
					*flog << "split_order";
					try {
						wx_window_ptr<split_order_f> form (new split_order_f(this, _("Split order"), order.get()));
						int result = form->ShowModal();
					}
					catch (std::exception& ex) {
						*flog << ex.what();
						wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
						return;
					}
				}
				else if (modal_code == 3) { // move to another table
					std::auto_ptr<table_show_t> table_show (new table_show_t(std::vector<wxButton*>(), table_container.get(), content::REGULAR, true));
					wx_window_ptr<btn_list_f> form (new btn_list_f(this, locale->get("global", "select_a_table_str", "Select a table"), _(""), table_show.get()));
					int result = form->ShowModal() - wxID_HIGHEST;
					if (result >= 0) {
						wxButton* table_btn = form->getTableBtn();
						ksi_cafe::table_t table = table_show->get_table(table_btn);
						order->change_table(table);
						try {
							order->set_accept(0, true);
							*flog << "ksi_cafe::order_t::commit";
							if (!order->commit(true)) *flog << "!!! force apply";
							*flog << "successuful";
						}
						catch (exception_t& ex) {
							*flog << ex.comp_message();
							bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
						}
						catch (ksi_client::connection_problem_exception& ex) {
							*flog << ex.what();
							bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
							logout();
							return;
						}
						catch (ksi_client::db_exception& ex) {
							*flog << ex.what();
							bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
						}	
						catch (std::exception& ex) {
							*flog << ex.what();
							bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
						}
					}
				}
				else if (modal_code == 4) { // move to another person
					std::vector<ksi_cafe::user_t> user_vec = users->get_all();
					std::vector<wxString> cap_vec; std::vector<wxColour> color_vec; std::vector<bool> enabled_vec; std::vector<bool> checked_vec; std::vector<bool> wrap_vec;
					for (size_t i=0; i<user_vec.size(); ++i) {
						cap_vec.push_back(to_uc(user_vec[i].fio()));
						color_vec.push_back(btn_face_color);
						enabled_vec.push_back(true);
						checked_vec.push_back(false);
						wrap_vec.push_back(true);
					}
					wx_window_ptr<btn_list_f> form (new btn_list_f(this, _(""), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
					int result = form->ShowModal() - wxID_HIGHEST;
					if (result >= 0 && result < user_vec.size()) {
						order->change_creator_id(user_vec[result].user_id());
						try {
							order->set_accept(0, true);
							*flog << "ksi_cafe::order_t::commit";
							if (!order->commit(true)) *flog << "!!! force apply";
							*flog << "successuful";
						}
						catch (exception_t& ex) {
							*flog << ex.comp_message();
							bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
						}
						catch (ksi_client::connection_problem_exception& ex) {
							*flog << ex.what();
							bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
							logout();
							return;
						}
						catch (ksi_client::db_exception& ex) {
							*flog << ex.what();
							bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
						}	
						catch (std::exception& ex) {
							*flog << ex.what();
							bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
							wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
						}
					}
				}
			}
			else return;
		}
	}
	else if (cur_status == btn_status::table_group) {
		ksi_cafe::table_t table_group = table_show_->get_table_group(btn);
		room_lb->SetLabel(_(" ") + to_uc(table_group.table_group_name()));
		table_show_->down(table_group.table_group_id());
		top_sizer->Layout();
	}
	else if (cur_status == btn_status::parent) {
		table_show_->up();
		room_lb->SetLabel(_(" "));
		top_sizer->Layout();
	}
}

void main_f::browse_delayed_orders_btnClick( wxCommandEvent& event ) {
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement browse_delayed_orders_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	needLock = true;
	*flog << "main_f::browse_delayed_orders_btnClick";
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));
	
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	std::vector<wxString> cap_vec;
	std::vector<wxColour> color_vec;
	std::vector<bool> enabled_vec;
	std::vector<bool> checked_vec;
	std::vector<bool> wrap_vec;
	std::vector<int> order_id_vec;
	std::string sqlStr = "SELECT "
	" ORDER_ID, "
	" ORDERS.TYPE_ID, "
	" ORDER_TYPE.NAME, "
	" SUBTOTAL, "
	" CREATED, "
	" TABLES, "
	" ORDERS.TABLE_GROUP_ID, "
	" TABLE_GROUP.NAME, "
	" ORDER_DELIVERY.DELIVERY_DATE, "
	" (ORDER_DELIVERY.FIRSTNAME || ' ' || ORDER_DELIVERY.LASTNAME) AS fio, "
	" ORDER_STATUS.NAME, "
	" ORDERS.CREATOR_ID, "
	" ORDER_DELIVERY.STREETADDRESS, "
	" ORDER_DELIVERY.CITY, "
	" STATE.SHORTNAME, "
	" ORDER_DELIVERY.ZIP, "
	" ORDERS.PREPAYMENT_TYPE_ID, "
	" PREPAYMENT_TYPE.NAME, "
	" ORDERS.PREPAYMENT, "
	" (SUBTOTAL + SALES_TAX + GRATUITY) AS GROSS, "
	" GUEST_COUNT "
	" FROM KSI.ORDERS "
	" INNER JOIN KSI.TABLE_GROUP ON TABLE_GROUP.TABLE_GROUP_ID = ORDERS.TABLE_GROUP_ID "
					" AND TABLE_GROUP.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
	" INNER JOIN KSI.ORDER_TYPE ON ORDER_TYPE.TYPE_ID = ORDERS.TYPE_ID "
	" INNER JOIN KSI.ORDER_DELIVERY ON ORDERS.CAFE_ID = ORDER_DELIVERY.CAFE_ID "
					" AND ORDERS.DELIVERY_ID = ORDER_DELIVERY.DELIVERY_ID "
	" LEFT JOIN KSI.STATE ON STATE.STATE_ID = ORDER_DELIVERY.STATE_ID "
	" INNER JOIN KSI.ORDER_STATUS ON ORDER_STATUS.STATUS_ID = ORDERS.STATUS_ID "
	" LEFT JOIN KSI.PAYMENT_TYPE PREPAYMENT_TYPE ON PREPAYMENT_TYPE.PAYMENT_TYPE_ID = ORDERS.PREPAYMENT_TYPE_ID "
	" WHERE STATUS_ID IN (1,2,6) AND ORDERS.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
	" AND ORDERS.TYPE_ID IN (3, 4, 6) "
	" ORDER BY ORDERS.ORDER_ID";

	query->create_statement(sqlStr);
	try {
		query->execute_statement();
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
		return;
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		return;
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
		boost::posix_time::ptime delivery_date = boost::get<boost::posix_time::ptime>(query->get(8));
		std::string customer_fio = boost::get<std::string>(query->get(9));
		std::string order_status = boost::get<std::string>(query->get(10));
		int creator_id = static_cast<int>(boost::get<double>(query->get(11)));
		std::string streetaddress = boost::get<std::string>(query->get(12));
		std::string city = boost::get<std::string>(query->get(13));
		std::string state = boost::get<std::string>(query->get(14));
		int zip = static_cast<int>(boost::get<double>(query->get(15)));
		int prepayment_type_id = static_cast<int>(boost::get<double>(query->get(16)));
		std::string prepayment_type_name = boost::get<std::string>(query->get(17));
		double prepayment = boost::get<double>(query->get(18));
		double gross = boost::get<double>(query->get(19));
		int guest_count = static_cast<int>(boost::get<double>(query->get(20)));
		std::string delivery_address = streetaddress + " " + city + " " + state + " " + boost::lexical_cast<std::string>(zip);
		
		
		std::map<std::string, std::string> mapper;
		mapper["endl"] = "\n";
		mapper["null"] = "";
		mapper["order_id"] = boost::lexical_cast<std::string>(order_id);
		mapper["order_status"] = order_status;
		mapper["creator"] = users->user(creator_id).fio();
		mapper["created"] = to_mb(locale->get_date_time(created));
		mapper["subtotal"] = to_mb(locale->get_currency(subtotal));
		mapper["prepayment_type_id"] = boost::lexical_cast<std::string>(prepayment_type_id);
		mapper["prepayment"] = to_mb(locale->get_currency(prepayment));
		mapper["type_id"] = boost::lexical_cast<std::string>(type_id);
		mapper["delivery_date"] = to_mb(locale->get_date_time(delivery_date));
		mapper["customer_fio"] = customer_fio;
		mapper["type_name"] = type_name;
		mapper["table_name"] = table_name;
		mapper["table_group_id"] = table_group_id;
		mapper["table_group_name"] = table_group_name;
		mapper["streetaddress"] = streetaddress;
		mapper["city"] = city;
		mapper["state"] = state;
		mapper["zip"] = boost::lexical_cast<std::string>(zip);
		mapper["prepayment_type_name"] = prepayment_type_name;
		mapper["gross"] = to_mb(locale->get_currency(gross));
		mapper["guest_count"] = boost::lexical_cast<std::string>(guest_count);
		
		std::string cap = to_mb(locale->get_string("delayed_order_list", mapper));
		if (cap == "") {
			std::string prepayment_str = "";
			if (prepayment_type_id != 0) prepayment_str = ", prepayment " + to_mb(locale->get_currency(prepayment));
			cap = "Order # " + boost::lexical_cast<std::string>(order_id) + " " + order_status + "\n"
			+ "Created by " + users->user(creator_id).fio() + "\n"
			+ "Accepted on " + to_mb(locale->get_date_time(created))+"\n"
			+ "Subtotal: " + to_mb(locale->get_currency(subtotal)) + prepayment_str + "\n";
			std::string tmp_cap;
			if (type_id == 3) {
				tmp_cap = "Delivery on " + to_mb(locale->get_date_time(delivery_date)) + "\n to " + customer_fio + "\n" + delivery_address;

			}
			else if (type_id == 4) {
				tmp_cap = "Pick up on " + to_mb(locale->get_date_time(delivery_date)) + "\n to " + customer_fio;

			}
			else if (type_id == 6) {
				tmp_cap = "Booked to " + to_mb(locale->get_date_time(delivery_date)) + "\n for " + customer_fio;
			}
			cap += tmp_cap;
		}
		cap_vec.push_back(to_uc(cap));

		if (type_id == 3) color_vec.push_back(table_type_delivery_color);
		else if (type_id == 4) color_vec.push_back(table_type_pickup_color);
		else if (type_id == 6) color_vec.push_back(table_type_table_booking_color);
		else color_vec.push_back(close_color);
		
		enabled_vec.push_back(true);
		checked_vec.push_back(false);
		wrap_vec.push_back(false);
		order_id_vec.push_back(order_id);
	}

	wx_window_ptr<btn_list_f> form (new btn_list_f(this, locale->get("global", "delayed_order_list_str", "Delayed orders list"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
	int result = form->ShowModal() - wxID_HIGHEST;
	if (result >= 0 && result < order_id_vec.size()) {
		int order_id = order_id_vec[result];
		std::auto_ptr<ksi_cafe::order_t> order;
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
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
				order.reset(ord_ptr);
			}
		}
		else {
			*flog << "delete ksi_cafe::order_t";
			delete ord_ptr;
			*flog << "successful";
			ord_ptr = 0;
		}
		if (order.get()) {
			if (order->type_table_booking()) {
				std::vector<wxString> cap_vec;
				cap_vec.push_back(_("Yes")); cap_vec.push_back(_("No"));
				std::vector<wxColour> color_vec;
				color_vec.push_back(ok_color); color_vec.push_back(close_color);
				wxString message = locale->get("global", "is_table_booking_started_being_used_str", "Is table booking started being used?");
				wx_window_ptr<dlg_f> dlg (new dlg_f(this, _("Confirmation"), message, cap_vec, color_vec));
		
				int dlgRes = dlg->ShowModal();
				if (dlgRes == wxID_HIGHEST) { // yes
					order->set_type_for_here(order->get_delivery_info()->clone());
				}
			}
			wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
			wx_hide_t hide_main(this);
			int res = form->ShowModal();
			hide_main.force();
			if (res == wxID_HIGHEST+1) {
				browse_delayed_orders_btnClick(event);
			}
			else if (res == wxID_HIGHEST+2) {
				open_orders_btnClick(event);
			}
			else if (res == wxID_HIGHEST+3) {
				logout();
			}
		}
		else {
			return;
		}
	}
}

void main_f::open_orders_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement open_orders_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	needLock = true;
	*flog << "main_f::open_orders_btnClick";
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));
	
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	std::vector<wxString> cap_vec;
	std::vector<wxColour> color_vec;
	std::vector<bool> enabled_vec;
	std::vector<bool> checked_vec;
	std::vector<bool> wrap_vec;
	std::vector<int> order_id_vec;
	std::string sqlStr;
	if (users->user().manager() || users->user().superwaiter()) {
		sqlStr = "SELECT "
		" ORDER_ID, "
		" ORDERS.TYPE_ID, "
		" ORDER_TYPE.NAME, "
		" SUBTOTAL, "
		" CREATED, "
		" GUEST_COUNT, "
		" TABLES, "
		" ORDERS.TABLE_GROUP_ID, "
		" TABLE_GROUP.NAME, "
		" ORDER_STATUS.NAME, "
		" CREATOR_ID, "
		" (SUBTOTAL + SALES_TAX + GRATUITY) AS GROSS "
		" FROM KSI.ORDERS "
		" INNER JOIN KSI.TABLE_GROUP ON TABLE_GROUP.TABLE_GROUP_ID = ORDERS.TABLE_GROUP_ID "
			" AND TABLE_GROUP.CAFE_ID= KSI.UTIL.GET_CUR_CAFE_ID "
		" INNER JOIN KSI.ORDER_TYPE ON ORDER_TYPE.TYPE_ID = ORDERS.TYPE_ID "
		" INNER JOIN KSI.ORDER_STATUS ON ORDER_STATUS.STATUS_ID = ORDERS.STATUS_ID "
		" INNER JOIN KSI.SEANCE ON SEANCE.SEANCE_ID = ORDERS.SEANCE_ID AND SEANCE.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
		" INNER JOIN KSI.SHIFT ON SHIFT.SHIFT_ID = SEANCE.SHIFT_ID AND SHIFT.SHIFT_ID = KSI.UTIL.GET_CUR_SHIFT_ID "
		" WHERE STATUS_ID IN (1,2,6) AND ORDERS.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID AND ORDERS.TYPE_ID NOT IN (3, 4, 6) "
		" ORDER BY ORDERS.ORDER_ID";
	}
	else {
		sqlStr = "SELECT "
		" ORDER_ID, "
		" ORDERS.TYPE_ID, "
		" ORDER_TYPE.NAME, "
		" SUBTOTAL, "
		" CREATED, "
		" GUEST_COUNT, "
		" TABLES, "
		" ORDERS.TABLE_GROUP_ID, "
		" TABLE_GROUP.NAME, "
		" ORDER_STATUS.NAME, "
		" CREATOR_ID, "
		" (SUBTOTAL + SALES_TAX + GRATUITY) AS GROSS "
		" FROM KSI.ORDERS "
		" INNER JOIN KSI.TABLE_GROUP ON TABLE_GROUP.TABLE_GROUP_ID = ORDERS.TABLE_GROUP_ID "
			" AND TABLE_GROUP.CAFE_ID= KSI.UTIL.GET_CUR_CAFE_ID "
		" INNER JOIN KSI.ORDER_TYPE ON ORDER_TYPE.TYPE_ID = ORDERS.TYPE_ID "
		" INNER JOIN KSI.ORDER_STATUS ON ORDER_STATUS.STATUS_ID = ORDERS.STATUS_ID "
		" INNER JOIN KSI.SEANCE ON SEANCE.SEANCE_ID = ORDERS.SEANCE_ID AND SEANCE.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
		" INNER JOIN KSI.SHIFT ON SHIFT.SHIFT_ID = SEANCE.SHIFT_ID AND SHIFT.SHIFT_ID = KSI.UTIL.GET_CUR_SHIFT_ID "
		" WHERE STATUS_ID IN (1,2,6) AND ORDERS.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID AND ORDERS.TYPE_ID NOT IN (3, 4, 6) "
		" AND CREATOR_ID = KSI.UTIL.GET_CUR_USER_ID "
		" ORDER BY ORDERS.ORDER_ID";
	}
	query->create_statement(sqlStr);
	try {
		query->execute_statement();
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
		return;
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		return;
	}
	
	while (!query->next()) {
		int order_id = static_cast<int>(boost::get<double>(query->get(0)));
		int type_id = static_cast<int>(boost::get<double>(query->get(1)));
		std::string type_name = boost::get<std::string>(query->get(2));
		double subtotal = boost::get<double>(query->get(3));
		boost::posix_time::ptime created = boost::get<boost::posix_time::ptime>(query->get(4));
		int guest_count = static_cast<int>(boost::get<double>(query->get(5)));
		std::string table_name = boost::get<std::string>(query->get(6));
		int table_group_id = static_cast<int>(boost::get<double>(query->get(7)));
		std::string table_group_name = boost::get<std::string>(query->get(8));
		std::string order_status = boost::get<std::string>(query->get(9));
		int creator_id = static_cast<int>(boost::get<double>(query->get(10)));
		double gross = boost::get<double>(query->get(11));
		
		
		std::map<std::string, std::string> mapper;
		mapper["endl"] = "\n";
		mapper["null"] = "";
		mapper["order_id"] = boost::lexical_cast<std::string>(order_id);
		mapper["order_status"] = order_status;
		mapper["creator"] = users->user(creator_id).fio();
		mapper["created"] = to_mb(locale->get_date_time(created));
		mapper["subtotal"] = to_mb(locale->get_currency(subtotal));
		mapper["type_id"] = boost::lexical_cast<std::string>(type_id);
		mapper["type_name"] = type_name;
		mapper["table_name"] = table_name;
		mapper["table_group_id"] = table_group_id;
		mapper["table_group_name"] = table_group_name;
		mapper["gross"] = to_mb(locale->get_currency(gross));
		mapper["guest_count"] = boost::lexical_cast<std::string>(guest_count);
		
		std::string cap = to_mb(locale->get_string("order_list", mapper));
		
		if (cap == "") {
			cap = "Order # " + boost::lexical_cast<std::string>(order_id) + " " + order_status + "\n"
			+ "Created by " + users->user(creator_id).fio() + "\n"
			+ "Accepted on " + to_mb(locale->get_date_time(created))+"\n"
			+ "Guests: " + boost::lexical_cast<std::string>(guest_count)
			+ ", Subtotal: " + to_mb(locale->get_currency(subtotal)) + "\n"
			+ table_group_name + " / " + table_name;
		}
		cap_vec.push_back(to_uc(cap));
		color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true);
		checked_vec.push_back(false);
		wrap_vec.push_back(false);
		order_id_vec.push_back(order_id);
	}
	
	wxString listCaption = locale->get("order_list_f", "caption3", "Order list");;
	wx_window_ptr<btn_list_f> form (new btn_list_f(this, listCaption, _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
	int result = form->ShowModal() - wxID_HIGHEST;
	if (result >= 0 && result < order_id_vec.size()) {
		int order_id = order_id_vec[result];
		std::auto_ptr<ksi_cafe::order_t> order;
		bool catch_exception = false;
		int res = 0;
		ksi_cafe::order_t* ord_ptr = 0;
		try {
			*flog << "load order_t, order_id = " + boost::lexical_cast<std::string>(order_id);
			wx_window_ptr<load_f> form (new load_f(this, locale->get("global", "order_load_str", "Order is loading"), locale->get("global", "please_wait_str", "Please wait.\nIf order is not opened for a long time, it might have been already opened by somebody else.")));
			form->start(boost::bind(load_order2, &ord_ptr, *cafe_container->current_cafe(), order_id, true, false));
			res = form->ShowModal();
		}
		catch (std::exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
				order.reset(ord_ptr);
			}
		}
		else {
			*flog << "delete ksi_cafe::order_t";
			delete ord_ptr;
			*flog << "successful";
			ord_ptr = 0;
		}
		if (order.get()) {
			wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
			wx_hide_t hide_main(this);
			int res = form->ShowModal();
			hide_main.force();
			if (res == wxID_HIGHEST+1) {
				browse_delayed_orders_btnClick(event);
			}
			else if (res == wxID_HIGHEST+2) {
				open_orders_btnClick(event);
			}
			else if (res == wxID_HIGHEST+3) {
				logout();
			}
		}
		else {
			return;
		}
	}
}

void main_f::logout_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement logout_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	
	*flog << "main_f::logout_btnClick";
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));
	
	wx_hide_t hide_body(main_sizer, body_pnl);
	wx_hide_t hide_status(main_sizer, bottom_pnl);
	wx_hide_t hide_button_hline(main_sizer, bottom_line);
	
	*flog << "lock";
	ksi_cafe::session.reset();
	current_user_lb->Hide();
	browse_delayed_orders_btn->Hide();
	open_orders_btn->Hide();
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
					try_cashierW_register(mac_vec[i]);
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
			wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
		}
		catch (ksi_client::ksi_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		}
		if (!successful) ksi_cafe::session.reset(0);
	}
	browse_delayed_orders_btn->Show(true);
	open_orders_btn->Show(true);
	current_user_lb->SetLabel(to_uc(users->user().role_name()) + _(" ") + to_uc(users->user().fio()));
	current_user_lb->Show(true);
	top_sizer->Layout();
	*flog << "Current USER = " + ksi_cafe::session->username();
	*flog << "unlock";
	vfd_print(display.get(), 1, vfd_line1, "", vfd_align_line1);
	vfd_print(display.get(), 2, vfd_line2, "", vfd_align_line2);
}

void main_f::open_cash_drawer_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement open_cash_drawer_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }	
	needLock = true;
	*flog << "main_f::open_cash_drawer_btnClick";
	
	if (printer.get()) printer->open_drawer();
	if (drawer.get()) drawer->open();
}

void main_f::close_order_operation_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement close_order_operation_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	needLock = true;
	*flog << "main_f::close_order_operation_btnClick";
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));

	try {
		wx_window_ptr<close_order_manager_f> form (new close_order_manager_f(this, locale->get("cashier_f", "close_order_management_str", "Operations with closed orders")));
		form->ShowModal();
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void main_f::cash_advance_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement cash_advance_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	needLock = true;
	*flog << "main_f::cash_advance_btnClick";
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));
	
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
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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

void main_f::create_to_go_order_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement create_to_go_order_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	needLock = true;
	*flog << "main_f::create_to_go_order_btnClick";
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));
	
	std::auto_ptr<ksi_cafe::order_t> order (new ksi_cafe::order_t(*cafe_container->current_cafe(), 0, false, false));	
	order->set_guest_count(1);
	order->set_type_to_go();
	wx_hide_t hide_main(this);
	wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
	int res = form->ShowModal();
	hide_main.force();
	if (res == wxID_HIGHEST+1) {
		browse_delayed_orders_btnClick(event);
	}
	else if (res == wxID_HIGHEST+2) {
		open_orders_btnClick(event);
	}
	else if (res == wxID_HIGHEST+3) {
		logout();
	}
}

void main_f::create_delivery_order_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement create_delivery_order_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	needLock = true;
	*flog << "main_f::create_delivery_order_btnClick";
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));
	
	std::auto_ptr<ksi_cafe::order_t> order (new ksi_cafe::order_t(*cafe_container->current_cafe(), 0, false, false));	
	order->set_guest_count(1);
	order->set_type_delivery(0);
	wx_hide_t hide_main(this);
	wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
	int res = form->ShowModal();
	hide_main.force();
	if (res == wxID_HIGHEST+1) {
		browse_delayed_orders_btnClick(event);
	}
	else if (res == wxID_HIGHEST+2) {
		open_orders_btnClick(event);
	}
	else if (res == wxID_HIGHEST+3) {
		logout();
	}
}

void main_f::create_table_booking_order_btnClick( wxCommandEvent& event ) {
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement create_table_booking_order_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	needLock = true;
	*flog << "main_f::create_table_booking_order_btnClick";
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));
	bool auth_ = true;
	if (config->get("security", "create_table_booking") == "0" || to_uc(config->get("security", "create_table_booking")).MakeUpper() == _("OFF"))
		auth_ = false;
	if (auth_ && !wx_mng_auth(this)) return;
	
	//std::auto_ptr<table_show_t> table_show (new table_show_t(std::vector<wxButton*>(), table_container.get(), false, true));
	//wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Select a table"), _(""), table_show.get()));
	//int result = form->ShowModal() - wxID_HIGHEST;
	//if (result >= 0) {
		//wxButton* table_btn = form->getTableBtn();
		//ksi_cafe::table_t table = table_show->get_table(table_btn);
		std::auto_ptr<ksi_cafe::order_t> order (new ksi_cafe::order_t(*cafe_container->current_cafe(), 0, false, false));
		order->set_guest_count(1);
		order->set_type_table_booking(0);
		wx_hide_t hide_main(this);
		wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
		int res = form->ShowModal();
		hide_main.force();
		if (res == wxID_HIGHEST+1) {
			browse_delayed_orders_btnClick(event);
		}
		else if (res == wxID_HIGHEST+2) {
			open_orders_btnClick(event);
		}
		else if (res == wxID_HIGHEST+3) {
			logout();
		}
	//}
}

void main_f::create_pickup_order_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement create_pickup_order_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	needLock = true;
	*flog << "main_f::create_pickup_order_btnClick";
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));
	
	std::auto_ptr<ksi_cafe::order_t> order (new ksi_cafe::order_t(*cafe_container->current_cafe(), 0, false, false));	
	order->set_guest_count(1);
	order->set_type_pickup(0);
	wx_hide_t hide_main(this);
	wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
	int res = form->ShowModal();
	hide_main.force();
	if (res == wxID_HIGHEST+1) {
		browse_delayed_orders_btnClick(event);
	}
	else if (res == wxID_HIGHEST+2) {
		open_orders_btnClick(event);
	}
	else if (res == wxID_HIGHEST+3) {
		logout();
	}
}

void main_f::create_employee_meal_order_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement create_employee_meal_order_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	needLock = true;
	*flog << "main_f::create_employee_meal_order_btnClick";
	
	destroyProcessor_t<boost::function<void ()> > resumeWait(boost::function<void ()>(boost::bind(&main_f::resume_wait, this)));
	
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
	wx_window_ptr<btn_list_f> form (new btn_list_f(this, cap_emp, footer_cap, cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
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
			if (query->next()) THROW_LOCATION(10345, "void cashier_f::order_type_btnClick( wxCommandEvent& event )");;
			int discounted = static_cast<int>(boost::get<double>(query->get(0)));
			if (discounted != 0) {
				wxMessageBox(to_uc(users->user(person_id).fio()) + _(" Employee meal is not available"), _("Message"), wxOK, this);
				return;
			}
			
			std::auto_ptr<ksi_cafe::order_t> order (new ksi_cafe::order_t(*cafe_container->current_cafe(), 0, false, false));	
			order->set_guest_count(1);
			order->set_type_employee_meal(person_id, "");
			wx_hide_t hide_main(this);
			wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
			int res = form->ShowModal();
			hide_main.force();
			if (res == wxID_HIGHEST+1) {
				browse_delayed_orders_btnClick(event);
			}
			else if (res == wxID_HIGHEST+2) {
				open_orders_btnClick(event);
			}
			else if (res == wxID_HIGHEST+3) {
				logout();
			}
		}
		catch (exception_t& ex) {
			*flog << ex.comp_message();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
			return;
		}
		catch (ksi_client::connection_problem_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			logout();
			return;
		}
		catch (ksi_client::db_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
			return;
		}
		catch (std::exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
				
				std::auto_ptr<ksi_cafe::order_t> order (new ksi_cafe::order_t(*cafe_container->current_cafe(), 0, false, false));	
				order->set_guest_count(1);
				order->set_type_employee_meal(0, to_mb(form->text()));
				{
					wx_hide_t hide_main(this);
					wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
					int res = form->ShowModal();
					hide_main.force();
					if (res == wxID_HIGHEST+1) {
						browse_delayed_orders_btnClick(event);
					}
					else if (res == wxID_HIGHEST+2) {
						open_orders_btnClick(event);
					}
					else if (res == wxID_HIGHEST+3) {
						logout();
					}
				}
			}
			else if (result > 0 && result <= user_vec.size()) {
				std::auto_ptr<ksi_cafe::order_t> order (new ksi_cafe::order_t(*cafe_container->current_cafe(), 0, false, false));	
				order->set_guest_count(1);
				order->set_type_employee_meal(user_vec[result-1].user_id(), "");
				{
					wx_hide_t hide_main(this);
					wx_window_ptr<cashier_f> form (new cashier_f(this, order.release()));
					int res = form->ShowModal();
					hide_main.force();
					if (res == wxID_HIGHEST+1) {
						browse_delayed_orders_btnClick(event);
					}
					else if (res == wxID_HIGHEST+2) {
						open_orders_btnClick(event);
					}
					else if (res == wxID_HIGHEST+3) {
						logout();
					}
				}
			}
			else return;
		}
		catch (exception_t& ex) {
			*flog << ex.comp_message();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
			return;
		}
		catch (ksi_client::connection_problem_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			logout();
			return;
		}
		catch (ksi_client::db_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
			return;
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			return;
		}
	}
}

void main_f::OnConfigDbClick( wxMouseEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement OnConfigDbClick
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
				locale.reset(new ksi_cafe::xml_lang_t(config->get("locale", "file"), config->get("locale", "name"), "cashierw"));
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
			}
		}
		apply_lang();	
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
					wxMessageBox(_("Unable to initialize COM4 port. VFD display will be disabled"), _("Message"), wxOK, this);
				}
			}
			
			if (config->get("resolution", "auto") == "1" || to_uc(config->get("resolution", "auto")).MakeUpper() == _("ON")) {
				wxScreenDC screen;
				SetSize(screen.GetSize().GetWidth(), screen.GetSize().GetHeight());
				btn_repaint();
				this->Move(0,0);
			}
			else {
				try {
					int width = boost::lexical_cast<int>(config->get("resolution", "width"));
					int height = boost::lexical_cast<int>(config->get("resolution", "height"));
					SetSize(width, height);
					btn_repaint();
				}
				catch (...){
					*flog << "bad cast resolution config, width = " + config->get("resolution", "width") + ", height = " + config->get("resolution", "height");
					wxScreenDC screen;
					SetSize(screen.GetSize().GetWidth(), screen.GetSize().GetHeight());
					btn_repaint();
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
					*flog << "create ksi_client::user_pos_i";
					int license_id = boost::lexical_cast<int>(config->get("Credit_Card_SubSystem", "license_id"));
					int site_id = boost::lexical_cast<int>(config->get("Credit_Card_SubSystem", "site_id"));
					int device_id = boost::lexical_cast<int>(config->get("Credit_Card_SubSystem", "device_id"));
					std::string user = config->get("Credit_Card_SubSystem", "user");
					std::string password = config->get("Credit_Card_SubSystem", "password");
					client = ksi_client::create_user_pos(license_id, site_id, device_id, user, password);
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
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			}
			catch (exception_t& ex) {
				*flog << ex.comp_message();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			}
			if (!tested_client) client.reset(0);
			try {
				if (to_uc(config->get("printer", "source")).MakeUpper() == _("REMOTE")) {
					printer.reset(new net_printer_t(config->get("printer", "host"), 703));
				}
				else if (to_uc(config->get("printer", "source")).MakeUpper() == _("LOCAL")) {
					com.reset(new com_t(config->get("printer", "port"), boost::lexical_cast<int>(config->get("printer", "baud_rate"))));
					if (to_uc(config->get("printer", "command_set")).MakeUpper() == _("STAR")) printer.reset(new star_printer_t(*com.get()));
					else if (to_uc(config->get("printer", "command_set")).MakeUpper() == _("EPSON")) printer.reset(new epson_printer_t(*com.get()));
					else com.reset();
				}
				else {
					printer.reset(new null_printer_t());
				}
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				//bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
			}
			catch (...) {
				*flog << "can't open serial port";
				//bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, "can't open serial port"));
				wxMessageBox(_("can't open serial port"), _("Message"), wxOK, this);
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
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			wxExit();
		}
		catch (exception_t& ex) {
			*flog << ex.comp_message();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
			wxExit();
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			wxExit();
		}
	}
}

void main_f::resume_wait() {
	if (needLock) {
		needLock = false;
		if (config->get("app", "auto_lock") == "1" || to_uc(config->get("app", "auto_lock")).MakeUpper() == _("ON")) {
			logout();
			return;
		}
	}
	table_show_->refresh();
	refresh_order_list();
}

void main_f::OnTimer (wxTimerEvent& event) {
	if (event.GetId() == timer_.timer_id()) {
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		current_dt_lb->SetLabel(locale->get_date_time(now));
	}
}

void main_f::OnSeanceTimer (wxTimerEvent& event) {
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
}

void main_f::OnLockTimer(wxTimerEvent& event) {
	//pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	//pause_timer_t<main_f> pause_lock_timer(lock_timer_);
}

void main_f::refresh_order_list() {
	wx_disable_t disable_window(this);
	pause_timer_t<main_f> pause_seance_timer(seance_timer_);
	pause_timer_t<main_f> pause_lock_timer(lock_timer_);
	
	if (!ksi_cafe::session_exist()) { logout(); }

	std::pair<int, int> result;
	try {
		result = makeOrderListInfo();
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		return;
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		logout();
		return;
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		return;
	}	
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		return;
	}

	open_order_count_ = result.first;
	delayed_order_count_ = result.second;
	
	browse_delayed_orders_btn->SetLabel(locale->get("cashier_f", "browse_delayed_orders_btn", "Browse delayed orders") + _(" (" + to_uc(boost::lexical_cast<std::string>(delayed_order_count_)) +_(")")));
	browse_delayed_orders_btn->Enable(delayed_order_count_);
	if (users->user().manager() || users->user().superwaiter())
		open_orders_btn->SetLabel(locale->get("cashier_f", "open_orders_btn_mng", "Browse all open orders") + _(" (") + to_uc(boost::lexical_cast<std::string>(open_order_count_)) + _(")"));
	else
		open_orders_btn->SetLabel(locale->get("cashier_f", "open_orders_btn", "Browse my open orders") + _(" (") + to_uc(boost::lexical_cast<std::string>(open_order_count_)) + _(")"));
	open_orders_btn->Enable(open_order_count_);
	
	top_sizer->Layout();
}

void main_f::logout() {
	wxCommandEvent event;
	logout_btnClick(event);
}

void main_f::apply_lang() {
	logout_btn->SetLabel(locale->get("cashier_f", "logout_btn", "Log out"));
	if (users->user().manager() || users->user().superwaiter())
		open_orders_btn->SetLabel(locale->get("cashier_f", "open_orders_btn_mng", "Browse all open orders") + _(" (") + to_uc(boost::lexical_cast<std::string>(open_order_count_)) + _(")"));
	else
		open_orders_btn->SetLabel(locale->get("cashier_f", "open_orders_btn", "Browse my open orders") + _(" (") + to_uc(boost::lexical_cast<std::string>(open_order_count_)) + _(")"));
	browse_delayed_orders_btn->SetLabel(locale->get("cashier_f", "browse_delayed_orders_btn", "Browse delayed orders") + _(" (") + to_uc(boost::lexical_cast<std::string>(delayed_order_count_)) +_(")"));
	top_sizer->Layout();
	
	open_cash_drawer_btn->SetLabel(locale->get("cashier_f", "open_cash_drawer_btn", "Open cash drawer"));
	wrap_label(open_cash_drawer_btn);
	create_table_booking_order_btn->SetLabel(locale->get("cashier_f", "create_table_booking_order_btn", "Table booking"));
	wrap_label(create_table_booking_order_btn);
	create_to_go_order_btn->SetLabel(locale->get("cashier_f", "to_go_btn", "To go"));
	wrap_label(create_to_go_order_btn);
	create_delivery_order_btn->SetLabel(locale->get("cashier_f", "delivery_btn", "Delivery"));
	wrap_label(create_delivery_order_btn);
	create_pickup_order_btn->SetLabel(locale->get("cashier_f", "pickup_btn", "Pickup"));
	wrap_label(create_pickup_order_btn);
	create_employee_meal_order_btn->SetLabel(locale->get("cashier_f", "employee_meal_btn", "Employee meal"));
	wrap_label(create_employee_meal_order_btn);
	close_order_operation_btn->SetLabel(locale->get("cashier_f", "refund_order_btn", "Operations with closed orders"));
	wrap_label(close_order_operation_btn);
	
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

	if (config->get("functionality", "order_type_to_go") == "0" || to_uc(config->get("functionality", "order_type_to_go")).MakeUpper() == _("OFF")) 
		create_to_go_order_btn->Show(false);
	else 
		create_to_go_order_btn->Show(true);
	if (config->get("functionality", "order_type_delivery") == "0" || to_uc(config->get("functionality", "order_type_delivery")).MakeUpper() == _("OFF")) 
		create_delivery_order_btn->Show(false);
	else
		create_delivery_order_btn->Show(true);
	if (config->get("functionality", "order_type_pickup") == "0" || to_uc(config->get("functionality", "order_type_pickup")).MakeUpper() == _("OFF"))
		create_pickup_order_btn->Show(false);
	else
		create_pickup_order_btn->Show(true);
	if (config->get("functionality", "order_type_employee_meal") == "0" || to_uc(config->get("functionality", "order_type_employee_meal")).MakeUpper() == _("OFF"))
		create_employee_meal_order_btn->Show(false);
	else
		create_employee_meal_order_btn->Show(true);
		
	if (config->get("functionality", "order_type_table_booking") == "0" || to_uc(config->get("functionality", "order_type_table_booking")).MakeUpper() == _("OFF")) {
		create_table_booking_order_btn->Show(false);
	}
	else {
		create_table_booking_order_btn->Show(true);
	}
		
	if (config->get("functionality", "cash_advance") == "0" || to_uc(config->get("functionality", "cash_advance")).MakeUpper() == _("OFF"))
		cash_advance_btn->Show(false);
	else 
		cash_advance_btn->Show(true);
	
	control_btn_pnl->SetSizer( control_btn_sizer );
	control_btn_pnl->Layout();
}
