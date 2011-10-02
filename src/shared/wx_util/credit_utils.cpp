
#include <wx/window.h>
#include "wx_util.hpp"
#include <lang/xml_lang.h>
#include "credit_utils.hpp"
#include <shared_modules/connect.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <boost/lexical_cast.hpp>
#include <ksi_include/algo_functor.hpp>
#include <boost/bind.hpp>
#include <ksi_util/log.hpp>
#include <ksi_libpos/pos.hpp>
#include <shared_modules/fin_load_f.h>
#include <shared_modules/btn_list_f.h>
#include <shared_modules/card_swipe_f.h>
#include <shared_modules/manual_entry_f.h>
#include <ksi_order/credit_card.hpp>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;
extern std::auto_ptr<ksi_cafe::xml_lang_t> shared_locale;
extern std::auto_ptr<file_log_t> flog;
extern std::auto_ptr<ksi_client::user_pos_i> client;


cctrans_vec_t fetch_cctrans_for_order(int order_id) {
	cctrans_vec_t vec;
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select \
	type_id, \
	amount, \
	tips, \
	batch_id, \
	transaction_id, \
	ref_transaction_id, \
	last_4_digits, \
	card_type \
	from ksi.order_cctrans \
	where order_id = :order_id \
	order by trans_id"
	);
	query->set("order_id", order_id);
	query->execute_statement();
	
	while (!query->next()) {
		int type_id = static_cast<int>(boost::get<double>(query->get(0)));
		double amount = boost::get<double>(query->get(1));
		double tips = boost::get<double>(query->get(2));
		int batch_id = static_cast<int>(boost::get<double>(query->get(3)));
		int txn_id = static_cast<int>(boost::get<double>(query->get(4)));
		int ref_txn_id = static_cast<int>(boost::get<double>(query->get(5)));
		std::string cc_num = boost::get<std::string>(query->get(6));
		std::string cc_type = boost::get<std::string>(query->get(7));
		vec.push_back(cctrans_record_t(type_id, amount, tips, batch_id, ref_txn_id, txn_id, cc_num, cc_type));
	}
	return vec;
}

cctrans_vec_t valid_cctrans(const cctrans_vec_t& vec) {
	cctrans_vec_t tmp;
	for (size_t i=0; i<vec.size(); i++) {
		if (vec[i].type_id() == 5) {
			int pos1 = 0;
			int pos2 = 0;
			for (size_t j=i+1; j<vec.size(); j++) {
				if (vec[j].ref_txn_id() == vec[i].txn_id()) {
					if (vec[j].type_id() == 2) pos2 = j;
					else if (vec[j].type_id() == 1) pos1 = j;
					if (!pos2) {
						tmp.push_back(vec[i]);
						if (pos1) tmp.push_back(vec[pos1]);
					}
				}
			}
		}
	}
	return tmp;
}

cctrans_record_t payment_auth(const cctrans_vec_t& vec, int payment_type, int prepayment_type) {
	cctrans_vec_t tmp = valid_cctrans(vec);
	int cnt = std::count_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
	if (prepayment_type == 2 && payment_type == 2) {
		if (cnt != 2) THROW_LOCATION(0, "cctrans_record_t payment_auth(const cctrnas_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(1, "cctrans_record_t payment_auth(const cctrnas_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 = std::find_if(i+1, tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i2 == tmp.end()) THROW_LOCATION(2, "cctrans_record_t payment_auth(const cctrnas_vec_t&, int, int)");
		return *i;
	}
	if (prepayment_type != 2 && payment_type == 2) {
		if (cnt != 1) THROW_LOCATION(3, "cctrans_record_t payment_auth(const cctrnas_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(4, "cctrans_record_t payment_auth(const cctrnas_vec_t&, int, int)");
		return *i;
	}
	THROW_LOCATION(5, "cctrans_record_t payment_auth(const cctrnas_vec_t&, int, int)");
}

cctrans_record_t payment_add_to_batch(const cctrans_vec_t& vec, int payment_type, int prepayment_type) {
	cctrans_vec_t tmp = valid_cctrans(vec);
	int cnt = std::count_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
	if (prepayment_type == 2 && payment_type == 2)
	{
		if (cnt != 2) THROW_LOCATION(0, "cctrans_vec_t payment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(1, "cctrans_vec_t payment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 = std::find_if(i+1, tmp.end(),  boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i2 == tmp.end()) THROW_LOCATION(2, "cctrans_vec_t payment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i3 = std::find_if(i2+1, tmp.end(), boost::bind(std::logical_and<bool>(), boost::bind(std::equal_to<int>(), 1, boost::bind(&cctrans_record_t::type_id, _1)), boost::bind(std::equal_to<int>(), i2->txn_id(), boost::bind(&cctrans_record_t::ref_txn_id, _1))));
		if (i3 == tmp.end()) THROW_LOCATION(3, "cctrans_vec_t payment_add_to_batch(const cctrans_vec_t&, int, int)");
		return *i3;
	}
	if (prepayment_type != 2 && payment_type == 2)
	{
		if (cnt != 1) THROW_LOCATION(4, "cctrans_vec_t payment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(5, "cctrans_vec_t payment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 =std::find_if(i+1, tmp.end(), boost::bind(std::logical_and<bool>(), boost::bind(std::equal_to<int>(), 1, boost::bind(&cctrans_record_t::type_id, _1)), boost::bind(std::equal_to<int>(), i->txn_id(), boost::bind(&cctrans_record_t::ref_txn_id, _1))));
		if (i2 == tmp.end()) THROW_LOCATION(6, "cctrans_vec_t payment_add_to_batch(const cctrans_vec_t&, int, int)");                                             
		return *i2;
	}
	THROW_LOCATION(7, "cctrans_vec_t payment_add_to_batch(const cctrans_vec_t&, int, int)");
}

cctrans_record_t prepayment_auth(const cctrans_vec_t& vec, int payment_type, int prepayment_type) {
	cctrans_vec_t tmp = valid_cctrans(vec);

	int cnt = std::count_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
	if (prepayment_type == 2 && payment_type == 2)
	{
		if (cnt != 2) THROW_LOCATION(0, "cctrans_record_t prepayment_auth(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(1, "cctrans_record_t prepayment_auth(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 = std::find_if(i+1, tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i2 == tmp.end()) THROW_LOCATION(2, "cctrans_record_t prepayment_auth(const cctrans_vec_t&, int, int)");
		return *i;
	}
	if (prepayment_type == 2 && payment_type != 2)
	{
		if (cnt != 1) THROW_LOCATION(3, "cctrans_record_t prepayment_auth(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(4, "cctrans_record_t prepayment_auth(const cctrans_vec_t&, int, int)");
		return *i;
	}
	THROW_LOCATION(5, "cctrans_record_t prepayment_auth(const cctrans_vec_t&, int, int)");
}

cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t& vec, int payment_type, int prepayment_type) {
	cctrans_vec_t tmp = valid_cctrans(vec);

	int cnt = std::count_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
	if (prepayment_type == 2 && payment_type == 2)
	{
		if (cnt != 2) THROW_LOCATION(0, "cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(),boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(1, "cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 = std::find_if(i+1, tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i2 == tmp.end()) THROW_LOCATION(2, "cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
	    cctrans_vec_t::iterator i3 = std::find_if(i+1, tmp.end(), boost::bind(std::logical_and<bool>(), boost::bind(std::equal_to<int>(), 1, boost::bind(&cctrans_record_t::type_id, _1)), boost::bind(std::equal_to<int>(), i->txn_id(), boost::bind(&cctrans_record_t::ref_txn_id, _1))));
		if (i3 == tmp.end()) THROW_LOCATION(3, "cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		return *i3;
	}
	if (prepayment_type == 2 && payment_type != 2)
	{
		if (cnt != 1) THROW_LOCATION(4, "cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(5, "cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 =std::find_if(i+1, tmp.end(), boost::bind(std::logical_and<bool>(), boost::bind(std::equal_to<int>(), 1, boost::bind(&cctrans_record_t::type_id, _1)), boost::bind(std::equal_to<int>(), i->txn_id(), boost::bind(&cctrans_record_t::ref_txn_id, _1))));
		if (i2 == tmp.end()) THROW_LOCATION(6, "cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t&, int, int)");                                          
		return *i2;
	}
	THROW_LOCATION(7, "cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
}

void credit_void_fn(int order_id, int batch_id, const cctrans_record_t& add_to_batch_rec, const cctrans_record_t& auth_rec, const std::string& refund_note) {
	*flog << "credit void";
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"insert into ksi.order_cctrans (cafe_id, order_id, trans_id, type_id, amount, tips, transaction_id, tran_date, last_4_digits, card_type, batch_id, person_id, ref_transaction_id) \
	values(ksi.util.get_cur_cafe_id, :order_id, ksi.cctrans_seq.nextval, 2, 0, 0, :transaction_id, sysdate, :num, :card_type, :batch_id, ksi.util.get_cur_user_id, :ref_transaction_id)"
	);
	query->set("order_id", order_id);
	query->set("card_type", add_to_batch_rec.cc_type());
	query->set("num", add_to_batch_rec.cc_num());
	query->set("ref_transaction_id", auth_rec.txn_id());
	query->set("batch_id", batch_id);
	std::auto_ptr<ksi_client::oracle_query_i> query2 = ksi_cafe::session->create_query();
	query2->create_statement
	(
	"update ksi.orders set refund_note = :refund_note, status_id = (case when type_id = 5 then 5 else  4 end) \
	where order_id = :order_id and cafe_id = ksi.util.get_cur_cafe_id"
	);
	query2->set("order_id", order_id);
	query2->set("refund_note", refund_note);
	int void_transaction_id = 0;
	try {
		ksi_cafe::ora_transaction_t ora_trans(ksi_cafe::session.get());
		ksi_cafe::logical_transaction_t logical_trans(ksi_cafe::session.get(), 167);
		*flog << "credit void, txn_id" + boost::lexical_cast<std::string>(auth_rec.txn_id());
		/**
		void_transaction_id = client->credit_void(auth_rec.txn_id());
		**/
		
		ksi_client::pos_exception* except = 0;
		wx_window_ptr<fin_load_f> fin_form (new fin_load_f(0, _(""), _("Communicating with processing server, please wait.")));
		boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_void, client.get(), auth_rec.txn_id());
		fin_form->start(boost::bind(cc_processing, &void_transaction_id, &except, f));
		fin_form->ShowModal();
		if (except != 0) throw *except;
		
		*flog << "successful";
		query->set("transaction_id", void_transaction_id);
		*flog << "update ksi.order_cctrans";
		query->execute_statement();
		*flog << "sucessful";
		*flog << "update ksi.orders";
		query2->execute_statement();
		*flog << "successful";
		logical_trans.logical_end();
		*flog << "transaction commit";
		if (!ora_trans.commit(true)) *flog << "force apply";				
		*flog << "successful";
	}
	catch (ksi_client::pos_exception& ex) {
		*flog << ex.what();
		throw;
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		if (void_transaction_id != 0) {
			//***/
		}
		throw;
	}
	catch (...) {
		THROW_LOCATION(0, "void credit_void_fn(int, int, const cctrans_record_t&, const cctrans_record_t&, const std::string&)");
	}
}

void credit_refund_fn(wxWindow* win, int order_id, int& current_batch_id, const cctrans_record_t &add_to_batch_rec, const std::string& refund_note) {
	*flog << "credit refund";
	std::vector<wxString> cap_vec; std::vector<wxColour> color_vec; std::vector<bool> enabled_vec; std::vector<bool> checked_vec; std::vector<bool> wrap_vec;
	cap_vec.push_back(_("AUTHORIZE CC")); cap_vec.push_back(_("Manual Entry"));
	color_vec.push_back(btn_face_color); color_vec.push_back(btn_face_color);
	enabled_vec.push_back(true); enabled_vec.push_back(true);
	checked_vec.push_back(false); checked_vec.push_back(false); 
	wrap_vec.push_back(true);
	wx_window_ptr<btn_list_f> form (new btn_list_f(win, _("Credit Card payment"), _(""), cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec));
	int result = form->ShowModal() - wxID_HIGHEST;
	
	int txn_id = 0;
	std::string cc_buff_;
	double total = add_to_batch_rec.amount() + add_to_batch_rec.tips();
	
	try {
		if (result == 0) { // Authorize cc
			*flog << "credit return, auth";
			wx_window_ptr<card_swipe_f> form (new card_swipe_f(win, _(""), locale->get("global", "AUTHORIZE_CC_str", "AUTHORIZE CC")));
			if (form->ShowModal() != wxID_OK) return;
			cc_buff_ = form->buff();
			*flog << "credit_return, cc_buff_ = " + cc_buff_;
			client->set_do_report(true);
			/**
			txn_id = client->credit_return(cc_buff_, total);
			*/
			ksi_client::pos_exception* except = 0;
			wx_window_ptr<fin_load_f> fin_form (new fin_load_f(win, _(""), locale->get("global", "communication_with_processing_server_please_wait", "Communicating with processing server, please wait.")));
			boost::function<int ()> f = boost::bind(&ksi_client::user_pos_i::credit_return, client.get(), total, cc_buff_);
			fin_form->start(boost::bind(cc_processing, &txn_id, &except, f));
			fin_form->ShowModal();
			if (except != 0) throw *except;
			*flog << "successful, txn_id = " + boost::lexical_cast<std::string>(txn_id);
		}
		else if (result == 1) { // Manual entry
			*flog << "credit return, manual entry";
			wx_window_ptr<manual_entry_f> form (new manual_entry_f(win, locale->get("global", "Manual_entry_str", "Manual Entry")));
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
			wx_window_ptr<fin_load_f> fin_form (new fin_load_f(win, _(""), locale->get("global", "communication_with_processing_server_please_wait", "Communicating with processing server, please wait.")));
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
		*flog << "credit_return_processing";
		ksi_cafe::credit_return_processing(credit_return, order_id, total);
		*flog << "sucessful";
		*flog << "update ksi.orders";
		query->execute_statement();
		*flog << "sucessful";
		logical_trans.logical_end();
		*flog << "transaction commit";
		if (!ora_trans.commit(true)) *flog << "force apply";
		*flog << "successful";
	}
	catch (ksi_client::pos_exception& ex) {
		*flog << ex.what();
		throw;
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		if (txn_id != 0) {
			/***/
		}
		throw;
	}
	catch (...) {
		THROW_LOCATION(0, "void credit_refund_fn(wxWindow*, int, int, const cctrans_record_t&, const std::string&)");
	}
}
