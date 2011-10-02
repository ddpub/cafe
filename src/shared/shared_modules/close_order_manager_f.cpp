#include "close_order_manager_f.h"
#include <wx/msgdlg.h>
#include "input_num_f.h"
#include <ksi_include/ksi_exceptions.hpp>
#include <ksi_include/wx_tree_node.hpp>
#include <ksi_include/custom_treelistctrl.hpp>
#include <ksi_util/users.hpp>
#include <ksi_util/log.hpp>
#include <wx_util/wx_util.hpp>
#include <memory>
#include <shared_modules/connect.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ksi_order/cafe_container.hpp>
#include "text_f.h"
#include <boost/lexical_cast.hpp>
#include <ksi_include/algo_functor.hpp>
#include <boost/bind.hpp>
#include <ksi_libpos/pos.hpp>
#include "btn_list_f.h"
#include "card_swipe_f.h"
#include "manual_entry_f.h"
#include "receipt.hpp"
#include <wx_util/xml_config.hpp>
#include "wx_authorise.hpp"
#include <lang/xml_lang.h>
#include <fiscal_register/fiscal.hpp>
#include <wx_util/credit_utils.hpp>
#include <printer/printer.hpp>
#include "receipt.hpp"

extern std::auto_ptr<ksi_cafe::users_t> users;
extern std::auto_ptr<file_log_t> flog;
extern std::auto_ptr<base_printer_t> printer;
extern std::auto_ptr<ksi_cafe::cafe_container_t> cafe_container;
extern std::auto_ptr<ksi_client::user_pos_i> client;
extern std::auto_ptr<xml_config_t> config;
extern bool by_group;
extern bool stand_alone;
extern int lang;
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;
extern std::auto_ptr<ksi_cafe::xml_lang_t> shared_locale;
extern std::auto_ptr<fiscal::registrator_t> registrar;
extern int app_id;


wxString wxRefundTreeListCtrl::OnGetItemText (wxTreeItemData* item_data, long column) const {
	try {
		close_order_t* node = dynamic_cast<close_order_t*>(item_data);
		if (!node) THROW_LOCATION(0, "wxString wxRefundTreeListCtrl::OnGetItemText(wxTreeItemData*, long) const");
		if (column == 0) {
			return to_uc(boost::lexical_cast<std::string>(node->col1())) + _(" ");
		}
		else if (column == 1) {
			return locale->get_date_time(node->col2());
		}
		else if (column == 2) {
			return locale->get_currency(node->col3());
		}
		else if (column == 3) {
			return _(" ") + to_uc(node->col5());
		}
		else if (column == 4) {
			return to_uc(node->col6());
		}
		else if (column == 5) {
			return to_uc(node->col7());
		}
		else if (column == 6) {
			if (node->col8() == 0) return _("");
			return to_uc(users->user(node->col8()).fio());
		}
	}
	catch (exception_t& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
	}
	return _("exception");

}

close_order_manager_f::close_order_manager_f( wxWindow* parent, const wxString& title )
:
base_close_order_manager_f( parent )
{
	CentreOnParent();
	SetLabel(title);
	sg->GetColumn(0).SetText(locale->get("close_order_manager_f", "sg_id", "Order #"));
	sg->GetColumn(1).SetText(locale->get("close_order_manager_f", "sg_created", "Created"));
	sg->GetColumn(2).SetText(locale->get("close_order_manager_f", "sg_amount", "Amount"));
	sg->GetColumn(3).SetText(locale->get("close_order_manager_f", "sg_payment_type", "Payment type"));
	sg->GetColumn(4).SetText(locale->get("close_order_manager_f", "sg_room", "Room"));
	sg->GetColumn(5).SetText(locale->get("close_order_manager_f", "sg_POS", "POS"));
	sg->GetColumn(6).SetText(locale->get("close_order_manager_f", "sg_Operator", "Operator"));
	
	find_order_btn->SetLabel(locale->get("close_order_manager_f", "find_order_btn", "Type order #"));
	print_receipt_duplicate_btn->SetLabel(locale->get("close_order_manager_f", "print_receipt_duplicate_btn", "Print reciept duplicate"));
	refund_order_btn->SetLabel(locale->get("close_order_manager_f", "refund_order_btn", "Refund order"));
	change_payment_type_btn->SetLabel(locale->get("close_order_manager_f", "change_payment_type_btn", "Change payment type"));
	reopen_order_btn->SetLabel(locale->get("close_order_manager_f", "reopen_order_btn", "Reopen order"));
	close_btn->SetLabel(locale->get("close_order_manager_f", "close_btn", "Cancel"));
	buttom_bar_sizer->Layout();
	wrap_label(find_order_btn);
	wrap_label(reopen_order_btn);
	wrap_label(change_payment_type_btn);
	wrap_label(refund_order_btn);
	wrap_label(print_receipt_duplicate_btn);
	wrap_label(close_btn);
	sg->AddRoot(_("root"));
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select \
	orders.order_id \
	, orders.created \
	, orders.subtotal \
	, orders.payment_type_id \
	, payment_type.name as payment_type \
	, table_group.name as room \
	, terminal.name as term \
	, orders.status_id \
	, orders.creator_id \
	, orders.closed \
	, orders.closer_id \
	, orders.prepayment_type_id \
	, prepayment_type.name as prepayment_type \
	, orders.paid \
	, min(nvl(order_cctrans.trans_id, 0)) \
	from ksi.orders \
	inner join ksi.seance on orders.seance_id = seance.seance_id and seance.commited is null \
	inner join ksi.shift on shift.shift_id = seance.shift_id and shift.stop_time is null \
	inner join ksi.terminal on terminal.terminal_id = seance.terminal_id \
	left join ksi.payment_type on payment_type.payment_type_id = orders.payment_type_id \
	left join ksi.payment_type prepayment_type on prepayment_type.payment_type_id = orders.prepayment_type_id \
	left join ksi.table_group on table_group.table_group_id = orders.table_group_id \
	left join ksi.order_cctrans on order_cctrans.order_id = orders.order_id and order_cctrans.cafe_id = ksi.util.get_cur_cafe_id \
	where orders.status_id = 3 and orders.type_id in (1,2,3,4,5,6) \
	and shift.shift_id = ksi.util.get_cur_shift_id \
	group by orders.order_id, orders.created, orders.subtotal, orders.payment_type_id, \
	payment_type.name, table_group.name, terminal.name, orders.status_id, orders.creator_id, \
	orders.closed, orders.closer_id, orders.prepayment_type_id, prepayment_type.name, orders.paid \
	order by orders.paid desc"
	);
	query->execute_statement();
	
	// Connect Events		
	sg->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( close_order_manager_f::sgSelChanged ), NULL, this );
	sg->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( close_order_manager_f::sgSelChanging ), NULL, this );	

	bool selected = false;
	while (!query->next()) {
		int order_id = static_cast<int>(boost::get<double>(query->get(0)));
		boost::posix_time::ptime created = boost::get<boost::posix_time::ptime>(query->get(1));
		double subtotal = boost::get<double>(query->get(2));
		int payment_type_id = static_cast<int>(boost::get<double>(query->get(3)));
		std::string payment_type = boost::get<std::string>(query->get(4));
		std::string room = boost::get<std::string>(query->get(5));
		std::string term = boost::get<std::string>(query->get(6));
		int status_id = static_cast<int>(boost::get<double>(query->get(7)));
		int creator_id = static_cast<int>(boost::get<double>(query->get(8)));
		boost::posix_time::ptime closed = boost::get<boost::posix_time::ptime>(query->get(9));
		int closer_id = static_cast<int>(boost::get<double>(query->get(10)));
		int prepayment_type_id = static_cast<int>(boost::get<double>(query->get(11)));
		std::string prepayment_type = boost::get<std::string>(query->get(12));
		int cctrans_count = static_cast<int>(boost::get<double>(query->get(14)));
		close_order_t* co_ptr = new close_order_t(node_type::other, order_id, created, subtotal, payment_type_id, payment_type, room, term, closer_id, prepayment_type_id, prepayment_type);
		wxTreeItemId curr = sg->AppendItem(sg->GetRootItem(), _(""), -1, -1, co_ptr);
		payment_type_map[payment_type_id] = payment_type;
		payment_type_map[prepayment_type_id] = prepayment_type;
		order_cctrans_count_map[order_id] = cctrans_count;
		if (!selected) {
			sg->SelectItem(curr);
			selected = true;
		}
	}
}

close_order_manager_f::~close_order_manager_f() {
	sg->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( close_order_manager_f::sgSelChanged ), NULL, this );
	sg->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( close_order_manager_f::sgSelChanging ), NULL, this );
}

void close_order_manager_f::close_btnClick( wxCommandEvent& event ) {
	EndModal(wxID_CLOSE);
}

void close_order_manager_f::sgSelChanged( wxTreeEvent& event )
{	
	wxTreeItemId id = event.GetItem();
	if (!event.GetItem().IsOk()) return;
	if (event.GetItem() == sg->GetRootItem()) return;
	
	close_order_t* node = dynamic_cast<close_order_t*>(sg->GetItemData(event.GetItem()));
	if (!node) return;
	print_receipt_duplicate_btn->Enable(true);
	refund_order_btn->Enable(true);
	change_payment_type_btn->Enable(order_cctrans_count_map[node->col1()] == 0);
	if (app_id != 5) reopen_order_btn->Enable(order_cctrans_count_map[node->col1()] == 0);
}

void close_order_manager_f::sgSelChanging( wxTreeEvent& event )
{
	print_receipt_duplicate_btn->Enable(false);
	refund_order_btn->Enable(false);
	change_payment_type_btn->Enable(false);
	reopen_order_btn->Enable(false);
}

void close_order_manager_f::find_order_btnClick( wxCommandEvent& event ) {
	wx_window_ptr<input_num_f> form (new input_num_f(this, locale->get("close_order_manager_f" , "find_order_btn", "Type order #"), ksi_cafe::identical_format_t()));
	if (form->ShowModal() != wxID_OK) return;
	int order_id = boost::lexical_cast<int>(to_mb(form->format_text()));
	try {
		if (!locate(order_id)) wxMessageBox(locale->get("global", "order_not_found_str", "Order not found"));
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.comp_message()));
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
}

void close_order_manager_f::change_payment_type_btnClick( wxCommandEvent& event ) {
	if(!wx_mng_auth(this)) return;
	try {
		wxTreeItemId curr = sg->GetSelection();
		if (!curr.IsOk()) THROW_LOCATION(0, "void close_order_manager_f::change_payment_type_btnClick(wxCommandEvent&)");
		wxTreeItemData* data = sg->GetItemData(curr);
		close_order_t* node = dynamic_cast<close_order_t*>(data);
		if (!node) THROW_LOCATION(1, "void close_order_manager_f::change_payment_type_btnClick(wxCommandEvent&)");
		
		int payment_type_id = 0;
		{
			std::vector<wxString> cap_vec;
			cap_vec.push_back(_("Cash"));
			cap_vec.push_back(_("Credit Card"));
			std::vector<wxColour> color_vec;
			color_vec.push_back(btn_face_color);
			color_vec.push_back(btn_face_color);
			std::vector<bool> enabled_vec;
			enabled_vec.push_back(true);
			enabled_vec.push_back(true);
			std::vector<bool> checked_vec;
			checked_vec.push_back(false);
			checked_vec.push_back(false);
			std::vector<bool> wrap_vec;
			wrap_vec.push_back(false);
			wrap_vec.push_back(false);
			wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Change payment type"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
			int result = form->ShowModal() - wxID_HIGHEST;
			if (result == 0) { // Cash
				payment_type_id = 1;
			}
			else if (result == 1) { // Credit Card
				payment_type_id = 2;
			}
		}
		if (!payment_type_id) return;
		wx_disable_t disable_window(this);
		wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));	
		*flog << "change payment_type, order # "  + boost::lexical_cast<std::string>(node->col1()) + ", new_payment_type_id=" + boost::lexical_cast<std::string>(payment_type_id);
		
		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
		query->create_statement(
		"UPDATE KSI.ORDERS SET PAYMENT_TYPE_ID=:payment_type_id "
		" WHERE CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID AND ORDER_ID=:order_id"
		);
		std::auto_ptr<ksi_client::oracle_query_i> lock_query = ksi_cafe::session->create_query();
		lock_query->create_statement(
		"SELECT ORDER_ID, STATUS_ID "
		" FROM KSI.ORDERS "
		" WHERE CAFE_ID=KSI.UTIL.GET_CUR_CAFE_ID "
		" AND ORDER_ID = :order_id "
		" FOR UPDATE NOWAIT"
		);
		
		try {
			query->set("payment_type_id", payment_type_id);
			query->set("order_id", node->col1());
			lock_query->set("order_id", node->col1());
			
			ksi_cafe::ora_transaction_t ora_trans(ksi_cafe::session.get());
			lock_query->execute_statement();
			ksi_cafe::logical_transaction_t logical_trans(ksi_cafe::session.get(), 0);
			query->execute_statement();
			node->col9(payment_type_id);
			std::string payment_type_str;
			if (payment_type_map.find(payment_type_id) == payment_type_map.end()) {
				std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
				query->create_statement(
				"SELECT NAME FROM KSI.PAYMENT_TYPE WHERE PAYMENT_TYPE_ID=:payment_type_id"
				);
				query->set("payment_type_id", payment_type_id);
				query->execute_statement();
				if (query->next()) THROW_LOCATION(10346, "void close_order_manager_f::change_payment_type_btnClick( wxCommandEvent& event )");
				payment_type_str = boost::get<std::string>(query->get(0));
			}
			else payment_type_str = payment_type_map[payment_type_id];
			node->col5(payment_type_str);
			
			*flog << "successful";
			logical_trans.logical_end();
			*flog << "transaction commit";
			if (!ora_trans.commit(true)) *flog << "force apply";
			*flog << "successful";
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			wxMessageBox(_("This order is being opened by another terminal. Please try to open it later."));
		}
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.comp_message()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
	sg->Refresh(false);
	if (config->get("functionality", "close_order_operation_f_auto_close") == "0" || to_uc(config->get("functionality", "close_order_operation_f_auto_close")).MakeUpper() == _("OFF")) {
	}
	else {
		EndModal(wxID_CLOSE);
	}
}

void close_order_manager_f::reopen_order_btnClick( wxCommandEvent& event ) {
	if(!wx_mng_auth(this)) return;
	wx_disable_t disable_window(this);
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	try {
		wxTreeItemId curr = sg->GetSelection();
		if (!curr.IsOk()) THROW_LOCATION(0, "void close_order_manager_f::reopen_order_btnClick( wxCommandEvent& event )");
		wxTreeItemData* data = sg->GetItemData(curr);
		close_order_t* node = dynamic_cast<close_order_t*>(data);
		if (!node) THROW_LOCATION(1, "void close_order_manager_f::reopen_order_btnClick( wxCommandEvent& event )");
		
		*flog << "reopen order # " + boost::lexical_cast<std::string>(node->col1());
		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
		query->create_statement(
		"UPDATE KSI.ORDERS SET STATUS_ID=:status_id, PAYMENT_TYPE_ID=NULL, PAID=NULL, CLOSED=NULL, CLOSER_ID=NULL "
		" WHERE CAFE_ID=KSI.UTIL.GET_CUR_CAFE_ID AND ORDER_ID=:order_id"
		);
		std::auto_ptr<ksi_client::oracle_query_i> lock_query = ksi_cafe::session->create_query();
		lock_query->create_statement(
		"SELECT ORDER_ID, STATUS_ID "
		" FROM KSI.ORDERS "
		" WHERE CAFE_ID=KSI.UTIL.GET_CUR_CAFE_ID "
		" AND ORDER_ID = :order_id "
		" FOR UPDATE NOWAIT"
		);
		
		bool successful = false;
		try {
			query->set("order_id", node->col1());
			query->set("status_id", 1);
			lock_query->set("order_id", node->col1());
			
			ksi_cafe::ora_transaction_t ora_trans(ksi_cafe::session.get());
			lock_query->execute_statement();
			ksi_cafe::logical_transaction_t logical_trans(ksi_cafe::session.get(), 0);
			query->execute_statement();
			
			*flog << "successful";
			logical_trans.logical_end();
			*flog << "transaction commit";
			if (!ora_trans.commit(true)) *flog << "force apply";
			*flog << "successful";
			successful = true;
		}
		catch (std::exception& ex) {
			*flog << ex.what();
			wxMessageBox(_("This order is being opened by another terminal. Please try to open it later."));
		}
		
		if (successful) {
			*flog << "delete grid_item";
			sg->Delete(curr);
		}
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.comp_message()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
	if (config->get("functionality", "close_order_operation_f_auto_close") == "0" || to_uc(config->get("functionality", "close_order_operation_f_auto_close")).MakeUpper() == _("OFF")) {
	}
	else {
		EndModal(wxID_CLOSE);
	}
}

bool close_order_manager_f::locate(int order_id) {
	wxTreeItemIdValue cookie;
	wxTreeItemId curr = sg->GetFirstChild(sg->GetRootItem(), cookie);
	while (curr.IsOk()) {
		wxTreeItemData* data = sg->GetItemData(curr);
		close_order_t* node = dynamic_cast<close_order_t*>(data);
		if (!node) THROW_LOCATION(0, "bool close_order_mananger_f::locate(int)");
		if (order_id == node->col1()) {
			sg->SelectItem(curr);
			return true;
		}
		curr = sg->GetNextChild(sg->GetRootItem(), cookie);
	}	
	return false;
}

void close_order_manager_f::print_receipt_duplicate_btnClick( wxCommandEvent& event ) {
	bool auth_ = true;
	if (config->get("security", "print_receipt_duplicate") == "0" || to_uc(config->get("security", "print_receipt_duplicate")).MakeUpper() == _("OFF"))
		auth_  = false;
	try {
		wxTreeItemId curr = sg->GetSelection();
		if (!curr.IsOk()) THROW_LOCATION(0, "void close_order_manager_f::print_receipt_duplicate_btnClick(wxCommandEvent&)");
		wxTreeItemData* data = sg->GetItemData(curr);
		close_order_t* node = dynamic_cast<close_order_t*>(data);
		if (!node) THROW_LOCATION(1, "void close_order_manager_f::print_receipt_duplicate_btnClick(wxCommandEvent&)");
		if (auth_ && !wx_mng_auth(this)) return;
		std::string xml = print_receipt_duplicate(*cafe_container->current_cafe(), 2, node->col1(), shared_locale->get_std("receipt", "receipt_duplicate_str", "DUPLICATE"), false, !stand_alone);
		print_xml(printer.get(), xml);
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()));
	}
	if (config->get("functionality", "close_order_operation_f_auto_close") == "0" || to_uc(config->get("functionality", "close_order_operation_f_auto_close")).MakeUpper() == _("OFF")) {
	}
	else {
		EndModal(wxID_CLOSE);
	}
}

void close_order_manager_f::refund_order_btnClick( wxCommandEvent& event ) {
	bool auth_ = true;
	if (config->get("security", "refund_order") == "0" || to_uc(config->get("security", "refund_order")).MakeUpper() == _("OFF"))
		auth_  = false;
	try {
		wxTreeItemId curr = sg->GetSelection();
		if (!curr.IsOk()) THROW_LOCATION(0, "void close_order_manager_f::refund_order_btnClick(wxCommandEvent&)");
		wxTreeItemData* data = sg->GetItemData(curr);
		close_order_t* node = dynamic_cast<close_order_t*>(data);
		if (!node) THROW_LOCATION(1, "void close_order_manager_f::refund_order_btnClick(wxCommandEvent&)");
		int order_id = node->col1();
		int payment_type_id = node->col4();
		int prepayment_type_id = node->col9();
		if (auth_ && !wx_mng_auth(this)) return;
		wx_window_ptr<text_f> form (new text_f(this, locale->get("close_order_manager_f", "caption", "Refund reason"), _(""), 0));
		if (form->ShowModal() != wxID_OK) return;
		std::string refund_note = to_mb(form->text());
		
		*flog << "Refund order_id = " + boost::lexical_cast<std::string>(order_id);
		
		if ((payment_type_id == 2 || prepayment_type_id == 2) && !stand_alone) {
			if (!client.get()) THROW_MESSAGE(8, "client.get() == 0", "void close_order_manager_f::refund_order_btnClick(wxCommandEvent&)");
			cctrans_vec_t base_vec = fetch_cctrans_for_order(order_id);
			int current_batch_id = client->get_current_batch_id();
			//if (current_batch_id == 0) THROW_LOCATION(2, "void close_order_manager_f::refund_order_btnClick(wxCommandEvent&)"); 
			//ksi_cafe::seq_t seq(ksi_cafe::session.get(), "cctrans_seq");
			if (prepayment_type_id == 2 && payment_type_id == 2) {
				*flog << "prepayment & payment";
				{
					*flog << "prepayment processing";
					cctrans_record_t auth_rec = prepayment_auth(base_vec, payment_type_id, prepayment_type_id);
					cctrans_record_t add_to_batch_rec = prepayment_add_to_batch(base_vec, payment_type_id, prepayment_type_id);
					int batch_id = add_to_batch_rec.batch_id();
					if (batch_id == 0) THROW_LOCATION(3, "void close_order_manager_f::refund_order_btnClick(wxCommandEvent&)");
					if (batch_id == current_batch_id) { // credit void
						credit_void_fn(order_id, batch_id, add_to_batch_rec, auth_rec, refund_note);
					}
					else { // credit refund
						credit_refund_fn(this, order_id, current_batch_id, add_to_batch_rec, refund_note);
					}
				}
				{
					*flog << "payment processing";
					cctrans_record_t auth_rec = payment_auth(base_vec, payment_type_id, prepayment_type_id);
					cctrans_record_t add_to_batch_rec = payment_add_to_batch(base_vec, payment_type_id, prepayment_type_id);
					int batch_id = add_to_batch_rec.batch_id();
					if (batch_id == 0) THROW_LOCATION(4, "void close_order_manager_f::refund_order_btnClick(wxCommandEvent&)");
					if (batch_id == current_batch_id) { // credit void
						credit_void_fn(order_id, batch_id, add_to_batch_rec, auth_rec, refund_note);
					}
					else { // credit refund
						credit_refund_fn(this, order_id, current_batch_id, add_to_batch_rec, refund_note);
					}
				}
			}
			else if (prepayment_type_id == 2) {
				*flog << "prepayment";
				cctrans_record_t auth_rec = prepayment_auth(base_vec, payment_type_id, prepayment_type_id);
				cctrans_record_t add_to_batch_rec = prepayment_add_to_batch(base_vec, payment_type_id, prepayment_type_id);
				int batch_id = add_to_batch_rec.batch_id();
				//if (batch_id == 0) THROW_LOCATION(5, "void close_order_manager_f::refund_order_btnClick(wxCommandEvent&)");
				if (batch_id == current_batch_id) { // credit void
					credit_void_fn(order_id, batch_id, add_to_batch_rec, auth_rec, refund_note);
				}
				else { // credit refund
					credit_refund_fn(this, order_id, current_batch_id, add_to_batch_rec, refund_note);
				}
			}
			else if (payment_type_id == 2) {
				*flog << "payment";
				cctrans_record_t auth_rec = payment_auth(base_vec, payment_type_id, prepayment_type_id);
				cctrans_record_t add_to_batch_rec = payment_add_to_batch(base_vec, payment_type_id, prepayment_type_id);
				int batch_id = add_to_batch_rec.batch_id();
				if (batch_id == 0) THROW_LOCATION(6, "void close_order_manager_f::refund_order_btnClick(wxCommandEvent&)");
				if (batch_id == current_batch_id) { // credit void
					credit_void_fn(order_id, batch_id, add_to_batch_rec, auth_rec, refund_note);
				}
				else { // credit refund
					credit_refund_fn(this, order_id, current_batch_id, add_to_batch_rec, refund_note);
				}
			}
		}
		else {
			*flog << "update ksi.orders for refund";
			std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
			query->create_statement
			(
			"update ksi.orders set refund_note = :refund_note, status_id = (case when type_id = 5 then 5 else  4 end) \
			where order_id = :order_id and cafe_id = ksi.util.get_cur_cafe_id"
			);
			query->set("order_id", order_id);
			query->set("refund_note", refund_note);
		
			ksi_cafe::ora_transaction_t ora_trans(ksi_cafe::session.get());
			ksi_cafe::logical_transaction_t logical_trans(ksi_cafe::session.get(), 167);
			query->execute_statement();
			logical_trans.logical_end();
			
			if (registrar.get()) {
				bool print_successful = false;
				try {
					*flog << "print_refund";
					print_refund(*cafe_container->current_cafe(), order_id, "REFUND", *registrar);
					print_successful = true;
					*flog << "successful";
				}
				catch (fiscal::exception& ex) {
					*flog << ex.what();
					wxMessageBox(to_uc(ex.what()));
				}
				if (!print_successful) {
					wxMessageBox(_("Не удалось напечатать фискальный ресит."));
					return;
				}
			}
			
			if (!ora_trans.commit(true)) *flog << "force apply";
		}
		*flog << "print receipt duplicate";
		try {
			std::string xml = print_receipt_duplicate(*cafe_container->current_cafe(), 1, order_id, shared_locale->get_std("receipt", "refund_str", "REFUND"), by_group, !stand_alone);
			print_xml(printer.get(), xml);

/*		
			if (lang == 1) {
				std::string xml = print_receipt_duplicate(*cafe_container->current_cafe(), 1, order_id, "REFUND", by_group, !stand_alone);
				print_xml(printer.get(), xml);
			}
			else if (lang == 2) {
				std::string xml = print_receipt_duplicate(*cafe_container->current_cafe(), 1, order_id, "�������", by_group, !stand_alone);
				print_xml(printer.get(), xml);
			}
*/
		}
		catch (exception_t& ex) {
			*flog << ex.comp_message();
			wxMessageBox(to_uc(ex.user_message()));
		}
		catch (...) {
			THROW_LOCATION(7, "void close_order_manager_f::refund_order_btnClick(wxCommandEvent&)");
		}
		*flog << "delete grid_item";
		sg->Delete(curr);
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.comp_message()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
	if (config->get("functionality", "close_order_operation_f_auto_close") == "0" || to_uc(config->get("functionality", "close_order_operation_f_auto_close")).MakeUpper() == _("OFF")) {
	}
	else {
		EndModal(wxID_CLOSE);
	}
}
