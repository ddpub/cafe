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
#include <ksi_order/menu_container.hpp>
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
#include <shared_modules/count_f.h>
#include <shared_modules/dlg_f.h>
#include <boost/shared_ptr.hpp>
#include <shared_modules/split_order_f.h>
#include <ksi_order/cctrans_manager.hpp>

std::auto_ptr<ksi_cafe::menu_manager_t> menu_manager;
std::auto_ptr<move_up_t> move_up;
std::auto_ptr<ksi_cafe::simple_stop_list_t> stop_list;
extern std::auto_ptr<xml_config_t> config;
extern std::auto_ptr<ksi_cafe::users_t> users;
extern std::auto_ptr<ksi_cafe::cafe_container_t> cafe_container;
extern std::auto_ptr<ksi_cafe::menu_container_t> menu_container;
extern std::auto_ptr<ksi_cafe::coupon_storage_t> coupon_storage;
extern std::auto_ptr<ksi_cafe::coupon_in_cafe_t> coupon_in_cafe;
extern std::auto_ptr<ksi_client::user_pos_i> client;
extern std::auto_ptr<file_log_t> flog;
extern std::auto_ptr<file_log_t> thflog;
extern std::auto_ptr<base_printer_t> printer;
extern std::auto_ptr<xml_config_t> config;
extern std::auto_ptr<drawer_t> drawer;
extern std::auto_ptr<customer_display_t> display;
extern std::auto_ptr<bag_spooler_t> bag_spooler;
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;
std::auto_ptr<fiscal::registrator_t> registrar;
extern std::auto_ptr<ksi_cafe::xml_lang_t> shared_locale;
extern std::auto_ptr<ksi_cafe::table_container_t> table_container;

extern bool hide_mode_;
extern bool by_group;
extern bool stand_alone;
extern int lang;

extern std::string app_name;
extern int app_id;
extern std::string server_host;

namespace version {
extern std::string version;
extern std::string compamny;
extern std::string product;
extern std::string copyright;
};

extern std::string vfd_line1;
extern std::string vfd_line2;
extern int vfd_align_line1;
extern int vfd_align_line2;

int last_menu_id = 0;

cashier_f* currentCashier_f = 0;

wxString wxOrderTreeListCtrl::OnGetItemText (wxTreeItemData* item_data, long column) const
{
  if (wx_disable_t::disable(currentCashier_f)) return _("");
  return sg_get_text(item_data, column);
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
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, sg);
	}
	THROW_LOCATION(0, "const ksi_cafe::order_dish_t& get_order_dish()");
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
		menu_tree_vec[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( cashier_f::menu_tree_btnClick ), NULL, this );
		menu_tree_vec[i]->Hide();
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
	
	int h_pos = btn_height + btn_space;
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
	
	move_up.reset(new move_up_t(menu_tree_vec, menu_item_vec, menu_hot_vec));
	move_up->show(menu_manager->selected());
}

cashier_f::cashier_f( wxWindow* parent, ksi_cafe::order_t* order_ptr)
: base_cashier_f( parent ) , menu_select_btn(0), timer_(obj_handler_t<cashier_f>(this, &cashier_f::OnTimer)),
seance_timer_(obj_handler_t<cashier_f>(this, &cashier_f::OnSeanceTimer)), order(order_ptr), orderChanged_(false)
{
	currentCashier_f = this;
	sg->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( cashier_f::sgSelChanged ), NULL, this );
	sg->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( cashier_f::sgSelChanging ), NULL, this );

	room_lb->SetLabel(_(" ") + to_uc(order->table().table_group_name()) + _(" / ") + to_uc(order->table().name()));
	delayed_order_count_ = 0;
	open_order_count_ = 0;
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	current_dt_lb->SetLabel(_("                   "));
	
	plus_btn->SetBitmapLabel(CashierBitmapsFunc(2));
	plus_btn->SetBitmapDisabled(CashierBitmapsFunc(3));
	minus_btn->SetBitmapLabel(CashierBitmapsFunc(4));
	minus_btn->SetBitmapDisabled(CashierBitmapsFunc(5));
	
	sg->AddRoot(_("root"));
	SetTitle(to_uc(app_name));
	if (config->get("null", "trainig_mode") == "1" || to_uc(config->get("null", "trainig_mode")).MakeUpper() == _("ON")) hide_mode_ = true;
	else hide_mode_ = false;
	
	if (config->get("resolution", "stay_on_top") == "0" || to_uc(config->get("resolution", "stay_on_top")).MakeUpper() == _("OFF")) {
	}
	else {
		long currStyle = this->GetWindowStyle();
		this->SetWindowStyle(currStyle|wxSTAY_ON_TOP);
	}	
	
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
	
	guest_btn->SetLabel(locale->get("cashier_f", "guest_btn", "Guests: ") + to_uc(boost::lexical_cast<std::string>(order->guest_count())));
	if (order->type_for_here()) order_type_btn->SetLabel(locale->get("cashier_f", "for_here_btn", "For here"));
	else if (order->type_to_go()) order_type_btn->SetLabel(locale->get("cashier_f", "to_go_btn", "To go"));
	else if (order->type_delivery()) order_type_btn->SetLabel(locale->get("cashier_f", "delivery_btn", "Delivery"));
	else if (order->type_pickup()) order_type_btn->SetLabel(locale->get("cashier_f", "pickup_btn", "Pickup"));
	else if (order->type_employee_meal()) order_type_btn->SetLabel(locale->get("cashier_f", "employee_meal_btn", "Employee meal"));
	else if (order->type_table_booking()) order_type_btn->SetLabel(locale->get("cashier_f", "table_booking_btn", "Table booking"));
	wrap_label(order_type_btn);
	
	if (order->token().exist()) token_btn->SetLabel(to_uc(order->token().name()));
	else token_btn->SetLabel(locale->get("cashier_f", "token_btn1", "Token")+_(": ")+locale->get("cashier_f", "token_btn2", "not selected"));
	wrap_label(token_btn);
	
	int curr_cafe_id = cafe_container->current_cafe()->cafe_id();
	std::vector<ksi_cafe::menu_t*> menu_vec = menu_container->menu_for_cafe(curr_cafe_id);
	menu_manager.reset(new ksi_cafe::menu_manager_t(menu_vec));
	menu_vec = menu_manager->menu_vec();
	std::vector<ksi_cafe::menu_t*>::const_iterator lastMenuIterator = 
	std::find_if(menu_vec.begin(), menu_vec.end(), boost::bind(std::equal_to<int>(), last_menu_id, boost::bind(&ksi_cafe::menu_t::menu_id, _1)));
	
	if (lastMenuIterator != menu_vec.end()) {
		menu_manager->select(last_menu_id, order->table().table_group_id());
	}
	else if (menu_vec.begin() != menu_vec.end()) {
		menu_manager->select((*menu_vec.begin())->menu_id(), order->table().table_group_id());
	}
	
	if (order->sales_tax() == 0 && order->gratuity_tax() == 0) {
		hide_sales_tax();
		hide_gratuity_tax();
		hide_subtotal();
	}
	else if (order->sales_tax() == 0) hide_sales_tax();
	else if (order->gratuity_tax() == 0) hide_gratuity_tax();

	delayed_order_details_btn->Show(order->type_delivery() || order->type_pickup() || order->type_table_booking());
	
	current_user_lb->SetLabel(to_uc(users->user().role_name()) + _(" ") + to_uc(users->user().fio()));
	btn_repaint();
	apply_lang();
	refresh_order_list();
	
	try {
		coupon_in_cafe->unselect_all();
		for (int i=0; i<order->dish_count(); ++i) {
			int coupon_id = order->dish(i).coupon_id();
			int coupon_mng_id = order->dish(i).coupon_mng_id();
			if (coupon_id && coupon_id != -1) {
				coupon_in_cafe->select(coupon_id, 1, coupon_mng_id);
			}
			const std::vector<ksi_cafe::order_dish_modifier_t>& mods = order->dish(i).modifier_vec();
			for (size_t j=0; j<mods.size(); ++j) {
				int coupon_id = mods[j].coupon_id();
				int coupon_mng_id = mods[j].coupon_mng_id();
				if (coupon_id && coupon_id != -1) {
					coupon_in_cafe->select(coupon_id, 1, coupon_mng_id);
				}
			}
		}
	}
	catch (std::exception& ex) {
		*flog << ex.what();
	}
	recalc_coupon();
	// Connect Events
	timer_.start(1);
	//"never" "5 sec" "10 sec" "15 sec" "30 sec" "60 sec"
	std::string check_timeout_str = config->get("app", "check_timeout");
	if (check_timeout_str == "5 sec") seance_timer_.start(5);
	else if (check_timeout_str == "10 sec") seance_timer_.start(10);
	else if (check_timeout_str == "15 min") seance_timer_.start(15);
	else if (check_timeout_str == "30 min") seance_timer_.start(30);
	else if (check_timeout_str == "60 min") seance_timer_.start(60);
	//ctrl_btn_pnl->SetSizer( ctrl_sizer );
	//ctrl_btn_pnl->Layout();
	//ctrl_sizer->Fit( ctrl_btn_pnl );
	rpanel->Layout();
	//rsizer->Fit( rpanel );
	
	if (config->get("functionality", "entree_codes") == "0" || to_uc(config->get("functionality", "entree_codes")).MakeUpper() == _("OFF"))
		entree_code_btn->Show(false);
	else
		entree_code_btn->Show(true);
		
	if (config->get("functionality", "misc_charges") == "0" || to_uc(config->get("functionality", "misc_charges")).MakeUpper() == _("OFF"))
		misc_charges_btn->Show(false);
	else
		misc_charges_btn->Show(true);
		
	if (config->get("functionality", "tokens") == "0" || to_uc(config->get("functionality", "tokens")).MakeUpper() == _("OFF"))
		token_btn->Show(false);
	else
		token_btn->Show(true);
		
	if (config->get("functionality", "comment_entree") == "0" || to_uc(config->get("functionality", "comment_entree")).MakeUpper() == _("OFF"))
		comment_entree_btn->Show(false);
	else
		comment_entree_btn->Show(true);
		
	if (config->get("functionality", "dicount") == "0" || to_uc(config->get("functionality", "discount")).MakeUpper() == _("OFF"))
		discount_btn->Show(false);
	else
		discount_btn->Show(true);
		
	discount_btn->SetLabel(locale->get("cashier_f", "discount_btn", "discounts"));
	delayed_order_details_btn->SetLabel(locale->get("cashier_f", "delayed_order_details_btn", "Delayed order details"));
	wrap_label(delayed_order_details_btn);
	save_changes_btn->SetLabel(locale->get("cashier_f", "save_changes_btn", "Save changes"));
	wrap_label(save_changes_btn);
	cancel_changes_btn->SetLabel(locale->get("cashier_f", "cancel_changes_btn", "Cancel changes"));
	wrap_label(cancel_changes_btn);
	entree_code_btn->SetLabel(locale->get("cashier_f", "entree_code_btn", "Entree code"));
	wrap_label(entree_code_btn);
	misc_charges_btn->SetLabel(locale->get("cashier_f", "misc_charges_btn", "Misc charges"));
	wrap_label(misc_charges_btn);
	comment_entree_btn->SetLabel(locale->get("cashier_f", "comment_entree_btn", "Comment entree"));
	wrap_label(comment_entree_btn);
	print_receipt_duplicate_btn->SetLabel(locale->get("cashier_f", "print_receipt_duplicate_btn", "Print receipt"));
	wrap_label(print_receipt_duplicate_btn);
	other_orders_operations_btn->SetLabel(locale->get("cashier_f", "other_orders_operations_btn", "Other operations"));
	wrap_label(other_orders_operations_btn);
	close_order_btn->SetLabel(locale->get("cashier_f", "close_order_btn", "Close order"));
	wrap_label(close_order_btn);
	
	top_bar_sizer->Layout();
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

void cashier_f::browse_delayed_orders_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	
	*flog << "cashier_f::browse_delayed_orders_btnClick";
	// TODO: Implement browse_delayed_orders_btnClick
	if (orderChanged_) {
		std::vector<wxString> cap_vec;
		cap_vec.push_back(_("Yes")); cap_vec.push_back(_("No")); cap_vec.push_back(_("Cancel"));
		std::vector<wxColour> color_vec;
		color_vec.push_back(ok_color); color_vec.push_back(close_color); color_vec.push_back(close_color);
		wxString message = _("Save open order changes?");
		wx_window_ptr<dlg_f> dlg (new dlg_f(this, _("Confirmation"), message, cap_vec, color_vec));
		
		int res = dlg->ShowModal();
		if (res == wxID_HIGHEST) { // yes
			wxCommandEvent evt(0, 1);
			save_changes_btnClick(evt);
		}
		else if (res == wxID_HIGHEST+1) { // no
		}
		else return;
	}
	*flog << "EndModal";
	EndModal(wxID_HIGHEST+1);

}

void cashier_f::open_orders_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }

	*flog << "cashier_f::open_orders_btnClick";
	// TODO: Implement open_orders_btnClick
	if (orderChanged_) {
		std::vector<wxString> cap_vec;
		cap_vec.push_back(_("Yes")); cap_vec.push_back(_("No")); cap_vec.push_back(_("Cancel"));
		std::vector<wxColour> color_vec;
		color_vec.push_back(ok_color); color_vec.push_back(close_color); color_vec.push_back(close_color);
		wxString message = _("Save open order changes?");
		wx_window_ptr<dlg_f> dlg (new dlg_f(this, _("Confirmation"), message, cap_vec, color_vec));
		
		int res = dlg->ShowModal();
		if (res == wxID_HIGHEST) { // yes
			wxCommandEvent evt(0, 1);
			save_changes_btnClick(evt);
		}
		else if (res == wxID_HIGHEST+1) { // no
		}
		else return;
	}
	*flog << "EndModal";
	EndModal(wxID_HIGHEST+2);
}

void cashier_f::logout_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement logout_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	
	if (orderChanged_) {
		std::vector<wxString> cap_vec;
		cap_vec.push_back(_("Yes")); cap_vec.push_back(_("No")); cap_vec.push_back(_("Cancel"));
		std::vector<wxColour> color_vec;
		color_vec.push_back(ok_color); color_vec.push_back(close_color); color_vec.push_back(close_color);
		wxString message = _("Save open order changes?");
		wx_window_ptr<dlg_f> dlg (new dlg_f(this, _("Confirmation"), message, cap_vec, color_vec));
		
		int res = dlg->ShowModal();
		if (res == wxID_HIGHEST) { // yes
			*flog << "Save changes";
			wxCommandEvent evt(0, 1);
			save_changes_btnClick(evt);
		}
		else if (res == wxID_HIGHEST+2) { // no
		}
		else return;
	}
	*flog << "EndModal";
	EndModal(wxID_HIGHEST+3);
}

void cashier_f::misc_charges_btnClick( wxCommandEvent& event ) {
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	bool auth_ = true;
	if (config->get("security", "create_misc_charges") == "0" || to_uc(config->get("security", "create_misc_charges")).MakeUpper() == _("OFF"))
		auth_ = false;
	try {
		if (auth_ && !wx_mng_auth(this)) return;
		wx_window_ptr<text_f> form (new text_f(this, locale->get("cashier_f", "add_misc_chage_str", "Add a miscellaneous charge"), locale->get("cashier_f", "charge_shot_description_str", "Charge short descrition"), true));
		if (form->ShowModal() != wxID_OK) return;
		orderChanged_ = true;
		const ksi_cafe::order_dish_t& dish = order->add_misc_charges(locale->reverse_currency(form->amount()), to_mb(form->text()));
		recalc_coupon(dish.type_check_sum(), dish.check_sum());
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));		
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void cashier_f::entree_code_btnClick( wxCommandEvent& event ) {
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	wx_window_ptr<input_num_f> form (new input_num_f(this, locale->get("global", "type_entree_code_str", "Type entree code"), false));
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
		wxMessageBox(locale->get("global", "entree_not_found_str", "Entree not found"), _("Message"), wxOK, this);
		return;
	}
	
	try {
		
		stop_list.reset(new ksi_cafe::simple_stop_list_t());
		if (stop_list->find_entry(dish->dish_id())) {
			const ksi_cafe::stop_list_entry_t& entry = stop_list->get_fined_entry(dish->dish_id());
			wxString msg = _("Entree in stop list since ") + locale->get_date_time(entry.since_dt()) + _(" till ") + locale->get_date_time(entry.till_dt());
			if (lang == 2) msg = _("Блюдо в стоп листе с ") + locale->get_date_time(entry.since_dt()) + _(" по ") + locale->get_date_time(entry.till_dt());
			wxMessageBox(msg, _("Message"), wxOK, this);
			return;
		}
		
		wx_window_ptr<add_modifiers_f> mod_form (new add_modifiers_f(this, locale->get("global", "select_modifiers_str", "Select modifiers"), stop_list.get()));
		if (mod_form->set_modifiers(menu_manager->selected(), dish)) {
			if (mod_form->ShowModal() != wxID_OK) return;
		}
		orderChanged_ = true;
		std::vector<ksi_cafe::order_dish_modifier_t> mods = mod_form->sel_mods();
		ksi_cafe::order_dish_t odish(*dish, mods);
		order->add_dish(odish, true);
		*flog << "add_dish: " + odish.check_sum();
		recalc_coupon(odish.type_check_sum(), odish.check_sum());
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
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
}

void cashier_f::comment_entree_btnClick( wxCommandEvent& event ) {
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	try {
		const ksi_cafe::order_dish_t& dish = get_order_dish(sg);
		wx_window_ptr<text_f> form (new text_f(this, locale->get("cashier_f", "comment_entree_str", "Comment entry"), _(""), 0));
		form->text(to_uc(dish.description()));
		if (form->ShowModal() != wxID_OK) return;
		orderChanged_ = true;
		const_cast<ksi_cafe::order_dish_t&>(dish).set_description(to_mb(form->text()));
		recalc_coupon(dish.type_check_sum(), dish.check_sum());
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void cashier_f::plus_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement plus_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
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

void cashier_f::minus_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement minus_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
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
			if (config->get("functionality", "allow_remove_kitchen_entrees") == "0" || to_uc(config->get("functionality", "allow_remove_kitchen_entrees")).MakeUpper() == _("OFF")) {
				wxMessageBox(_("This entree has been sent to kitchen and thus cannot be removed"), _("Message"), wxOK, this);
				return;
			}
			else {
				wxString message = locale->get("global", "waste_entree_str", "waste entree products/ingrediends?");
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
					recalc_coupon();
				}
				else {
					EndModal(wxID_CANCEL);
					return;
				}
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
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void cashier_f::order_type_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement order_type_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	*flog << "order_type_btnClick";
	
	std::vector<wxString> cap_vec;
	std::vector<wxColour> color_vec;
	std::vector<bool> enabled_vec;
	std::vector<bool> checked_vec;
	std::vector<bool> wrap_vec;
	
	std::map<size_t, ksi_cafe::order_type::order_type_t> orderTypeIndex_;
	
	if (config->get("functionality", "order_type_for_here") == "0" || to_uc(config->get("functionality", "order_type_for_here")).MakeUpper() == _("OFF"))  {
	}
	else {
		orderTypeIndex_[std::distance(cap_vec.begin(), cap_vec.end())] = ksi_cafe::order_type::FOR_HERE;
		cap_vec.push_back(locale->get("cashier_f", "for_here_btn", "For here"));
		color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true);
		checked_vec.push_back(order->type_for_here());
		wrap_vec.push_back(true);
	}
	if (config->get("functionality", "order_type_to_go") == "0" || to_uc(config->get("functionality", "order_type_to_go")).MakeUpper() == _("OFF"))  {
	}
	else {
		orderTypeIndex_[std::distance(cap_vec.begin(), cap_vec.end())] = ksi_cafe::order_type::TO_GO;
		cap_vec.push_back(locale->get("cashier_f", "to_go_btn", "To go"));
		color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true);
		checked_vec.push_back(order->type_to_go());
		wrap_vec.push_back(true);
	}
	if (config->get("functionality", "order_type_delivery") == "0" || to_uc(config->get("functionality", "order_type_delivery")).MakeUpper() == _("OFF"))  {
	}
	else {
		orderTypeIndex_[std::distance(cap_vec.begin(), cap_vec.end())] = ksi_cafe::order_type::DELIVERY;
		cap_vec.push_back(locale->get("cashier_f", "delivery_btn", "Delivery"));
		color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true);
		checked_vec.push_back(order->type_delivery());
		wrap_vec.push_back(true);
	}
	if (config->get("functionality", "order_type_pickup") == "0" || to_uc(config->get("functionality", "order_type_pickup")).MakeUpper() == _("OFF")) {
	}
	else {
		orderTypeIndex_[std::distance(cap_vec.begin(), cap_vec.end())] = ksi_cafe::order_type::PICKUP;
		cap_vec.push_back(locale->get("cashier_f", "pickup_btn", "Pickup"));
		color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true);
		checked_vec.push_back(order->type_pickup());
		wrap_vec.push_back(true);
	}
	if (config->get("functionality", "order_type_employee_meal") == "0" || to_uc(config->get("functionality", "order_type_employee_meal")).MakeUpper() == _("OFF")) {
	}
	else {
		orderTypeIndex_[std::distance(cap_vec.begin(), cap_vec.end())] = ksi_cafe::order_type::EMPLOYEE_MEAL;
		cap_vec.push_back(locale->get("cashier_f", "employee_meal_btn", "Employee meal"));
		color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true);
		checked_vec.push_back(order->type_employee_meal());
		wrap_vec.push_back(true);
	}
	if (config->get("functionality", "order_type_table_booking") == "0" || to_uc(config->get("functionality", "order_type_table_booking")).MakeUpper() == _("OFF")) {
	}
	else {
		orderTypeIndex_[std::distance(cap_vec.begin(), cap_vec.end())] = ksi_cafe::order_type::TABLE_BOOKING;
		cap_vec.push_back(locale->get("cashier_f", "table_booking_btn", "Table booking"));
		color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true);
		checked_vec.push_back(order->type_table_booking());
		wrap_vec.push_back(true);
	}
	
	wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Change order type"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
	int result = form->ShowModal() - wxID_HIGHEST;
	if (result >= 0 && result < cap_vec.size()) {
		if (orderTypeIndex_[result] == ksi_cafe::order_type::FOR_HERE) {
			*flog << "ksi_cafe::order_t::set_type_for_here()";
			if (order->type_for_here()) return;
			orderChanged_ = true;
			discount_btn->Enable(true);
			order->set_type_for_here(order->type_table_booking() && order->get_delivery_info() ? order->get_delivery_info()->clone() : 0);
			delayed_order_details_btn->Show(false);
			order_type_btn->SetLabel(locale->get("cashier_f", "for_here_btn", "For here"));
		}
		else if (orderTypeIndex_[result] == ksi_cafe::order_type::TO_GO) {
			*flog << "ksi_cafe::order_t::set_type_to_go()";
			if (order->type_to_go()) return;
			orderChanged_ = true;
			discount_btn->Enable(true);
			order->set_type_to_go();
			delayed_order_details_btn->Show(false);
			order_type_btn->SetLabel(locale->get("cashier_f", "to_go_btn", "To go"));
		}
		else if (orderTypeIndex_[result] == ksi_cafe::order_type::DELIVERY) {
			*flog << "ksi_cafe::order_t::set_type_delivery()";
			if (order->type_delivery()) return;
			orderChanged_ = true;
			discount_btn->Enable(true);
			order->set_type_delivery(0);
			delayed_order_details_btn->Show(true);
			order_type_btn->SetLabel(locale->get("cashier_f", "delivery_btn", "Delivery"));
		}
		else if (orderTypeIndex_[result] == ksi_cafe::order_type::PICKUP) {
			*flog << "ksi_cafe::order_t::set_type_pickup()";
			if (order->type_pickup()) return;
			orderChanged_ = true;
			discount_btn->Enable(true);
			order->set_type_pickup(0);
			delayed_order_details_btn->Show(true);
			order_type_btn->SetLabel(locale->get("cashier_f", "pickup_btn", "Pickup"));
		}
		else if (orderTypeIndex_[result] == ksi_cafe::order_type::EMPLOYEE_MEAL) {
			*flog << "ksi_cafe::order_t::set_type_employee_meal()";
			if (order->type_employee_meal()) return;
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
					if (query->next()) THROW_LOCATION(10344, "void cashier_f::order_type_btnClick( wxCommandEvent& event )");
					int discounted = static_cast<int>(boost::get<double>(query->get(0)));
					if (discounted != 0) {
						wxMessageBox(to_uc(users->user(person_id).fio()) + _(" Employee meal is not available"), _("Message"), wxOK, this);
						return;
					}
					orderChanged_ = true;
					order->clear_employee();
					order->clear_coupon();
					*flog << "ksi_cafe::order_t::set_type_employee_meal";
					order->set_type_employee_meal(person_id, "");
					*flog << "successful";
					order_type_btn->SetLabel(locale->get("cashier_f", "employee_meal_btn", "Employee meal"));
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
						orderChanged_ = true;
						order->clear_employee();
						order->clear_coupon();
						order->set_type_employee_meal(0, to_mb(form->text()));
					}
					else if (result > 0 && result <= user_vec.size()) {
						orderChanged_ = true;
						order->clear_employee();
						order->clear_coupon();
						order->set_type_employee_meal(user_vec[result-1].user_id(), "");
					}
					else return;
					order_type_btn->SetLabel(locale->get("cashier_f", "employee_meal_btn", "Employee meal"));
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
			else return;
			
			discount_btn->Enable(false);
			delayed_order_details_btn->Show(false);
		}
		else if (orderTypeIndex_[result] == ksi_cafe::order_type::TABLE_BOOKING) {
			*flog << "ksi_cafe::order_t::set_type_table_booking()";
			if (order->type_table_booking()) return;
			orderChanged_ = true;
			order->set_type_table_booking(0);
			delayed_order_details_btn->Show(true);
			order_type_btn->SetLabel(locale->get("cashier_f", "table_booking_btn", "Table booking"));
			wrap_label(order_type_btn);
		}
	}
	recalc_coupon();
	//ctrl_btn_pnl->SetSizer( ctrl_sizer );
	//ctrl_btn_pnl->Layout();
	//ctrl_sizer->Fit( ctrl_btn_pnl );
	rpanel->Layout();
	//rsizer->Fit( rpanel );
}

void cashier_f::guest_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement guest_btnClick
	*flog << "cashier_f::guest_btnClick";
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seqnce_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	wx_window_ptr<count_f> form (new count_f(this, _("Specify number of guests"), 1, 1000, order->guest_count()));
	form->ShowModal();
	orderChanged_ = true;
	*flog << "ksi_cafe::order_t::set_guest_count(" + boost::lexical_cast<std::string>(form->get_count()) + ")";
	order->set_guest_count(form->get_count());
	guest_btn->SetLabel(locale->get("cashier_f", "guest_btn", "Guests: ") + to_uc(boost::lexical_cast<std::string>(order->guest_count())));
}

void cashier_f::token_btnClick( wxCommandEvent& event ) {
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
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
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		logout();
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}

	try {
		wx_window_ptr<btn_list_f> form (new btn_list_f(this, locale->get("global", "select_a_token_str", "Select a token"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
		int result = form->ShowModal() - wxID_HIGHEST;
		if (result >= 0 && result < token_vec.size()+1) {
			orderChanged_ = true;
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
		wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void cashier_f::delayed_order_details_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement delayed_order_details_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	
	*flog << "delayed_order_details_btnClick";
	
	int type_id = 0;
	if (order->type_delivery()) {
		type_id = 3;
	}
	else if (order->type_pickup()) {
		type_id = 4;
	}
	else if (order->type_table_booking()) {
		type_id = 6;
	}
	else return;
	
	wxString postfix = _(" order");
	if (order->order_id()) postfix = _(" order # ") + to_uc(boost::lexical_cast<std::string>(order->order_id()));
	sg_clear(sg);
	wx_window_ptr<delayed_order_f> form (new delayed_order_f(this, locale->get("global", "delayed_order_details_str1", "Delayed ") + postfix + locale->get("global", "delayed_order_details_str2", " details"), type_id, false, order.release()));
	try {
		orderChanged_ = true;
		int result = form->ShowModal();
		order.reset(form->release());
		if (result == wxID_OK) {
			*flog << "save_delay_order";
			recalc_coupon("", "");
		}
		else if (result == wxID_CLOSE) {
			*flog << "close_delay_order";
			return;
		}
		else if (result == wxID_UNDO) {
			*flog << "cancel_delay_order";
			return;
		}
		else recalc_coupon("", "");
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
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void cashier_f::save_changes_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement save_changes_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
		
	if (order->order_id() == 0 && order->real_dish_count() == 0) {
		return;
	}
	
	if ((order->type_delivery() || order->type_pickup() || order->type_table_booking()) && !order->get_delivery_info()) {
		wxString msg = locale->get("global", "required_delayed_order_properties_str", "Required delayed order properties are not filled, order processing is not available.");
		wxMessageBox(msg, _("Message"), wxOK, this);
		return;
	}
	
	if (order->type_delivery() && order->subtotal() < order->delivery_min_subtotal()) {
		wxString msg4_str = locale->get("global", "msg4_str", "Minimal order subtotal available for delivery is ");
		wxMessageBox(msg4_str + locale->get_currency(order->delivery_min_subtotal()), _("Message"), wxOK, this);
		return;
	}
	
	if (order->type_employee_meal() && order->real_dish_count() == 0) {
		return;
	}
	
	if (order->type_table_booking() && !order->table().exist()) {
		return;
	}
	
	if (order->type_employee_meal() && order->def_subtotal() > order->meal_free_limit()) {
		std::vector<wxString> cap_vec;
		cap_vec.push_back(locale->get("global", "yes_str", "Yes")); cap_vec.push_back(locale->get("global", "no_str", "No"));
		std::vector<wxColour> color_vec;
		color_vec.push_back(ok_color); color_vec.push_back(close_color);
		wxString msg3_prefix = locale->get("global", "msg3_prefix", "Meal free limit has been exceeded on ");
		wxString msg3_postfix = locale->get("global", "msg3_postfix", ". Proceed?");
		wxString message = msg3_prefix + locale->get_currency(order->def_subtotal()-order->meal_free_limit()) + msg3_postfix;
		wx_window_ptr<dlg_f> dlg (new dlg_f(this, _("Employee meal"), message, cap_vec, color_vec));
		
		int res = dlg->ShowModal();
		if (res == wxID_HIGHEST) { // yes
			if (!wx_mng_auth(this)) return;
		}
		else if (res == wxID_HIGHEST+1) { // no
			return;
		}
		else return;
	}
	
	try {
		*flog << "save_changes";
		order->set_accept(0, true);
		int order_id = order->order_id();
		*flog << "order commit. order_id = " + boost::lexical_cast<std::string>(order_id);
		if (!order->commit(true)) *flog << "!!! force apply";
		*flog << "successful";
		orderChanged_ = false;
/*
		sg_clear(sg);
		order.reset();
*/
		if (event.GetId() != 1) {
			EndModal(wxID_OK);
			return;
		}
/*
		*flog << "reloadOrder";
		ksi_cafe::order_t* ord_ptr = reloadOrder(this, order_id, *flog, *cafe_container);
		*flog << "reloadOrder.successful";
		order.reset(ord_ptr);
		if (order.get()) {
			order->clear_wasted();
			recalc_coupon();
		}
		else {
			EndModal(wxID_CANCEL);
			return;
		}
*/
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
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

bool fixOrder(wxWindow* parentWnd, ksi_cafe::order_t* order_ptr, const std::string& pos_exceptionMessage) {
	*flog << "fixOrder";
	std::vector<wxString> cap_vec;
	cap_vec.push_back(_("Ok")); cap_vec.push_back(_("Fix order in DB"));
	std::vector<wxColour> color_vec;
	color_vec.push_back(ok_color); color_vec.push_back(close_color);
	wxString message = to_uc(pos_exceptionMessage);
	wx_window_ptr<dlg_f> dlg (new dlg_f(parentWnd, _("Confirmation"), message, cap_vec, color_vec));
	int res = dlg->ShowModal();
	if (res == wxID_HIGHEST) { // ok
	}
	else if (res == wxID_HIGHEST+1) { // Fix order in DB
		if (!wx_mng_auth(parentWnd)) return false;
		
		std::vector<wxString> cap_vec; std::vector<wxColour> color_vec; std::vector<bool> enabled_vec; std::vector<bool> checked_vec; std::vector<bool> wrap_vec;
		cap_vec.push_back(_("Remove authorization info\n order will remain opened"));
		cap_vec.push_back(_("Save settlement info\n order will be closed"));
		color_vec.push_back(btn_face_color); color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true); enabled_vec.push_back(true);
		checked_vec.push_back(false); checked_vec.push_back(false);
		wrap_vec.push_back(true); wrap_vec.push_back(true);
		wx_window_ptr<btn_list_f> form (new btn_list_f(parentWnd, _("Order repair"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
		int result = form->ShowModal() - wxID_HIGHEST;
		
		if (result == 0) { // Remove authorization info
			try {
				ksi_cafe::cctrans_vec_t base_vec = ksi_cafe::fetch_cctrans_for_order(order_ptr->order_id());
				ksi_cafe::cctrans_record_t auth = ksi_cafe::payment_auth(base_vec, 2, order_ptr->prepayment_type_id());
				int cctrans_id = auth.trans_id();
				
				std::auto_ptr<ksi_cafe::logical_transaction_t> log_trans;
				log_trans.reset(new ksi_cafe::logical_transaction_t(ksi_cafe::session.get(), 0));
				
				std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();

				query->create_statement
				(
				"DELETE FROM KSI.ORDER_CCTRANS WHERE CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID AND TRANS_ID = :trans_id"
				);

				query->set("trans_id", cctrans_id);
				query->execute_statement();
				
				query->create_statement
				(
				"UPDATE KSI.ORDERS SET PAYMENT_TYPE_ID = NULL "
				"WHERE CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID AND ORDER_ID = :order_id"
				);
				
				query->set("order_id", order_ptr->order_id());
				query->execute_statement();
				log_trans.reset();
				return true;
			}
			catch (std::exception& ex) {
				*flog << ex.what();
			}
			return false;
		}
		else if (result == 1) { // Save settlement info
			double total = order_ptr->total() - order_ptr->prepayment_amount();
			wx_window_ptr<tip_amount_f> form (new tip_amount_f(parentWnd, _("Enter tips (for reporting purposes only)"), total, false));
			int result = form->ShowModal();
			if (result == wxID_OK) {
				try {
					double tip = form->tip();
					ksi_cafe::cctrans_vec_t base_vec = ksi_cafe::fetch_cctrans_for_order(order_ptr->order_id());
					ksi_cafe::cctrans_record_t auth = ksi_cafe::payment_auth(base_vec, 2, order_ptr->prepayment_type_id());
					int txn_id = auth.txn_id();
					ksi_cafe::credit_payment_t* payment = dynamic_cast<ksi_cafe::credit_payment_t*>(order_ptr->cur_payment());
					payment->credit_add_to_batch(txn_id, 0, total, tip);
					order_ptr->set_close(true);
					return true;
				}
				catch (std::exception& ex) {
					*flog << ex.what();
					return false;
				}
			}
			return false;
		}
	}
	return false;
}

void cashier_f::close_order_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement close_order_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	
	*flog << "close_order_btnClick";
	if ((order->type_delivery() || order->type_pickup() || order->type_table_booking()) && !order->get_delivery_info()) {
		wxString msg = locale->get("global", "msg1_str", "Required delayed order properties are not filled, order processing is not available.");
		wxMessageBox(msg, _("Message"), wxOK, this);
		return;
	}
	if (order->order_id() == 0 && order->real_dish_count() == 0) {
		return;
	}
	if (order->type_table_booking() && !order->table().exist()) {
		return;
	}
	else if (order->order_id() == 0 && order->real_dish_count() == 0) {
		try {
			order->set_accept(0, true);
			order->set_simply_close(true);
			if (order->commit(true)) *flog << "!!! force apply";
			sg_clear(sg);
			order.reset();
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
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
		}
		EndModal(wxID_OK);
		return;
	}
	if (order->type_delivery() && order->subtotal() < order->delivery_min_subtotal()) {
		std::vector<wxString> cap_vec;
		cap_vec.push_back(_("Yes")); cap_vec.push_back(_("No"));
		std::vector<wxColour> color_vec;
		color_vec.push_back(ok_color); color_vec.push_back(close_color);
		wxString msg2_prefix = locale->get("global", "msg2_prefix", "The order subtotal is less than required for delivery (");
		wxString msg2_postfix = locale->get("global", "msg2_postfix", "). Create order anyway?");
		wxString message = msg2_prefix+locale->get_currency(order->delivery_min_subtotal())+msg2_postfix;
		wx_window_ptr<dlg_f> dlg (new dlg_f(this, _("Confirmation"), message, cap_vec, color_vec));
		int res = dlg->ShowModal();
		if (res == wxID_HIGHEST) { // yes
		}
		else if (res == wxID_HIGHEST+1) { // no
			return;
		}
		else return;
	}
	if (order->type_employee_meal() && order->def_subtotal() > order->meal_free_limit()) {
		std::vector<wxString> cap_vec;
		cap_vec.push_back(locale->get("global", "yes_str", "Yes")); cap_vec.push_back(locale->get("global", "no_str", "No"));
		std::vector<wxColour> color_vec;
		color_vec.push_back(ok_color); color_vec.push_back(close_color);
		wxString msg3_prefix = locale->get("global", "msg3_prefix", "Meal free limit has been exceeded on ");
		wxString msg3_postfix = locale->get("global", "msg3_postfix", ". Proceed?");
		wxString message =  msg3_prefix + locale->get_currency(order->def_subtotal()-order->meal_free_limit()) + msg3_postfix;
		wx_window_ptr<dlg_f> dlg (new dlg_f(this, locale->get("cashier_f", "employee_meal_btn", "Employee meal"), message, cap_vec, color_vec));
		
		int res = dlg->ShowModal();
		if (res == wxID_HIGHEST) { // yes
			if (!wx_mng_auth(this)) return;
		}
		else if (res == wxID_HIGHEST+1) { // no
			return;
		}
		else return;
	}
	
	vfd_clear_all(display.get());
	vfd_print(display.get(), 1, "Subtotal", to_mb(locale->get_currency(order->subtotal())), 0);
	vfd_print(display.get(), 2, "Total ", to_mb(locale->get_currency(order->total())), 0);
	
	int payment_type_id = 0;
	if (order->payment_credit_card()) payment_type_id = 2;
	else {
		wx_window_ptr<payment_f> payment_form (new payment_f(this, locale->get("cashier_f", "select_payment_type_str", "Select payment type"), order->type_employee_meal(), (stand_alone || client.get()) && order->subtotal() != 0));
		if (!order->type_employee_meal() && !((stand_alone || client.get()) && order->subtotal() != 0)) {
			payment_type_id = 1;
		}
		else {
			if (payment_form->ShowModal() != wxID_OK) return;
			payment_type_id = payment_form->result();
		}
	}
	*flog << "payment, payment_type_id = " + boost::lexical_cast<std::string>(payment_type_id);
	
	double cash = 0;
	double change = 0;
	
	if (payment_type_id == 1) { // Cash
		double total = order->total() > order->prepayment_amount() ? order->total() - order->prepayment_amount() : 0;
		wx_window_ptr<calc_f> form (new calc_f(this, locale->get("calc_f", "caption", "Cash payment"), total, false));
		bool cash_input = true;
		if (config->get("functionality", "cash_input") == "0" || to_uc(config->get("functionality", "cash_input")).MakeUpper() == _("OFF")) cash_input = false;
		if (total == 0) {
			cash_input = false;
			cash = 0;
			change = 0;
		}
		if (cash_input) if (form->ShowModal() != wxID_OK) return;
		cash = cash_input ? form->amount() : total;
		change = cash - total;
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
		catch (exception_t& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
			return;
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
		catch (exception_t& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
			return;
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			return;
		}
	}
	else if (payment_type_id == 2 && !stand_alone) { // Credit card		
		int auth_txn_id = 0;
		ksi_cafe::credit_payment_t* payment = 0;
		double total = order->total() > order->prepayment_amount() ? order->total() - order->prepayment_amount() : 0;
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
							//wxMessageBox(to_uc(boost::lexical_cast<std::string>(ex.msg_id())) + _(" ") + to_uc(ex.what()), _("Message"), wxOK, this);
							
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
					wxMessageBox(to_uc(boost::lexical_cast<std::string>(ex.msg_id())) + _(" ") + to_uc(ex.what()), _("Message"), wxOK, this);
					return;
				}
				catch (std::exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
							//wxMessageBox(to_uc(boost::lexical_cast<std::string>(ex.msg_id())) + _(" ") + to_uc(ex.what()), _("Message"), wxOK, this);
							
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
					wxMessageBox(to_uc(boost::lexical_cast<std::string>(ex.msg_id())) + _(" ") + to_uc(ex.what()), _("Message"), wxOK, this);
					return;
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
					wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
				}
				catch (std::exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				}
				return;
			}
			payment = new ksi_cafe::credit_payment_t(auth, total);
			
			
			bool successful_commit = false;
			try {
				order->set_payment_credit_card(payment);
				*flog << "ksi_cafe::order_t::set_accept";
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
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			}
			catch (ksi_client::db_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
				wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			}			
		}
		else if (order->payment_credit_card()){
			payment = dynamic_cast<ksi_cafe::credit_payment_t*>(order->cur_payment());
			auth_txn_id = payment->auth_txn_id();
		}
		
		wx_window_ptr<tip_amount_f> form (new tip_amount_f(this, _("Input tips"), total, false));
		int result = form->ShowModal();
		if (result == wxID_OK) { // Ok
			double tip = form->tip();
			bool successful_commit = false;
			bool pos_exceptionCathed = false;
			std::string pos_exceptionMessage = "";
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
						//wxMessageBox(to_uc(boost::lexical_cast<std::string>(ex.msg_id())) + _(" ") + to_uc(ex.what()), _("Message"), wxOK, this);
						
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
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				}
				catch (std::exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				}
				*flog << std::string("batch_id = ") + boost::lexical_cast<std::string>(batch_id);
				payment->credit_add_to_batch(txn_id, batch_id, total, tip);
				successful_commit = true;
			}
			catch (ksi_client::pos_exception& ex) {
				*flog << ex.what();
				pos_exceptionMessage = ex.what();
				pos_exceptionCathed = true;
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			}
			
			if (!successful_commit) {
				int order_id = order->order_id();
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
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				}
				catch (ksi_client::db_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
				}
				catch (exception_t& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
				}
				catch (std::exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				}
				if (!successful_oper) {
					EndModal(wxID_CANCEL);
					return;
				}
				sg_clear(sg);
				order.reset();
				*flog << "reloadOrder";
				ksi_cafe::order_t* ord_ptr = reloadOrder(this, order_id, *flog, *cafe_container);
				*flog << "reloadOrder.successful";
				order.reset(ord_ptr);
				if (order.get()) {
					order->clear_wasted();
					recalc_coupon();
					
					fixOrder(this, order.get(), pos_exceptionMessage);
					
					*flog << "ksi_cafe::order_t::commit(), order_id = " + boost::lexical_cast<std::string>(order_id);
					if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
					*flog << "commit successful";
					
					EndModal(wxID_CANCEL);
				}
				else {
					EndModal(wxID_CANCEL);
				}
				return;
			}
		}
		else if (result == wxID_MORE) { // Hide
			*flog << "hide";
			bool successful_commit = false;
			int order_id = order->order_id();
			try {
				*flog << "ksi_cafe::order_t::set_accept";
				order->set_accept(0, true);
				order_id = order->order_id();
				*flog << "successful";
				*flog << "ksi_cafe::order_t::commit(), order_id = " + boost::lexical_cast<std::string>(order_id);
				if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
				*flog << "commit successful";
				successful_commit = true;
			}
			catch (ksi_client::db_exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
			}
			catch (std::exception &ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			}
			if (!successful_commit) {
				EndModal(wxID_CANCEL);
				return;
			}
			sg_clear(sg);
			order.reset();
			*flog << "reloadOrder";
			ksi_cafe::order_t* ord_ptr = reloadOrder(this, order_id, *flog, *cafe_container);
			*flog << "reloadOrder.successful";
			order.reset(ord_ptr);
			if (order.get()) {
				order->clear_wasted();
				recalc_coupon();
				return;
			}
			else {
				EndModal(wxID_CANCEL);
				return;
			}
		}
		else { // Cancel
			bool pos_exceptionCathed = false;
			std::string pos_exceptionMessage = "";
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
						wxMessageBox(to_uc(ERROR_IN_LOCATION), _("Message"), wxOK, this);
						return;
					}
					std::string xml = print_csleep(*cafe_container->current_cafe(), curr_payment->num(), txn_id, auth_txn_id);
					print_xml(printer.get(), xml);
				}
				catch (std::exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				}
				wxMessageBox(locale->get("global", "credit_card_auth_has_been_void", "Credit card authorization has been void"), _("Message"), wxOK, this);
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
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				}
				catch (ksi_client::db_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
				}
				catch (exception_t& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
				}
				catch (std::exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				}
				if (!successful_commit) {
					*flog << WARNING_IN_LOCATION;
					return;
				}
				
				successful_commit = false;
				int order_id = order->order_id();
				try {
					*flog << "ksi_cafe::order_t::commit(), order_id = " + boost::lexical_cast<std::string>(order_id);
					if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
					*flog << "commit successful";
					successful_commit = true;
				}
				catch (ksi_client::connection_problem_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				}
				catch (ksi_client::db_exception& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
				}
				catch (exception_t& ex) {
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
				}
				if (!successful_commit) {
					*flog << WARNING_IN_LOCATION;
					EndModal(wxID_CANCEL);
					return;
				}
				sg_clear(sg);
				order.reset();
				*flog << "reloadOrder";
				ksi_cafe::order_t* ord_ptr = reloadOrder(this, order_id, *flog, *cafe_container);
				*flog << "reloadOrder.successful";
				order.reset(ord_ptr);
				if (order.get()) {
					order->clear_wasted();
					recalc_coupon();
				}
				else {
					EndModal(wxID_CANCEL);
					return;
				}
			}
			catch (ksi_client::pos_exception& ex) {
				*flog << ex.what();
				pos_exceptionMessage = ex.what();
				pos_exceptionCathed = true;
				//bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				//wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			}
			
			if (pos_exceptionCathed) {
				int order_id = order->order_id();
				fixOrder(this, order.get(), pos_exceptionMessage);
				
				*flog << "ksi_cafe::order_t::commit(), order_id = " + boost::lexical_cast<std::string>(order_id);
				if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
				*flog << "commit successful";
			}
			
			EndModal(wxID_OK);
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
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			logout();
		}
		catch (ksi_client::db_exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
		}
		catch (exception_t& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
		*flog << "ksi_cafe::order_t::commit(), order_id = " + boost::lexical_cast<std::string>(order->order_id());
		if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
		*flog << "commit successful";
		
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
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
			}
			if (!print_successful) {
				wxMessageBox(_("Не удалось напечатать фискальный ресит."), _("Message"), wxOK, this);
				EndModal(wxID_OK);
				return;
			}
		}
		
		std::string xml = print_order(*order, by_group, !stand_alone);
		print_xml(printer.get(), xml);
		sucessful_commit = true;
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
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
		+ locale->get("global", "has_been_successfully_created_str", " has been successfully closed.") 
		+ _("\n\n") 
		+ locale->get("global", "order_total_is_str", "order total is ") 
		+ locale->get_currency(order->total());
		wxString change_lb = _("");
		if (order->payment_cash()) change_lb = locale->get("global", "change_is_str", "Change is ") + locale->get_currency(change);
		wx_window_ptr<info_f> info (new info_f(this, locale->get("global", "Order_str", "Order") + _(" # ") + to_uc(order_id_str) + locale->get("global", "order_created_str", " closed"), info_lb, change_lb, order->payment_cash()));
		
		info->ShowModal();
		EndModal(wxID_OK);
	}
	else EndModal(wxID_OK);
}

void cashier_f::cancel_changes_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement cancel_changes_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	
	*flog << "cancel_changes";
	EndModal(wxID_OK);
}

void cashier_f::print_receipt_duplicate_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement print_receipt_duplicate_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }

	if (config->get("security", "print_receipt_duplicate") == "0" || to_uc(config->get("security", "print_receipt_duplicate")).MakeUpper() == _("OFF")) {
	}
	else {
		if (!wx_mng_auth(this)) return;
	}
	
	*flog << "print_receipt_duplicate_btnClick";
	if (lang == 1) wxMessageBox(_("All changes will be saved."), _("Message"), wxOK, this);
	try {
		//*flog << "ksi_cafe::order_t::set_accept";
		//order->set_accept(0, true);
		//*flog << "successful";
		//*flog << "ksi_cafe::order_t::commit(), order_id = " + boost::lexical_cast<std::string>(order->order_id());
		//if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
		//*flog << "commit successful";
		orderChanged_ = true;
		wxCommandEvent evt(0, 1);
		save_changes_btnClick(evt);
		if (!orderChanged_) {//&& order.get()) {
			std::string xml = print_order(*order, by_group, !stand_alone);
			print_xml(printer.get(), xml);
			EndModal(wxID_OK);
		}
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
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void cashier_f::other_orders_operations_btnClick( wxCommandEvent& event )
{
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	// TODO: Implement other_orders_operations_btnClick
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	*flog << "cashier_f::other_orders_operations_btnClick";
	
	std::vector<wxString> cap_vec;
	std::vector<wxColour> color_vec;
	std::vector<bool> checked_vec;
	std::vector<bool> enabled_vec;
	std::vector<bool> wrap_vec;
	
	cap_vec.push_back(locale->get("action_f", "split_order_btn", "Split order"));
	color_vec.push_back(btn_face_color);
	checked_vec.push_back(false);
	enabled_vec.push_back(true);
	wrap_vec.push_back(true);

	cap_vec.push_back(locale->get("action_f", "move_order_btn", "Transfer order to another table"));
	color_vec.push_back(btn_face_color);
	checked_vec.push_back(false);
	enabled_vec.push_back(true);
	wrap_vec.push_back(true);

	cap_vec.push_back(locale->get("action_f", "move_person_btn", "Transfer order to another waiter"));
	color_vec.push_back(btn_face_color);
	checked_vec.push_back(false);
	enabled_vec.push_back(true);
	wrap_vec.push_back(true);

	wxString caption = _("");
	wx_window_ptr<btn_list_f> form (new btn_list_f(this, caption, _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
	int modal_code = form->ShowModal() - wxID_HIGHEST;
	if (modal_code == 0) { // split
		*flog << "split order";
		wx_window_ptr<split_order_f> form (new split_order_f(this, _("Split order"), order.get()));
		int result = form->ShowModal();
		int order_id = order->order_id();
		sg_clear(sg);
		order.reset();
		*flog << "reloadOrder";
		ksi_cafe::order_t* ord_ptr = reloadOrder(this, order_id, *flog, *cafe_container);
		*flog << "reloadOrder.successful";
		order.reset(ord_ptr);
		if (order.get()) {
			order->clear_wasted();
			recalc_coupon();
		}
		else {
			EndModal(wxID_CANCEL);
			return;
		}
	}
	else if (modal_code == 1) { // move to another table
		std::auto_ptr<table_show_t> table_show (new table_show_t(std::vector<wxButton*>(), table_container.get(), content::REGULAR, true));
		wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Select a table"), _(""), table_show.get()));
		int result = form->ShowModal() - wxID_HIGHEST;
		if (result >= 0) {
			wxButton* table_btn = form->getTableBtn();
			ksi_cafe::table_t table = table_show->get_table(table_btn);
			order->change_table(table);
		}
	}
	else if (modal_code == 2) { // move to another person
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
		if (result > 0 && result < user_vec.size()) {
			order->change_creator_id(user_vec[result].user_id());
		}
	}
}

void cashier_f::menu_select_btnClick( wxCommandEvent& event ) {
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	*flog << "cashier_f::menu_select_btnClick";
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
				*flog << LLOCATION;
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
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
			last_menu_id = selected_menu->menu_id();
			menu_manager->select(selected_menu->menu_id(), order->table().table_group_id());
		}
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
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void cashier_f::menu_tree_btnClick( wxCommandEvent& event ) {
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
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
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void cashier_f::menu_item_btnClick( wxCommandEvent& event ) {
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
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
				wxMessageBox(msg, _("Message"), wxOK, this);
				return;
			}
			
			wx_window_ptr<add_modifiers_f> form (new add_modifiers_f(this, _("Select modifiers"), stop_list.get()));
			if (form->set_modifiers(menu_manager->selected(), move_up->get_menu_dish(btn))) {
				if (form->ShowModal() != wxID_OK) return;
			}
			orderChanged_ = true;
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
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void cashier_f::OnTimer (wxTimerEvent& event) {
	if (event.GetId() == timer_.timer_id()) {
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		current_dt_lb->SetLabel(locale->get_date_time(now));
	}
}

void cashier_f::OnSeanceTimer (wxTimerEvent& event) {
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
}

void cashier_f::sgSelChanged( wxTreeEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	
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
	}
}

void cashier_f::sgSelChanging( wxTreeEvent& event ) {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	
	comment_entree_btn->Enable(false);
	plus_btn->Enable(false);
	minus_btn->Enable(false);
}

void cashier_f::recalc_coupon() {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	
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
	
	try {
		sg_order_show(sg, *order.get(), type_check_sum, check_sum);
	}
	catch (...) {
		*flog << "... void cashier_f::sg_show(const std::string&, const std::string&)";
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, "... void cashier_f::sg_show(const std::string&, const std::string&)"));
		wxMessageBox(_("... void cashier_f::sg_show(const std::string&, const std::string&)"), _("Message"), wxOK, this);
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
	
	if (order->type_employee_meal()) {
		int employee_id = order->employee_id();
		std::string memo = order->memo();
		order->clear_employee();
		order->set_type_employee_meal(employee_id, memo);		
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
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, this)));
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
	if (!ksi_cafe::session_exist()) { logout(); }
	try {
		wx_window_ptr<coupon_list_f> form (new coupon_list_f(this, locale->get("cashier_f", "select_coupon_discout_str", "Select a coupon/discount"), locale->get("cashier_f", "coupon_msg1", "Green items are applicable to current order. Red ones are not. Yellow are system discounts."), order.get()));
		orderChanged_ = true;
		form->ShowModal();
		recalc_coupon("","");
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()), _("Message"), wxOK, this);
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
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
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}	
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, this);
	}
}

void cashier_f::logout() {
	wxCommandEvent event;
	logout_btnClick(event);
}

void cashier_f::apply_lang() {
	logout_btn->SetLabel(locale->get("cashier_f", "logout_btn", "Log out"));

	sg->GetColumn(0).SetText(locale->get("cashier_f", "sg_entree", "Entree"));
	sg->GetColumn(1).SetText(locale->get("cashier_f", "sg_price", "Price"));
	sg->GetColumn(2).SetText(locale->get("cashier_f", "sg_qty", "Qty"));
	sg->GetColumn(3).SetText(locale->get("cashier_f", "sg_amount", "Amount"));

	m_subtotal_lb->SetLabel(locale->get("cashier_f", "m_subtotal_lb", "Subtotal:"));
	m_sales_tax_lb->SetLabel(locale->get("cashier_f", "m_sales_tax_lb", "Sales tax:"));
	m_total_lb->SetLabel(locale->get("cashier_f", "m_total_lb", "Total: "));

}


std::pair<int, int> makeOrderListInfo() {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	if (users->user().manager() || users->user().superwaiter())
		query->create_statement
		(
		"SELECT SUM(open_order_cnt), SUM(delayed_order_cnt) "
		" FROM "
		" ( "
		" SELECT COUNT(order_id) as open_order_cnt, 0 as delayed_order_cnt "
		       " FROM ksi.orders "
		       " WHERE status_id IN (1,2,6) AND orders.cafe_id = ksi.util.get_cur_cafe_id "
		       " AND orders.type_id NOT IN (3,4,6) "
		       " AND seance_id IN (SELECT seance_id FROM ksi.seance WHERE shift_id = ksi.util.get_cur_shift_id) "
		" UNION "
		" SELECT 0 as open_order_cnt, COUNT(order_id) as delayed_order_cnt "
		       " FROM ksi.orders "
		       " WHERE status_id IN (1,2,6) AND orders.cafe_id = ksi.util.get_cur_cafe_id "
		       " AND orders.type_id IN (3, 4, 6) "
		" )"
		);
	else 
		query->create_statement
		(
		"SELECT SUM(open_order_cnt), SUM(delayed_order_cnt) "
		" FROM "
		" ( "
		" SELECT COUNT(order_id) as open_order_cnt, 0 as delayed_order_cnt "
		       " FROM ksi.orders "
		       " WHERE status_id IN (1,2,6) AND orders.cafe_id = ksi.util.get_cur_cafe_id "
		       " AND orders.type_id NOT IN (3,4,6) "
		       " AND seance_id IN (SELECT seance_id FROM ksi.seance WHERE shift_id = ksi.util.get_cur_shift_id) "
		       " AND creator_id = ksi.util.get_cur_user_id "
		" UNION "
		" SELECT 0 as open_order_cnt, COUNT(order_id) as delayed_order_cnt "
		       " FROM ksi.orders "
		       " WHERE status_id IN (1,2,6) AND orders.cafe_id = ksi.util.get_cur_cafe_id "
		       " AND orders.type_id IN (3, 4, 6) "
		" )"
		);
		
	query->execute_statement();
	if (query->next()) THROW_LOCATION(0, "std::pair<int, int> makeOrderListInfo()");
	
	int open_order_count = static_cast<int>(boost::get<double>(query->get(0)));
	int delayed_order_count = static_cast<int>(boost::get<double>(query->get(1)));
	return std::make_pair(open_order_count, delayed_order_count);
}

void cashier_f::refresh_order_list() {
	wx_disable_t disable_window(this);
	pause_timer_t<cashier_f> pause_seance_timer(seance_timer_);
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
		*flog << LLOCATION;
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
	
	top_bar_sizer->Layout();
}
