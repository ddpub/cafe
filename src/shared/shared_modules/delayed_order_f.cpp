#include "delayed_order_f.h"
#include <wx/msgdlg.h>
#include <wx_util/wx_util.hpp>
#include "text_f.h"
#include "input_num_f.h"
#include <ksi_util/format.hpp>
#include <memory>
#include <ksi_order/order_interface.hpp>
#include <vector>
#include <ksi_order/schedule_container.hpp>
#include "select_date_f.h"
#include <ksi_util/date_time.hpp>
#include "select_time_f.h"
#include <ksi_util/log.hpp>
#include <ksi_libpos/pos.hpp>
#include "calc_f.h"
#include <ksi_order/order_payment.hpp>
#include "payment_f.h"
#include <ksi_util/currency.hpp>
#include "btn_list_f.h"
#include <boost/lexical_cast.hpp>
#include "card_swipe_f.h"
#include "manual_entry_f.h"
#include <ksi_order/cafe_container.hpp>
#include "coupon_calc_f.h"
#include <oracle_client/ksi_exception.hpp>
#include <ksi_util/util.hpp>
#include "fin_load_f.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <printer/xml_check_printer.hpp>
#include "receipt.hpp"
#include <printer/printer.hpp>
#include "load_f.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <ksi_util/loader.hpp>
#include <ksi_order/order_payment.hpp>
#include <lang/xml_lang.h>
#include <memory>
#include <ksi_util/bag_spooler.hpp>
#include <wx_util/xml_config.hpp>
#include <wx_util/credit_utils.hpp>
#include <wx_util/order_show.hpp>

extern std::auto_ptr<file_log_t> flog;
extern std::auto_ptr<ksi_client::user_pos_i> client;
extern std::auto_ptr<ksi_cafe::cafe_container_t> cafe_container;
extern std::auto_ptr<base_printer_t> printer;
extern bool stand_alone;
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;
extern std::auto_ptr<ksi_cafe::xml_lang_t> shared_locale;
extern bool by_group;
extern std::auto_ptr<bag_spooler_t> bag_spooler;
extern int app_id;
extern std::auto_ptr<xml_config_t> config;
extern std::auto_ptr<ksi_cafe::table_container_t> table_container;

ksi_cafe::delivery_schedule_container_t delivery_schedule_container;
std::auto_ptr<ksi_cafe::cafe_schedule_t> cafe_schedule_ptr;

namespace version {
extern std::string version;
extern std::string compamny;
extern std::string product;
extern std::string copyright;
};

delayed_order_f::delayed_order_f( wxWindow* parent, const wxString& title, int type_id_, bool hide_exist, ksi_cafe::order_t* order)
: base_delayed_order_f( parent ), type_id_(type_id_), dummy_(false), order(order)
{
	SetLabel(title);
	if (!hide_exist) save_and_hide_btn->Hide();
	CentreOnParent();
	state_vec_ = ksi_cafe::fetch_state();
	for (size_t i=0; i<state_vec_.size(); ++i) {
		state_cb->Append(to_uc(state_vec_[i].name()));
	}

	if (config->get("functionality", "us_mode") == "0" || to_uc(config->get("functionality", "us_mode")).MakeUpper() == _("OFF"))
		usMode_ = false;
	else 
		usMode_ = true;

	firstname_lb->SetLabel(locale->get("delayed_order_f", "firstname_lb", "First name"));
	lastname_lb->SetLabel(locale->get("delayed_order_f", "lastname_lb", "Last name"));
	street_address_lb->SetLabel(locale->get("delayed_order_f", "street_address_lb", "Street address"));
	city_lb->SetLabel(locale->get("delayed_order_f", "city_lb", "City"));
	zip_lb->SetLabel(locale->get("delayed_order_f", "zip_lb", "ZIP"));
	phone_lb->SetLabel(locale->get("delayed_order_f", "phone_lb", "Phone"));
	yellow_lb->SetLabel(locale->get("delayed_order_f", "yellow_lb", "Mandatory fields are marked yellow"));
	delivery_pickup_cap->GetStaticBox()->SetLabel(locale->get("delayed_order_f", "delivery_pickup_cap", "Date and time"));
	set_date_btn->SetLabel(locale->get("delayed_order_f", "set_date_btn", "Set the date"));
	set_time_btn->SetLabel(locale->get("delayed_order_f", "set_time_btn", "Set the time"));
	note_cap->GetStaticBox()->SetLabel(locale->get("delayed_order_f", "note_cap", "Order notes"));
	prepayment_cap->GetStaticBox()->SetLabel(locale->get("delayed_order_f", "prepayment_cap", "Prepayment"));
	prepayment_btn->SetLabel(locale->get("delayed_order_f", "prepayment_btn", "Prepayment"));
	cancel_prepayment_btn->SetLabel(locale->get("delayed_order_f", "cancel_prepayment_btn", "Cancel prepayment"));
	auth_cc_btn->SetLabel(locale->get("delayed_order_f", "auth_cc_btn", "Authorize CC"));
	cancel_auth_cc_btn->SetLabel(locale->get("delayed_order_f", "cancel_auth_cc_btn", "Cancel authorize CC"));
	wrap_label(cancel_auth_cc_btn);
	print_slip_btn->SetLabel(locale->get("delayed_order_f", "print_slip_btn", "Print slip"));
	cancel_order_btn->SetLabel(locale->get("delayed_order_f", "cancel_order_btn", "Cancel order"));
	save_btn->SetLabel(locale->get("delayed_order_f", "save_btn", "Save"));
	save_and_print_btn->SetLabel(locale->get("delayed_order_f", "save_and_print_btn", "Save and print \nreceipt copy"));
	save_and_hide_btn->SetLabel(locale->get("delayed_order_f", "save_and_hide_btn", "Save and hide order"));
	wrap_label(save_and_hide_btn);
	cancel_btn->SetLabel(locale->get("delayed_order_f", "cancel_btn", "Cancel"));

	if (state_vec_.size() > 0 && state_vec_.size() > 5) state_cb->SetStringSelection(to_uc(state_vec_[5].name()));
	else if (state_vec_.size() > 0) state_cb->SetStringSelection(to_uc(state_vec_[0].name()));
	if (!delivery_schedule_container.loaded()) delivery_schedule_container.load();
	if (!cafe_schedule_ptr.get()) cafe_schedule_ptr.reset(new ksi_cafe::cafe_schedule_t());
	
	if (type_id_ == 4) {
		street_address_lb->Hide();
		address_ed->Hide();
		city_lb->Hide();
		city_ed->Hide();
		zip_lb->Hide();
		zip_ed->Hide();
		state_lb->Hide();
		state_cb->Hide();
		set_date_btn->Enable(true);
	}
	else if (type_id_ == 6) {
		street_address_lb->Hide();
		address_ed->Hide();
		city_lb->Hide();
		city_ed->Hide();
		zip_lb->Hide();
		zip_ed->Hide();
		state_lb->Hide();
		state_cb->Hide();
		set_date_btn->Enable(true);
		select_table_btn->Show(true);
		table_.reset(new ksi_cafe::table_t(order->table()));
		
		if (table_->exist()) {
			select_table_btn->SetLabel(locale->get("delayed_order_f", "table_str", "Table: ") + to_uc(order->table().name()));
		}
		else {
			select_table_btn->SetLabel(locale->get("delayed_order_f", "table_str", "Table: ") + locale->get("delayed_order_f", "not_selected_str", "not selected"));
		}
		
		this->Layout();
		main_sizer->Fit( this );
	}
	if (order->get_delivery_info()) {
		save_and_hide_btn->Enable(true);
		fname_ed->SetValue(to_uc(order->get_delivery_info()->firstname()));
		lname_ed->SetValue(to_uc(order->get_delivery_info()->lastname()));
		state_cb->SetStringSelection(to_uc(order->get_delivery_info()->state()));
		if (type_id_ == 3) zip_ed->SetValue(to_uc(boost::lexical_cast<std::string>(order->get_delivery_info()->zip())));
		phone_ed->SetValue(to_uc(order->get_delivery_info()->phone()));
		address_ed->SetValue(to_uc(order->get_delivery_info()->streetaddress()));
		city_ed->SetValue(to_uc(order->get_delivery_info()->city()));
		selected_date_ = order->get_delivery_info()->delivery_time().date();
		set_date(selected_date_);
		selected_time_ = order->get_delivery_info()->delivery_time().time_of_day();
		set_time(selected_time_);
		if (boost::posix_time::second_clock::local_time() > order->get_delivery_info()->delivery_time()) {
			dummy_ = true;
		}
		set_date_btn->Enable(true);
		set_time_btn->Enable(true);
	}

	if (!usMode_) {
		state_lb->Hide();
		state_cb->Hide();
		zip_ed->SetBackgroundColour( wxColour( 255, 255, 255 ) );
		set_date_btn->Enable(true);
	}
	change_item();
}

void delayed_order_f::fname_edClick( wxMouseEvent& event )
{
	// TODO: Implement finame_edClick
	*flog << "delayed_order_f::fname_edClick";
	wx_window_ptr<text_f> form (new text_f(this, firstname_lb->GetLabel(), _(""), 0));
	form->text(fname_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	fname_ed->SetValue(form->text());
	change_item();
}

void delayed_order_f::lname_edClick( wxMouseEvent& event )
{
	// TODO: Implement lname_edClick
	*flog << "delaed_order_f::lname_edClick";
	wx_window_ptr<text_f> form (new text_f(this, lastname_lb->GetLabel(), _(""), 0));
	form->text(lname_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	lname_ed->SetValue(form->text());
	change_item();
}

void delayed_order_f::address_edClick( wxMouseEvent& event )
{
	// TODO: Implement address_edClick
	*flog << "delayed_order_f::address_edClick";
	wx_window_ptr<text_f> form (new text_f(this, street_address_lb->GetLabel(), _(""), 0));
	form->text(address_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	address_ed->SetValue(form->text());
	change_item();
}

void delayed_order_f::city_edClick( wxMouseEvent& event )
{
	// TODO: Implement city_edClick
	*flog << "delayed_order_f::city_edClick";
	wx_window_ptr<text_f> form (new text_f(this, city_lb->GetLabel(), _(""), 0));
	form->text(city_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	city_ed->SetValue(form->text());
	change_item();
}

void delayed_order_f::set_date(const boost::gregorian::date& date) {
	*flog << "delayed_order_f::set_date, date = " + to_simple_string(date); 
	if (date.is_not_a_date()) {
		//set_date_btn->SetLabel(_("Set the date"));
		set_date_btn->SetLabel(locale->get("delayed_order_f", "set_date_btn", "Set the date"));
	}
	else {
		set_date_btn->SetLabel(locale->get_date(boost::posix_time::ptime(date, boost::posix_time::time_duration())));
	}
}

void delayed_order_f::set_time(const boost::posix_time::time_duration& time) {
	*flog << "delayed_order_f::set_time";
	if (time == boost::posix_time::not_a_date_time) {
		//set_time_btn->SetLabel(_("Set the time"));
		set_time_btn->SetLabel(locale->get("delayed_order_f", "set_time_btn", "Set the time"));
	}
	else {
		set_time_btn->SetLabel(locale->get_time(boost::posix_time::ptime(selected_date_, time)));
	}
}

void delayed_order_f::zip_edClick( wxMouseEvent& event )
{
	// TODO: Implement zip_edClick
	*flog << "delayed_order_f::zip_edClick";
	wx_window_ptr<input_num_f> form (new input_num_f(this, zip_lb->GetLabel(), false));
	wxString last_zip = zip_ed->GetValue();
	form->set_format_value(zip_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	zip_ed->SetValue(form->format_text());
	wxString curr_zip = zip_ed->GetValue();
	if (usMode_ && last_zip != curr_zip) {
		selected_date_ = boost::gregorian::date(boost::gregorian::not_a_date_time);
		set_date(selected_date_);
		selected_time_ = boost::posix_time::time_duration(boost::posix_time::not_a_date_time);
		set_time(selected_time_);
		set_time_btn->Enable(false);
		save_btn->Enable(false);
		save_and_print_btn->Enable(false);
		save_and_hide_btn->Enable(false);
		set_date_btn->Enable(true);
		if (curr_zip != _("")) set_date_btn->Enable(true);
	}
}

void delayed_order_f::change_item() {
	prepayment_btn->Enable(false);
	cancel_prepayment_btn->Enable(false);
	auth_cc_btn->Enable(false);
	cancel_auth_cc_btn->Enable(false);
	if (fname_ed->GetValue() != _("") && phone_ed->GetValue() != _("") 
	&& !selected_date_.is_not_a_date()
	&& selected_time_ != boost::posix_time::not_a_date_time
	&& ((type_id_ == 3 && usMode_) ? zip_ed->GetValue() != _("") : true)) {
		if ((type_id_ == 6) ? table_->exist() : true) {
			save_btn->Enable(true);
			save_and_print_btn->Enable(true);
			save_and_hide_btn->Enable(true);
			set_date_btn->Enable(true);
			if (order->prepayment_empty() && order->payment_empty()) {
				prepayment_btn->Enable(true);
				auth_cc_btn->Enable(true && client.get());
				cancel_prepayment_btn->Enable(false);
				cancel_auth_cc_btn->Enable(false);
			}
			else if (order->prepayment_empty() && !order->payment_empty()) {
				prepayment_btn->Enable(false);
				auth_cc_btn->Enable(false);
				cancel_prepayment_btn->Enable(false);
				cancel_auth_cc_btn->Enable(true);
			}
			else if (!order->prepayment_empty() && order->payment_empty()) {
				prepayment_btn->Enable(false);
				auth_cc_btn->Enable(false);
				cancel_prepayment_btn->Enable(false);
				cancel_auth_cc_btn->Enable(false);
			}
		}
		select_table_btn->Enable(true);
	}
	else {
		save_btn->Enable(false);
		save_and_print_btn->Enable(false);
		save_and_hide_btn->Enable(false);
		cancel_prepayment_btn->Enable(false);
		auth_cc_btn->Enable(false);
		cancel_auth_cc_btn->Enable(false);
		select_table_btn->Enable(false);
	}
	
	if (order->prepayment_cash()) {
		wxString type_name = locale->get("global", "type_cash", "Cash");
		prepayment_btn->SetLabel(locale->get("delayed_order_f", "prepayment_btn", "Prepayment") + _("\n") + locale->get_currency(order->prepayment_amount()) + _(" ") + type_name);		
		prepayment_btn->Enable(false);
		cancel_prepayment_btn->Enable(true);
		auth_cc_btn->SetLabel(locale->get("delayed_order_f", "auth_cc_btn", "Authorize CC"));
		auth_cc_btn->Enable(false);
		cancel_auth_cc_btn->Enable(false);
	}
	else if (order->prepayment_credit_card()) {
		wxString type_name = locale->get("global", "type_cc", "CC");
		prepayment_btn->SetLabel(locale->get("delayed_order_f", "prepayment_btn", "Prepayment") + _("\n") + locale->get_currency(order->prepayment_amount()) + _(" ") + type_name);
		prepayment_btn->Enable(false);
		cancel_prepayment_btn->Enable(true);
		auth_cc_btn->SetLabel(locale->get("delayed_order_f", "auth_cc_btn", "Authorize CC"));
		auth_cc_btn->Enable(false);
		cancel_auth_cc_btn->Enable(false);
	}
	else if (order->payment_credit_card()) {
		wxString type_name = locale->get("global", "type_cc", "CC");
		auth_cc_btn->SetLabel(locale->get("delayed_order_f", "auth_cc_btn", "Authorize CC") + _("\n") + locale->get_currency(order->payment_amount()) + _(" ") + type_name);
		auth_cc_btn->Enable(false);
		cancel_auth_cc_btn->Enable(true);
		prepayment_btn->Enable(false);
		cancel_prepayment_btn->Enable(false);
	}
	else {
		prepayment_btn->SetLabel(locale->get("delayed_order_f", "prepayment_btn", "Prepayment"));
		auth_cc_btn->SetLabel(locale->get("delayed_order_f", "auth_cc_btn", "Authorize CC"));
	}
	
	if (order->prepayment_empty()) {
		prepayment_btn->SetLabel(locale->get("delayed_order_f", "prepayment_btn", "Prepayment"));		
	}
	else {
		wxString type_name = _("");
		if (order->prepayment_cash()) type_name = locale->get("global", "type_cash", "Cash");
		else if (order->prepayment_credit_card()) type_name = locale->get("global", "type_cc", "CC");
		prepayment_btn->SetLabel(locale->get("delayed_order_f", "prepayment_btn", "Prepayment") + _("\n") + locale->get_currency(order->prepayment_amount()) + _(" ") + type_name);
		cancel_prepayment_btn->Enable(true);
		auth_cc_btn->Enable(false);
		cancel_auth_cc_btn->Enable(false);
	}
	
	order_notes_ed->SetValue(to_uc(order->memo()));
	
	if (order->payment_credit_card() || order->prepayment_credit_card()) {
		print_slip_btn->Enable(true);
	}
	else {
		print_slip_btn->Enable(false);
	}
}

void delayed_order_f::select_table_btnClick( wxCommandEvent& event ) {
	// TODO: Implement select_table_btnClick
	std::auto_ptr<table_show_t> table_show (new table_show_t(std::vector<wxButton*>(), table_container.get(), content::BOOKING, true, boost::posix_time::ptime(selected_date_, selected_time_)));
	wx_window_ptr<btn_list_f> form (new btn_list_f(this, locale->get("global", "select_a_table_str", "Select a table"), _(""), table_show.get()));
	int result = form->ShowModal() - wxID_HIGHEST;
	if (result >= 0) {
		wxButton* table_btn = form->getTableBtn();
		ksi_cafe::table_t table = table_show->get_table(table_btn);
		table_.reset(new ksi_cafe::table_t(table));
		if (table_->exist()) {
			select_table_btn->SetLabel(locale->get("delayed_order_f", "table_str", "Table: ") + to_uc(table_->name()));
		}
		else {
			select_table_btn->SetLabel(locale->get("delayed_order_f", "table_str", "Table: ") + locale->get("delayed_order_f", "not_selected_str", "not selected"));
		}
	}
	change_item();
}

void delayed_order_f::state_cbChoice( wxCommandEvent& event )
{
	// TODO: Implement state_cbChoice
	*flog << "delayed_order_f::state_cbChoice";
	change_item();
}

void delayed_order_f::phone_edClick( wxMouseEvent& event )
{
	// TODO: Implement phone_edClick
	*flog << "delayed_order_f::phone_edClick";
	wx_window_ptr<input_num_f> form (new input_num_f(this, phone_lb->GetLabel(), ksi_cafe::en_phone_format_t()));
	form->set_format_value(phone_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	phone_ed->SetValue(form->format_text());
	change_item();
}

void delayed_order_f::set_date_btnClick( wxCommandEvent& event )
{
	// TODO: Implement set_date_btnClick
	*flog << "delayed_order_f::set_date_btnClick";
	dummy_ = false;
	boost::posix_time::ptime curr = boost::posix_time::second_clock::local_time();
	
	boost::gregorian::date curr_date = curr.date();
	boost::posix_time::time_duration curr_time_of_day = curr.time_of_day();

	std::vector<boost::gregorian::date> vec_date;
	std::vector<int> vec_enable;
	bool date_exist = false;
		
	for (size_t i=0; 1; ++i) {
		boost::gregorian::date next_date = curr_date + boost::gregorian::date_duration(i);
		if (i && next_date > curr_date + boost::gregorian::months(1)) break;
		
		vec_date.push_back(next_date);
		if (usMode_ && type_id_ == 3) {
			int zip = boost::lexical_cast<int>(to_mb(zip_ed->GetValue()));
			boost::posix_time::ptime start_time = delivery_schedule_container.start_time(zip, next_date.day_of_week());
			boost::posix_time::ptime stop_time = delivery_schedule_container.stop_time(zip, next_date.day_of_week());
			if (start_time != boost::posix_time::not_a_date_time && stop_time != boost::posix_time::not_a_date_time) {
				vec_enable.push_back(1);
				date_exist = true;
			}
			else vec_enable.push_back(0);
		}
		else if (type_id_ == 4 || (!usMode_ && type_id_ == 3) || type_id_ == 6) {
			boost::posix_time::ptime start_time = cafe_schedule_ptr->start_time(next_date.day_of_week());
			boost::posix_time::ptime stop_time = cafe_schedule_ptr->stop_time(next_date.day_of_week());
			if (start_time != boost::posix_time::not_a_date_time && stop_time != boost::posix_time::not_a_date_time) {
				vec_enable.push_back(1);
				date_exist = true;
			}
			else vec_enable.push_back(0);			
		}
		else vec_enable.push_back(0);
	}

	
	wxString form_cap = locale->get("global", "pick_the_delivery_date_str", "Pick the delivery date");
	if (type_id_ == 4) form_cap = locale->get("global", "pick_the_pickup_date_str", "Pick the pickup date");
	else if (type_id_ == 6) form_cap = locale->get("global", "pick_the_table_booking_date_str", "Pick the table booking date");
	if (date_exist) {
		wx_window_ptr<select_date_f> form (new select_date_f(this, form_cap, vec_date, vec_enable, selected_date_));
		if (form->ShowModal() == wxID_OK) {
			if (form->selected().is_not_a_date()) THROW_LOCATION(0, "void delayed_order_f::set_date_btnClick( wxCommandEvent&)");
			selected_date_ = form->selected();
			set_date(selected_date_);
			set_time_btn->Enable(true);
			selected_time_ = boost::posix_time::time_duration(boost::posix_time::not_a_date_time);
			set_time(selected_time_);
		}
	}
	else {
		wxString message = _("The zip code you entered is not in the delivery schedule. Are you sure?");
		if (order->type_pickup()) message = _("Working days and hours, don't exist. Are you sure?");
		wxMessageDialog dlg(this, message, _("Confirmation"), wxYES_NO);
		dlg.Centre(wxBOTH);
		if (dlg.ShowModal() == wxID_YES) {
			dummy_ = true;
			for (size_t i=0; i<vec_enable.size(); ++i) vec_enable[i] = 1;
			wx_window_ptr<select_date_f> form (new select_date_f(this, form_cap, vec_date, vec_enable, selected_date_));
			if (form->ShowModal() == wxID_OK) {
				if (form->selected().is_not_a_date()) THROW_LOCATION(0, "void delayed_order_f::set_date_btnClick( wxCommandEvent&)");
				selected_date_ = form->selected();
				set_date(selected_date_);
				set_time_btn->Enable(true);
				selected_time_ = boost::posix_time::time_duration(boost::posix_time::not_a_date_time);
				set_time(selected_time_);
			}
		}
	}
	change_item();
}

void delayed_order_f::set_time_btnClick( wxCommandEvent& event )
{
	// TODO: Implement set_time_btnClick
	*flog << "delayed_order_f::set_time_btnClick";
	boost::posix_time::ptime start_time, stop_time;
	if (dummy_) {
		start_time = cafe_schedule_ptr->start_time(selected_date_.day_of_week());
		stop_time = cafe_schedule_ptr->stop_time(selected_date_.day_of_week());
	}
	else if (usMode_ && type_id_ == 3) {
		int zip = boost::lexical_cast<int>(to_mb(zip_ed->GetValue()));
		start_time = delivery_schedule_container.start_time(zip, selected_date_.day_of_week());
		stop_time = delivery_schedule_container.stop_time(zip, selected_date_.day_of_week());
	}
	else if (type_id_ == 4 || (!usMode_ && type_id_ == 3) || type_id_ == 6) {
		start_time = cafe_schedule_ptr->start_time(selected_date_.day_of_week());
		stop_time = cafe_schedule_ptr->stop_time(selected_date_.day_of_week());
	}
	wxString form_cap = locale->get("global", "pick_the_delivery_time_str", "Pick the delivery time");
	if (type_id_ == 4) form_cap = locale->get("global", "pick_the_pickup_time_str", "Pick the pickup time");
	else if (type_id_ == 6) form_cap = locale->get("global", "pick_the_table_booking_time_str", "Pick the table booking time");
	wx_window_ptr<select_time_f> form (new select_time_f(this, form_cap, boost::posix_time::time_period(boost::posix_time::ptime(selected_date_, start_time.time_of_day()), boost::posix_time::ptime(selected_date_, stop_time.time_of_day())), order->cafe().setup_time(type_id_)));
	if (form->ShowModal() == wxID_OK) {
		if (form->selected() == boost::posix_time::not_a_date_time) THROW_LOCATION(0, "void delayed_order_f::set_time_btnClick( wxCommandEvent&)");
		selected_time_ = form->selected().time_of_day();
		set_time(selected_time_);
	}
	change_item();
}

void delayed_order_f::prepayment_btnClick( wxCommandEvent& event )
{
	// TODO: Implement prepayment_btnClick
	*flog << "delayed_order_f::prepayment_btnClick";
	if (!order->prepayment_empty()) return;
	
	if (!order->type_table_booking() && order->real_dish_count() == 0) {
		wxMessageBox(locale->get("delayed_order_f", "please_make_the_order_first_str", "Please make the order first."));
		return;
	}
	
	int prepayment_type_id = 0;
	wx_window_ptr<payment_f> prepayment_form (new payment_f(this, locale->get("delayed_order_f", "prepayment_cap", "Prepayment"), order->type_employee_meal(), client.get()));
	if (prepayment_form->ShowModal() != wxID_OK) return;
	prepayment_type_id = prepayment_form->result();
	*flog << "prepayment, prepayment_type_id = " + boost::lexical_cast<std::string>(prepayment_type_id);
	
	if (prepayment_type_id == 1) { // Cash
		wx_window_ptr<input_num_f> sum_form (new input_num_f(this, locale->get("delayed_order_f", "prepayment_cap", "Prepayment amount"), true));
		do {
			if (sum_form->ShowModal() != wxID_OK) return;
			if (int(sum_form->value()*100) == 0) return;
			if (type_id_ != 6 && sum_form->value() > order->total())
				wxMessageBox(locale->get("global", "maximum_prepayment_amount_is_str", "Maximum prepayment amount is ") + locale->get_currency(order->total()));
		} while (type_id_ != 6 && sum_form->value() > order->total());
		double prepayment_amount = sum_form->value();
		
		ksi_cafe::cash_prepayment_t* prepayment = new ksi_cafe::cash_prepayment_t(prepayment_amount);
		bool catch_exception = true;
		try {
			*flog << "ksi_cafe::order_t::set_prepayment_cash";
			order->set_prepayment_cash(prepayment);
			*flog << "successful";
			*flog << "ksi_cafe::order_t::set_accept";
			order->set_accept(0, true);
			*flog << "successful";
			prepayment->payment(order->order_id(), true);
			*flog << "prepayment_cash" + to_mb(locale->get_currency(prepayment_amount));
			catch_exception = false;
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			wxMessageBox(to_uc(ex.what()));
			return;
		}
		if (catch_exception) {
			/** cancel all */
		}
	}
	else if (prepayment_type_id == 2) { // Credit Card
		int auth_txn_id = 0;
		ksi_cafe::credit_prepayment_t* prepayment = 0;
		wx_window_ptr<input_num_f> sum_form (new input_num_f(this, locale->get("delayed_order_f", "prepayment_cap", "Prepayment amount"), true));
		do {
			if (sum_form->ShowModal() != wxID_OK) return;
			if (int(sum_form->value()*100) == 0) return;
			if (sum_form->value() > order->total())
				wxMessageBox(locale->get("global", "maximum_prepayment_amount_is_str", "Maximum prepayment amount is ") + locale->get_currency(order->total()));
		} while (sum_form->value() > order->total());
		double prepayment_amount = sum_form->value();	
			
		std::vector<wxString> cap_vec; std::vector<wxColour> color_vec; std::vector<bool> enabled_vec; std::vector<bool> checked_vec; std::vector<bool> wrap_vec;
		cap_vec.push_back(_("AUTHORIZE CC")); cap_vec.push_back(_("Manual Entry"));
		color_vec.push_back(btn_face_color); color_vec.push_back(btn_face_color);
		enabled_vec.push_back(true); enabled_vec.push_back(true);
		checked_vec.push_back(false); checked_vec.push_back(false);
		wrap_vec.push_back(true); wrap_vec.push_back(true);
		wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Credit Card prepayment"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
		int result = form->ShowModal() - wxID_HIGHEST;
		std::string cc_buff_;
		bool swiped_ = true;
		try {
			if (result == 0) { // Authorize cc 
				swiped_ = true;
				*flog << "prepayment_credit_card, authorize cc";
				wx_window_ptr<card_swipe_f> form (new card_swipe_f(this, _(""), _("AUTHORIZE CC")));
				if (form->ShowModal() != wxID_OK) return;
				cc_buff_ = form->buff();
				*flog << "credit_auth, cc_buff_ = " + ksi_cafe::masked_buff(cc_buff_);
				/**
				auth_txn_id = client->credit_auth(prepayment_amount, 0, cc_buff_);
				**/
				ksi_client::pos_exception* except = 0;
				wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), _("Communicating with processing server, please wait.")));
				client->set_do_report(true);
				boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_auth, client.get(), prepayment_amount, 0, cc_buff_);
				fin_form->start(boost::bind(cc_processing, &auth_txn_id, &except, f));
				fin_form->ShowModal();
				if (except != 0) throw *except;
				*flog << "successful, auth_txn_id = " + boost::lexical_cast<std::string>(auth_txn_id);
			}
			else if (result == 1) { // manual Entry
				swiped_ = false;
				*flog << "prepayment_credit_card, manual entry";
				wx_window_ptr<manual_entry_f> form (new manual_entry_f(this, _("Manual Entry")));
				if (form->ShowModal() != wxID_OK) return;
				cc_buff_ = ";"+to_mb(form->card_num())+"=";
				*flog << "credit_auth, cc_buff_ = " + ksi_cafe::masked_buff(cc_buff_);
				int exp_month = boost::lexical_cast<int>(to_mb(form->exp_month()));
				int exp_year = boost::lexical_cast<int>(to_mb(form->exp_year()));
				/**
				auth_txn_id = client->credit_auth(prepayment_amount, 0, to_mb(form->card_num()), exp_month, exp_year, to_mb(form->cvv2()), true, true);
				**/
				ksi_client::pos_exception* except = 0;
				wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), _("Communicating with processing server, please wait.")));
				client->set_do_report(true);
				boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_auth, client.get(), prepayment_amount, 0, to_mb(form->card_num()), exp_month, exp_year, to_mb(form->cvv2()), true, true);
				fin_form->start(boost::bind(cc_processing, &auth_txn_id, &except, f));
				fin_form->ShowModal();
				if (except != 0) throw *except;
				*flog << "successful, auth_txn_id = " + boost::lexical_cast<std::string>(auth_txn_id);
			}
			else return;
			
			*flog << "ksi_client::pos_user_i::get_card_type";
			std::string cc_type = client->get_card_type(auth_txn_id);
			*flog << "successful";
			*flog << "ksi_client::pos_user_i::get_card_holder_firstname";
			std::string cardHolder_fname = client->get_card_holder_first_name(auth_txn_id);
			*flog << "successful";
			*flog << "ksi_client::pos_user_i::get_card_holder_last_name";
			std::string cardHolder_lname = client->get_card_holder_last_name(auth_txn_id);
			*flog << "successful";
			std::string msk_card_num = ksi_cafe::first6andlast4digits(cc_buff_.substr(cc_buff_.find(";")+1, cc_buff_.find("=")-cc_buff_.find(";")-1));
			ksi_cafe::card_info_t card_info(msk_card_num, cc_type, cardHolder_fname + " " + cardHolder_lname, true);
			ksi_cafe::credit_auth_t auth(card_info, auth_txn_id, client->get_auth_code(auth_txn_id));	
			prepayment = new ksi_cafe::credit_prepayment_t(auth, prepayment_amount);
			*flog << "ksi_cafe::order_t::set_prepayment_credit_card";
			order->set_prepayment_credit_card(prepayment);
			*flog << "successful";
			*flog << "ksi_cafe::order_t::set_accept";
			order->set_accept(0, true);
			*flog << "successful";
			*flog << "auth_payment";
			prepayment->auth_payment(order->order_id(), true);
			*flog << "successful";
			try {
				std::string msg = print_msleep(*cafe_container->current_cafe(), order->order_id(), prepayment_amount, cc_type, msk_card_num, cardHolder_fname + " " + cardHolder_lname, auth_txn_id, swiped_);
				print_xml(printer.get(), msg);
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
			}
		}
		catch (ksi_client::pos_exception& ex) {
			*flog << ex.what();
			if (auth_txn_id == 0) {
				try {
					std::string msk_card_num = ksi_cafe::first6andlast4digits(cc_buff_.substr(cc_buff_.find(";")+1, cc_buff_.find("=")-cc_buff_.find(";")-1));
					std::string msg = print_csleep(*cafe_container->current_cafe(), msk_card_num, ex.what(), /*client->get_last_txn_id()*/0, true);
					print_xml(printer.get(), msg);
				}
				catch (exception_t& ex) {
					*flog << ex.what();
					wxMessageBox(to_uc(ex.what()));
				}
			}
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			wxMessageBox(to_uc(ex.what()));
		}
		if (auth_txn_id != 0) {
			try {
				*flog << "add_to_batch. auth_txn_id = " + boost::lexical_cast<std::string>(auth_txn_id);
				int txn_id = client->credit_add_to_batch(auth_txn_id, prepayment_amount, 0);
				*flog << "txn_id = " + boost::lexical_cast<std::string>(txn_id);
				int batch_id = client->get_current_batch_id();
				*flog << "batch_id = "+ boost::lexical_cast<std::string>(batch_id);
				prepayment->credit_add_to_batch(txn_id, batch_id, prepayment_amount, 0);
				*flog << "add_to_batch_payment";
				prepayment->payment(order->order_id(), true);
				*flog << "prepayment_credit_card " + to_mb(locale->get_currency(prepayment_amount));
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
			}
		}
	}
	
	if (!order->prepayment_empty()) {
		if (!order->type_delivery() || !order->type_pickup() || !order->type_table_booking()) {
			try {
				set_delivery_pickup_info();
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
			}
		}
		
		bool catch_exception = false;
		int res = 0;
		ksi_cafe::order_t* ord_ptr = 0;
		int order_id = order->order_id();
		ksi_cafe::table_t table = order->table();
		ksi_cafe::token_t token = order->token();
		*flog << "ksi_cafe::order_t::commit, order_id = " + boost::lexical_cast<std::string>(order_id);
		if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
		*flog << "commit successful";
		delete order;
		order = 0;
		try {
			*flog << "load commited order_t, order_id = " + boost::lexical_cast<std::string>(order_id);
			wx_window_ptr<load_f> form (new load_f(this, locale->get("global", "order_load_str", "Order is loading"), locale->get("global", "please_wait_str", "Please wait.\nIf order is not opened for a long time, it might have been already opened by somebody else.")));
			form->start(boost::bind(load_order5, &ord_ptr, *cafe_container->current_cafe(), table, token, order_id, true, false));
			res = form->ShowModal();
		}
		catch (std::exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			wxMessageBox(to_uc(ex.what()));
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
				if (ord_ptr->status_accepted() || ord_ptr->status_ready() || ord_ptr->status_send_to_kitchen()) {
					order = ord_ptr;
				}
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
		if (order) order->clear_wasted();
		else EndModal(wxID_CANCEL);
		
		wxString type_name = _("");
		if (order->prepayment_cash()) type_name = locale->get("global", "type_cash", "Cash");
		else if (order->prepayment_credit_card()) type_name = locale->get("global", "type_cc", "CC");
		prepayment_btn->SetLabel(locale->get("delayed_order_f", "prepayment_btn", "Prepayment") + _("\n") + locale->get_currency(order->prepayment_amount()) + _(" ") + type_name);
		prepayment_btn->Enable(false);
		cancel_prepayment_btn->Enable(true);
		auth_cc_btn->Enable(false);
		cancel_auth_cc_btn->Enable(false);
	}
	else {
		prepayment_btn->SetLabel(locale->get("delayed_order_f", "prepayment_btn", "Prepayment"));
		prepayment_btn->Enable(true);
		cancel_prepayment_btn->Enable(false);
		auth_cc_btn->Enable(true);
		cancel_auth_cc_btn->Enable(false);
	}
	save_btn->Enable(true);
	save_and_print_btn->Enable(true);
	save_and_hide_btn->Enable(true);
	if (order->payment_credit_card() || order->prepayment_credit_card()) {
		print_slip_btn->Enable(true);
	}
	else {
		print_slip_btn->Enable(false);
	}
}

void delayed_order_f::cancel_prepayment_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_prepayment_btnClick
	wxString message = locale->get("global", "this_will_cancel_the_prepayment_continue_str", "This will cancel the prepayment. Continue?");
	wxMessageDialog dlg(this, message, _("Confirmation"), wxYES_NO);
	dlg.Centre(wxBOTH);
	if (dlg.ShowModal() != wxID_YES) return;
	*flog << "delayed_order_f::cancel_prepayment_btnClick";
	int void_transaction_id = 0;
	try {
		if (order->prepayment_credit_card() && !stand_alone) {
			if (!client.get()) THROW_MESSAGE(1, "client.get() == 0", "void delayed_order_f::cancel_prepayment_btnClick( wxCommandEvent& )");
			cctrans_vec_t base_vec = fetch_cctrans_for_order(order->order_id());
			int current_batch_id = client->get_current_batch_id();
			cctrans_record_t auth_rec = prepayment_auth(base_vec, 1, 2);
			cctrans_record_t add_to_batch_rec = prepayment_add_to_batch(base_vec, 1, 2);
			int batch_id = add_to_batch_rec.batch_id();
			if (batch_id == current_batch_id) { // credit void
				*flog << "credit_void, auth_txn_id = " + boost::lexical_cast<std::string>(auth_rec.txn_id());
				try {
					ksi_client::pos_exception* except = 0;
					wx_window_ptr<fin_load_f> fin_form (new fin_load_f(0, _(""), _("Communicating with processing server, please wait.")));
					boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_void, client.get(), auth_rec.txn_id());
					fin_form->start(boost::bind(cc_processing, &void_transaction_id, &except, f));
					fin_form->ShowModal();
					if (except != 0) throw *except;	
					*flog << "successful";
				}
				catch (ksi_client::pos_exception& ex) {
					*flog << ex.what();
					wxMessageBox(to_uc(ex.what()));
					return;
				}
				catch (std::exception& ex) {
					*flog << ex.what();
					wxMessageBox(to_uc(ex.what()));
					return;
				}
				
				//credit_void_fn(order_id, batch_id, add_to_batch_rec, auth_rec, refund_note);
			}
			else { // credit refund
				*flog << "credit_refund";
				std::vector<wxString> cap_vec;
				std::vector<wxColour> color_vec;
				std::vector<bool> enabled_vec;
				std::vector<bool> checked_vec;
				std::vector<bool> wrap_vec;
				
				cap_vec.push_back(_("AUTHORIZE CC")); cap_vec.push_back(_("Manual Entry"));
				color_vec.push_back(btn_face_color); color_vec.push_back(btn_face_color);
				enabled_vec.push_back(true); enabled_vec.push_back(true);
				checked_vec.push_back(false); checked_vec.push_back(false); 
				wrap_vec.push_back(true);
				wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Credit Card payment"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
				int result = form->ShowModal() - wxID_HIGHEST;
				
				int txn_id = 0;
				std::string cc_buff_;
				double total = 0;
				if (result == 0) { // Authorize cc
					*flog << "credit return, auth";
					wx_window_ptr<card_swipe_f> form (new card_swipe_f(this, _(""), locale->get("global", "AUTHORIZE_CC_str", "AUTHORIZE CC")));
					if (form->ShowModal() != wxID_OK) return;
					cc_buff_ = form->buff();
					*flog << "credit_return, cc_buff_ = " + cc_buff_;
					client->set_do_report(true);
					/**
					txn_id = client->credit_return(cc_buff_, total);
					*/
					ksi_client::pos_exception* except = 0;
					wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), locale->get("global", "communication_with_processing_server_please_wait", "Communicating with processing server, please wait.")));
					boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_return, client.get(), total, cc_buff_);
					fin_form->start(boost::bind(cc_processing, &txn_id, &except, f));
					fin_form->ShowModal();
					if (except != 0) throw *except;
					*flog << "successful, txn_id = " + boost::lexical_cast<std::string>(txn_id);
				}
				else if (result == 1) { // Manual entry
					*flog << "credit return, manual entry";
					wx_window_ptr<manual_entry_f> form (new manual_entry_f(this, locale->get("global", "Manual_entry_str", "Manual Entry")));
					if (form->ShowModal() != wxID_OK) return;
					std::string cc_num = to_mb(form->card_num());
					cc_buff_ = ";" + cc_num + "=";
					int exp_month = boost::lexical_cast<int>(to_mb(form->exp_month()));
					int exp_year = boost::lexical_cast<int>(to_mb(form->exp_year()));
					std::string cvv2 = to_mb(form->cvv2());
					*flog << "credit_return, cc_buff_ = " + cc_buff_;
					client->set_do_report(true);
					/**
					txn_id = client->credit_return(cc_num, exp_month, exp_year, cvv2, total);
					*/
					ksi_client::pos_exception* except = 0;
					wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), locale->get("global", "communication_with_processing_server_please_wait", "Communicating with processing server, please wait.")));
					boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_return, client.get(), total, cc_num, exp_month, exp_year, cvv2, true, true);
					fin_form->start(boost::bind(cc_processing, &txn_id, &except, f));
					fin_form->ShowModal();
					*flog << "successful, txn_id = " + boost::lexical_cast<std::string>(txn_id);
				}
				else return;
				
				try {
					current_batch_id = client->get_current_batch_id();
				}
				catch (ksi_client::pos_exception& ex) {
					*flog << ex.what();
					wxMessageBox(to_uc(ex.what()));
				}
				
				std::string cc_type = client->get_card_type(txn_id);
				std::string cardHolder_fname = client->get_card_holder_first_name(txn_id);
				std::string cardHolder_lname = client->get_card_holder_last_name(txn_id);
				std::string msk_card_num = ksi_cafe::first6andlast4digits(cc_buff_.substr(cc_buff_.find(";")+1, cc_buff_.find("=")-cc_buff_.find(";")-1));
				ksi_cafe::card_info_t card_info(msk_card_num, cc_type, cardHolder_fname + " " + cardHolder_lname, true);
				ksi_cafe::credit_return_t credit_return(card_info, txn_id, current_batch_id);
				
				//credit_refund_fn(this, order_id, current_batch_id, add_to_batch_rec, refund_note);
			}
		}
		order->set_prepayment_empty(void_transaction_id);
		if (order->order_id() != 0) {
			int order_id = order->order_id();
			*flog << std::string("order commit. order_id = ") + boost::lexical_cast<std::string>(order_id);
			if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
			*flog << "commit successful";
			delete order;
			order = 0;
			*flog << "reloadOrder";
			order = reloadOrder(this, order_id, *flog, *cafe_container);
			*flog << "reloadOrder.successful";
			if (order) order->clear_wasted();
			else EndModal(wxID_CANCEL);
		}
/*
		if (order->prepayment_credit_card()) {
			try {
				int auth_txn_id = dynamic_cast<ksi_cafe::credit_prepayment_t*>(order->cur_prepayment())->auth_txn_id();
				*flog << "credit_void, auth_txn_id = " + boost::lexical_cast<std::string>(auth_txn_id);
				
				//txn_id = client->credit_void(dynamic_cast<ksi_cafe::credit_prepayment_t*>(order->cur_prepayment())->auth_txn_id());
				
				ksi_client::pos_exception* except = 0;
				wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), locale->get("global", "communication_with_processing_server_please_wait", "Communicating with processing server, please wait.")));
				boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_void, client.get(), auth_txn_id);
				fin_form->start(boost::bind(cc_processing, &txn_id, &except, f));
				fin_form->ShowModal();
				if (except != 0) throw *except;	
				*flog << "successful";
			}
			catch (ksi_client::pos_exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
				return;
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
				return;
			}
			
			try {
				std::string msk_card_num = ksi_cafe::first6andlast4digits(dynamic_cast<ksi_cafe::credit_prepayment_t*>(order->cur_prepayment())->num());
				int auth_txn_id = dynamic_cast<ksi_cafe::credit_prepayment_t*>(order->cur_prepayment())->auth_txn_id();
				std::string msg = print_csleep(*cafe_container->current_cafe(), msk_card_num, txn_id, auth_txn_id);
				print_xml(printer.get(), msg);
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
			}
			
			wxMessageBox(_("There is an error occured during saving order. Credit card transaction was rolled back"));
			*flog << std::string("credit_void txn_id = ") + boost::lexical_cast<std::string>(txn_id);
		}

		order->set_prepayment_empty(void_transaction_id);
		if (order->order_id() != 0) {
			bool catch_exception = false;
			int res = 0;
			ksi_cafe::order_t* ord_ptr = 0;
			if (order->real_dish_count() == 0) order->sync_order_created();
			int order_id = order->order_id();
			ksi_cafe::table_t table = order->table();
			ksi_cafe::token_t token = order->token();
			*flog << std::string("order commit. order_id = ") + boost::lexical_cast<std::string>(order_id);
			if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
			*flog << "commit successful";
			delete order;
			order = 0;
			try {
				*flog << "load commited order_t, order_id = " + boost::lexical_cast<std::string>(order_id);
				wx_window_ptr<load_f> form (new load_f(this, locale->get("global", "order_load_str", "Order is loading"), locale->get("global", "please_wait_str", "Please wait.\nIf order is not opened for a long time, it might have been already opened by somebody else.")));
				form->start(boost::bind(load_order5, &ord_ptr, *cafe_container->current_cafe(), table, token, order_id, true, false));
				res = form->ShowModal();
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
				if (ord_ptr) {
					*flog << LLOCATION;
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
					if (ord_ptr->status_accepted() || ord_ptr->status_ready() || ord_ptr->status_send_to_kitchen()) {
						order = ord_ptr;
					}
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
			if (order) order->clear_wasted();
			else EndModal(wxID_CANCEL);
		}
*/
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}

	change_item(); 
}

void delayed_order_f::auth_cc_btnClick( wxCommandEvent& event )
{
	// TODO: Implement auth_cc_btnClick
	*flog << "delayed_order_f::auth_cc_btnClick";
	if (!order->payment_empty()) return;
	
	if (order->real_dish_count() == 0) {
		wxMessageBox(locale->get("delayed_order_f", "please_make_the_order_first_str", "Please make the order first."));
		return;
	}

	int auth_txn_id = 0;
	ksi_cafe::credit_payment_t* payment = 0;
	wx_window_ptr<input_num_f> sum_form (new input_num_f(this, locale->get("delayed_order_f", "prepayment_cap", "Prepayment amount"), true));
	do {
		if (sum_form->ShowModal() != wxID_OK) return;
		if (int(sum_form->value()*100) == 0) return;
		if (sum_form->value() > order->total())
			wxMessageBox(locale->get("global", "maximum_prepayment_amount_is_str", "Maximum prepayment amount is ") + locale->get_currency(order->total()));
	} while (sum_form->value() > order->total());
	double prepayment_amount = sum_form->value();
	
	std::vector<wxString> cap_vec; std::vector<wxColour> color_vec; std::vector<bool> enabled_vec; std::vector<bool> checked_vec; std::vector<bool> wrap_vec;
	cap_vec.push_back(_("AUTHORIZE CC")); cap_vec.push_back(_("Manual Entry"));
	color_vec.push_back(btn_face_color); color_vec.push_back(btn_face_color);
	enabled_vec.push_back(true); enabled_vec.push_back(true);
	checked_vec.push_back(false); checked_vec.push_back(false);
	wrap_vec.push_back(true); wrap_vec.push_back(true);
	wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Credit Card prepayment"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
	int result = form->ShowModal() - wxID_HIGHEST;
	std::string cc_buff_;
	bool swiped_ = true;
	try {
		if (result == 0) { // Authorize CC
			swiped_ = true;
			*flog << "authorize_credit_card, authorize cc";
			wx_window_ptr<card_swipe_f> form (new card_swipe_f(this, _(""), _("AUTHORIZE CC")));
			if (form->ShowModal() != wxID_OK) return;
			cc_buff_ = form->buff();
			*flog << "credit_auth, cc_buff_ = " + ksi_cafe::masked_buff(cc_buff_);
			/**
			auth_txn_id = client->credit_auth(prepayment_amount, 0, cc_buff_);
			**/
			ksi_client::pos_exception* except = 0;
			wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), _("Communicating with processing server, please wait.")));
			client->set_do_report(true);
			boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_auth, client.get(), prepayment_amount, 0, cc_buff_);
			fin_form->start(boost::bind(cc_processing, &auth_txn_id, &except, f));
			fin_form->ShowModal();
			if (except != 0) throw *except;
			*flog << "successful, auth_txn_id = " + boost::lexical_cast<std::string>(auth_txn_id);
		}
		else if (result == 1) { // manual Entry
			swiped_ = false;
			*flog << "authorize_credit_card, manual entry";
			wx_window_ptr<manual_entry_f> form (new manual_entry_f(this, _("Manual Entry")));
			if (form->ShowModal() != wxID_OK) return;
			cc_buff_ = ";"+to_mb(form->card_num())+"=";
			*flog << "credit_auth, cc_buff_ = " + ksi_cafe::masked_buff(cc_buff_);
			int exp_month = boost::lexical_cast<int>(to_mb(form->exp_month()));
			int exp_year = boost::lexical_cast<int>(to_mb(form->exp_year()));
			/**
			auth_txn_id = client->credit_auth(prepayment_amount, 0, to_mb(form->card_num()), exp_month, exp_year, to_mb(form->cvv2()), true, true);
			**/
			ksi_client::pos_exception* except = 0;
			wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), _("Communicating with processing server, please wait.")));
			client->set_do_report(true);
			boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_auth, client.get(), prepayment_amount, 0, to_mb(form->card_num()), exp_month, exp_year, to_mb(form->cvv2()), true, true);
			fin_form->start(boost::bind(cc_processing, &auth_txn_id, &except, f));
			fin_form->ShowModal();
			if (except != 0) throw *except;
			*flog << "successful, auth_txn_id = " + boost::lexical_cast<std::string>(auth_txn_id);
		}
		else return;
		
		*flog << "ksi_client::pos_user_i::get_card_type";
		std::string cc_type = client->get_card_type(auth_txn_id);
		*flog << "successful";
		*flog << "ksi_client::pos_user_i::get_card_holder_firstname";
		std::string cardHolder_fname = client->get_card_holder_first_name(auth_txn_id);
		*flog << "successful";
		*flog << "ksi_client::pos_user_i::get_card_holder_last_name";
		std::string cardHolder_lname = client->get_card_holder_last_name(auth_txn_id);
		*flog << "successful";
		std::string msk_card_num = ksi_cafe::first6andlast4digits(cc_buff_.substr(cc_buff_.find(";")+1, cc_buff_.find("=")-cc_buff_.find(";")-1));
		ksi_cafe::card_info_t card_info(msk_card_num, cc_type, cardHolder_fname + " " + cardHolder_lname, true);
		ksi_cafe::credit_auth_t auth(card_info, auth_txn_id, client->get_auth_code(auth_txn_id));
		payment = new ksi_cafe::credit_payment_t(auth, prepayment_amount);
		*flog << "ksi_cafe::order_t::set_prepayment_credit_card";
		order->set_payment_credit_card(payment);
		*flog << "successful";
		*flog << "ksi_cafe::order_t::set_accept";
		order->set_accept(0, true);
		*flog << "successful";
		*flog << "auth_payment";
		payment->auth_payment(order->order_id(), true);
		*flog << "successful";
		try {
			std::string msg = print_msleep(*cafe_container->current_cafe(), order->order_id(), prepayment_amount, cc_type, msk_card_num, cardHolder_fname + " " + cardHolder_lname, auth_txn_id, swiped_);
			print_xml(printer.get(), msg);
		}
		catch (exception_t& ex) {
			*flog << ex.what();
			wxMessageBox(to_uc(ex.what()));
		}
	}
	catch (ksi_client::pos_exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
		if (auth_txn_id == 0) {
			try {
				std::string msk_card_num = ksi_cafe::first6andlast4digits(cc_buff_.substr(cc_buff_.find(";")+1, cc_buff_.find("=")-cc_buff_.find(";")-1));
				std::string msg = print_csleep(*cafe_container->current_cafe(), msk_card_num, ex.what(), /*client->get_last_txn_id()*/0, true);
				print_xml(printer.get(), msg);
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
			}		
		}
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	
	if (!order->payment_empty()) {
		if (!order->type_delivery() || !order->type_pickup() || !order->type_table_booking()) {
			try {
				set_delivery_pickup_info();
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
			}
		}
		bool catch_exception = false;
		int res = 0;
		ksi_cafe::order_t* ord_ptr = 0;
		int order_id = order->order_id();
		ksi_cafe::table_t table = order->table();
		ksi_cafe::token_t token = order->token();
		*flog << "ksi_cafe::order_t::commit, order_id = " + boost::lexical_cast<std::string>(order_id);
		if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
		*flog << "commit successful";
		delete order;
		order = 0;
		try {
			*flog << "load commited order_t, order_id = " + boost::lexical_cast<std::string>(order_id);
			wx_window_ptr<load_f> form (new load_f(this, locale->get("global", "order_load_str", "Order is loading"), locale->get("global", "please_wait_str", "Please wait.\nIf order is not opened for a long time, it might have been already opened by somebody else.")));
			form->start(boost::bind(load_order5, &ord_ptr, *cafe_container->current_cafe(), table, token, order_id, true, false));
			res = form->ShowModal();
		}
		catch (std::exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			wxMessageBox(to_uc(ex.what()));
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
				if (ord_ptr->status_accepted() || ord_ptr->status_ready() || ord_ptr->status_send_to_kitchen()) {
					order = ord_ptr;
				}
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
		if (order) order->clear_wasted();
		else EndModal(wxID_CANCEL);		
	}
	change_item();
}

void delayed_order_f::cancel_auth_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_auth_btnClick
	*flog << "void delayed_order_f::cancel_auth_btnClick";
	wxString message = locale->get("global", "this_will_cancel_the_authorize_continue_str", "This will cancel the authorize. Continue?");
	wxMessageDialog dlg(this, message, _("Confirmation"), wxYES_NO);
	dlg.Centre(wxBOTH);
	if (dlg.ShowModal() != wxID_YES) return;
	int txn_id = 0;
	try {
		if (order->payment_credit_card()) {
			try {
				int auth_txn_id = dynamic_cast<ksi_cafe::credit_payment_t*>(order->cur_payment())->auth_txn_id();
				*flog << "ciredt_void, auth_txn_id = " + boost::lexical_cast<std::string>(auth_txn_id);
				/**
				int txn_id = client->credit_void(auth_txn_id);
				**/
				ksi_client::pos_exception* except = 0;
				wx_window_ptr<fin_load_f> fin_form (new fin_load_f(this, _(""), locale->get("global", "communication_with_processing_server_please_wait", "Communicating with processing server, please wait.")));
				boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_void, client.get(), auth_txn_id);
				fin_form->start(boost::bind(cc_processing, &txn_id, &except, f));
				fin_form->ShowModal();
				if (except != 0) throw *except;	
				*flog << "successful";
			}
			catch (ksi_client::pos_exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
				return;
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
				return;
			}
			
			try {
				ksi_cafe::credit_payment_t* curr_payment = dynamic_cast<ksi_cafe::credit_payment_t*>(order->cur_payment());
				std::string msk_card_num = ksi_cafe::first6andlast4digits(curr_payment->num());
				int auth_txn_id = curr_payment->auth_txn_id();
				std::string msg = print_csleep(*cafe_container->current_cafe(), msk_card_num, txn_id, auth_txn_id);
				print_xml(printer.get(), msg);
			}
			catch (exception_t& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
			}
			
			wxMessageBox(_("There is an error occured during saving order. Credit card transaction was rolled back"));
			*flog << std::string("credit_void txn_id = ") + boost::lexical_cast<std::string>(txn_id);
		}
		order->set_payment_empty(txn_id);
		if (order->order_id() != 0) {
			bool catch_exception = false;
			int res = 0;
			ksi_cafe::order_t* ord_ptr = 0;
			if (order->real_dish_count() == 0) order->sync_order_created();
			int order_id = order->order_id();
			ksi_cafe::table_t table = order->table();
			ksi_cafe::token_t token = order->token();
			*flog << std::string("order commit. order_id = ") + boost::lexical_cast<std::string>(order_id);
			if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
			*flog << "commit successful";
			delete order;
			order = 0;
			try {
				*flog << "load commited order_t, order_id = " + boost::lexical_cast<std::string>(order_id);
				wx_window_ptr<load_f> form (new load_f(this, locale->get("global", "order_load_str", "Order is loading"), locale->get("global", "please_wait_str", "Please wait.\nIf order is not opened for a long time, it might have been already opened by somebody else.")));
				form->start(boost::bind(load_order5, &ord_ptr, *cafe_container->current_cafe(), table, token, order_id, true, false));
				res = form->ShowModal();
			}
			catch (std::exception& ex) {
				*flog << LLOCATION;
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
				if (ord_ptr) {
					*flog << LLOCATION;
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
					if (ord_ptr->status_accepted() || ord_ptr->status_ready() || ord_ptr->status_send_to_kitchen()) {
						order = ord_ptr;
					}
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
			if (order) order->clear_wasted();
			else EndModal(wxID_CANCEL);
		}
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	change_item();
}

void delayed_order_f::print_sleep_btnClick( wxCommandEvent& event )
{
	// TODO: Implement print_sleep_btnClick
}

void delayed_order_f::cancel_order_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_order_btnClick
	std::vector<wxString> cap_vec; std::vector<wxColour> color_vec; std::vector<bool> enabled_vec; std::vector<bool> checked_vec; std::vector<bool> wrap_vec;
	cap_vec.push_back(locale->get("global", "charge_cancel_fee_str", "Charge cancel fee"));
	cap_vec.push_back(locale->get("global", "do_not_charge_anything_str", "Do not charge anything"));
	color_vec.push_back(btn_face_color); color_vec.push_back(btn_face_color);
	enabled_vec.push_back(true); enabled_vec.push_back(true);
	checked_vec.push_back(false); checked_vec.push_back(false);
	wrap_vec.push_back(true); wrap_vec.push_back(true);
	wx_window_ptr<btn_list_f> form (new btn_list_f(this, locale->get("global", "delayed_order_cancel_str", "Delayed order cancel"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
	int result = form->ShowModal() - wxID_HIGHEST;
	
	if (result == 0) {
		double prepayment_amount = order->prepayment_amount();
		if (order->payment_credit_card()) prepayment_amount = order->total();
		wx_window_ptr<coupon_calc_f> form (new coupon_calc_f(this, prepayment_amount, locale->get("global", "order_subtotal_precent_str", "Order subtotal precent"), locale->get("global", "specified_amount_str", "Specified amount")));
		if (form->ShowModal() != wxID_OK) return;
		double fee = form->discount_amount();
		int res;
		if (ksi_cafe::round(fee) < ksi_cafe::round(prepayment_amount)) {
			std::vector<wxString> cap_vec; std::vector<wxColour> color_vec; std::vector<bool> enabled_vec; std::vector<bool> checked_vec; std::vector<bool> wrap_vec;
			cap_vec.push_back(locale->get("global", "type_cash", "Cash"));
			cap_vec.push_back(locale->get("global", "type_cc", "Credit card"));
			color_vec.push_back(btn_face_color); color_vec.push_back(btn_face_color);
			enabled_vec.push_back(true); enabled_vec.push_back(true && client.get());

			checked_vec.push_back(false); checked_vec.push_back(false);
			wrap_vec.push_back(true); wrap_vec.push_back(true);
			wx_window_ptr<btn_list_f> form (new btn_list_f(this, _(""), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
			res = form->ShowModal() - wxID_HIGHEST + 1;
		}
		else if (ksi_cafe::round(fee) == ksi_cafe::round(prepayment_amount)) {
			if (order->payment_credit_card()) res = 2;
			else if (order->prepayment_cash()) res = 1;
			else if (order->prepayment_credit_card()) res = 2;
		}
		try {
			if (res == 1) { // Cash
				if (order->payment_credit_card()) {
					ksi_cafe::credit_payment_t* payment = dynamic_cast<ksi_cafe::credit_payment_t*>(order->cur_payment());
					int txn_id = client->credit_void(payment->auth_txn_id());
					wxMessageBox(_("Credit card authorization has been void"));
					payment->cancel(order->order_id(), true);
					ksi_cafe::cash_cancel_fee_t(fee).cancel_fee(order->order_id(), true);
				}
				 else if (!order->prepayment_empty()) ksi_cafe::cash_cancel_fee_t(fee).cancel_fee(order->order_id(), true);
			}
			else if (res == 2) { // Credit Card
			    if (order->payment_credit_card()) {
			      ksi_cafe::credit_payment_t* payment = dynamic_cast<ksi_cafe::credit_payment_t*>(order->cur_payment());
			      int txn_id = client->credit_add_to_batch(payment->auth_txn_id(), fee, 0);
			      int batch_id = client->get_current_batch_id();
			      ksi_cafe::credit_cancel_fee_t(payment->cur_auth(), txn_id, batch_id, fee).cancel_fee(order->order_id(), true);
			    }
			}
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
		}

		if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
	}
	else if (result == 1) {
		if (order->payment_credit_card()) {
			try {
				ksi_cafe::credit_payment_t* payment = dynamic_cast<ksi_cafe::credit_payment_t*>(order->cur_payment());
				int txn_id = client->credit_void(payment->auth_txn_id());
				wxMessageBox(_("Credit card authorization has been void"));
				payment->cancel(order->order_id(), true);	        
				ksi_cafe::dummy_cancel_fee_t().cancel_fee(order->order_id(), true);
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
				wxMessageBox(to_uc(ex.what()));
			}
		}
		else ksi_cafe::dummy_cancel_fee_t().cancel_fee(order->order_id(), true);

		if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
	}
	
	EndModal(wxID_UNDO);
}

void delayed_order_f::order_notes_edClick( wxMouseEvent& event )
{
	// TODO: Implement order_notes_edClick
	wx_window_ptr<text_f> form (new text_f(this, _("Order notes"), _(""), 0));
	form->text(order_notes_ed->GetValue());
	if (form->ShowModal() != wxID_OK) return;
	order_notes_ed->SetValue(form->text());
	order->set_memo(to_mb(form->text()));
}

void delayed_order_f::save_and_print_btnClick( wxCommandEvent& event ) {	
	try {
		set_delivery_pickup_info();
		if (order->order_id() != 0) {
			if (order->real_dish_count() == 0) order->sync_order_created();
			int order_id = order->order_id();
			ksi_cafe::table_t table = order->table();
			ksi_cafe::token_t token = order->token();
			*flog << std::string("order commit. order_id = ") + boost::lexical_cast<std::string>(order_id);
			if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
			*flog << "commit successful";
			*flog <<  "print delayed order, order_id = " + boost::lexical_cast<std::string>(order->order_id());
			//std::string xml = print_receipt_duplicate(*cafe_container->current_cafe(), 2, order->order_id(), shared_locale->get_std("receipt", "receipt_duplicate_str", "DUPLICATE"), false, !stand_alone);
			std::string xml = print_order(*order, by_group, !stand_alone);
			delete order;
			order = 0;
			order = new ksi_cafe::order_t(*cafe_container->current_cafe(), table, token, order_id, true, false);
			order->clear_wasted(); 
			*flog << "printing";
			print_xml(printer.get(), xml);
			*flog << "print successful"; 
		}
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}

	EndModal(wxID_OK);
}

void delayed_order_f::save_btnClick( wxCommandEvent& event )
{
	// TODO: Implement save_btnClick
	try {
		set_delivery_pickup_info();
		if (order->order_id() != 0) {
			if (order->real_dish_count() == 0) order->sync_order_created();
			int order_id = order->order_id();
			ksi_cafe::table_t table = order->table();
			ksi_cafe::token_t token = order->token();
			*flog << std::string("order commit. order_id = ") + boost::lexical_cast<std::string>(order_id);
			if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
			*flog << "commit successful";
			delete order;
			order = 0;
			order = new ksi_cafe::order_t(*cafe_container->current_cafe(), table, token, order_id, true, false);
			order->clear_wasted();  
		}
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}

	EndModal(wxID_OK);
}

void delayed_order_f::save_and_hide_btnClick( wxCommandEvent& event )
{
	// TODO: Implement save_and_hide_btnClick
	try {
		set_delivery_pickup_info();
		if (order->real_dish_count() == 0) order->sync_order_created();
		int order_id = order->order_id();
		*flog << std::string("order commit. order_id = ") + boost::lexical_cast<std::string>(order_id);
		if (!order->commit(true)) *flog << "ksi_cafe::order_t(bool force), force apply";
		*flog << "commit successful";
		delete order;
		order = 0;
		order = new ksi_cafe::order_t(*cafe_container->current_cafe(), 0);
		order->clear_wasted();  
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(cafe_container->current_cafe()->cafe_id(), app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
	}
	EndModal(wxID_CLOSE);
}

void delayed_order_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

void delayed_order_f::set_delivery_pickup_info() {
	if (type_id_ == 3) {
		if (state_cb->GetCurrentSelection() >= state_vec_.size()) THROW_LOCATION(1, "void delayed_order_f::set_delivery_pickup_info()"); 
		ksi_cafe::state_t state = state_vec_[state_cb->GetCurrentSelection()];
		order->set_type_delivery(
		new ksi_cafe::delivery_info_t(
		to_mb(fname_ed->GetValue()),
		to_mb(lname_ed->GetValue()),
		state.state_id(),
		state.name(),
		zip_ed->GetValue() == _("") ? 0 : boost::lexical_cast<int>(to_mb(zip_ed->GetValue())),
		to_mb(address_ed->GetValue()),
		to_mb(phone_ed->GetValue()),
		boost::posix_time::ptime(selected_date_, selected_time_),
		to_mb(city_ed->GetValue()))
		);
		order->change_table(ksi_cafe::table_t(order->table().table_group_id(), order->table().table_group_name()));
	}
	else if (type_id_ == 4) {
		order->set_type_pickup(
		new ksi_cafe::delivery_info_t(
		to_mb(fname_ed->GetValue()),
		to_mb(lname_ed->GetValue()),
		0,
		"",
		0,
		"",
		to_mb(phone_ed->GetValue()),
		boost::posix_time::ptime(selected_date_, selected_time_),
		"")
		);
		order->change_table(ksi_cafe::table_t(order->table().table_group_id(), order->table().table_group_name()));
	}
	else if (type_id_ == 6) {
		order->set_type_table_booking(
		new ksi_cafe::delivery_info_t(
		to_mb(fname_ed->GetValue()),
		to_mb(lname_ed->GetValue()),
		0,
		"",
		0,
		"",
		to_mb(phone_ed->GetValue()),
		boost::posix_time::ptime(selected_date_, selected_time_),
		"")
		);
		order->change_table(*table_.get());
	}
	else THROW_LOCATION(0, "void delayed_order_f::set_delivery_pickup_info()");
	if (order->real_dish_count() == 0) order->sync_order_created();
	order->set_accept(0, true);
}
