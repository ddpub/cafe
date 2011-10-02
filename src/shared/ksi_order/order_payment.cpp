
#include "order_payment.hpp"
#include <shared_modules/connect.hpp>
#include <memory>


namespace ksi_cafe {

void base_cancel_fee_t::simple_cancel_fee(int order_id, int payment_type_id, bool logical_trans)
{	
	ora_transaction_t trans(session.get());

	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 0));
	
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(
	"update ksi.orders set status_id = 5, cancel_fee = :fee, \
  cancel_payment_type_id = decode(:type_id, 0, null, :type_id) \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);

	query->set("order_id", order_id);
	query->set("fee", fee());
	query->set("type_id", payment_type_id);

	query->execute_statement();
	log_trans.reset();
	trans.commit();
}

void credit_cancel_fee_t::cancel_fee(int order_id, bool logical_trans)
{
	ora_transaction_t trans(session.get());

	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 0));

	if (cc_return_.get())
		credit_return_processing(*cc_return_, order_id, fee());
	else if (cc_batch_.get())
		credit_batch_processing(*cc_batch_, order_id, fee(), 0);
	base_cancel_fee_t::simple_cancel_fee(order_id, 2, logical_trans);
	log_trans.reset();
	trans.commit();
}

void dummy_payment_t::payment(int order_id, bool logical_trans)
{
  ora_transaction_t trans(session.get());
	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 152));
	
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(
	"update ksi.orders set payment_type_id = 4 \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);
	query->set("order_id", order_id);

	query->execute_statement();
	log_trans.reset();
	trans.commit();
}

void dummy_payment_t::cancel(int order_id, bool logical_trans)
{
	ora_transaction_t trans(session.get());
	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 0));

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(
	"update ksi.orders set payment_type_id = null, \
  tendered_amount = null \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);

	query->set("order_id", order_id);

	query->execute_statement();
	log_trans.reset();
	trans.commit();
}

void cash_payment_t::payment(int order_id, bool logical_trans)
{  
	ora_transaction_t trans(session.get());
  std::auto_ptr<logical_transaction_t> log_trans;
  if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 152));
  
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
  "update ksi.orders set payment_type_id = 1, \
  tendered_amount = :tendered_amount \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);

	query->set("order_id", order_id);
	query->set("tendered_amount", amount()+change_);

	query->execute_statement();	
  log_trans.reset();
  trans.commit();
}

void cash_payment_t::cancel(int order_id, bool logical_trans)
{
  ora_transaction_t trans(session.get());
  std::auto_ptr<logical_transaction_t> log_trans;
  if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 0));
  
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"update ksi.orders set payment_type_id = null, \
  tendered_amount = null \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);
  query->set("order_id", order_id);
  log_trans.reset();
  trans.commit();
}

void check_payment_t::payment(int order_id, bool logical_trans)
{
  ora_transaction_t trans(session.get());
  std::auto_ptr<logical_transaction_t> log_trans;
  if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 152));
  
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"update ksi.orders set payment_type_id = 3 \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);
	query->set("order_id", order_id);
	query->execute_statement();
  
	query->create_statement
	(
	"insert into ksi.order_check \
  (trans_id, cafe_id, order_id, check_number, amount, manager_id) \
  values \
  (ksi.order_check_seq.nextval, ksi.util.get_cur_cafe_id, \
  :order_id, :check_number, :amount, :manager_id)"
	);
  
	query->set("order_id", order_id);
	query->set("check_number", check_num_);
	query->set("amount", amount());
	query->set("manager_id", mng_);

	query->execute_statement();

	log_trans.reset();
  trans.commit();
}

void check_payment_t::cancel(int order_id, bool logical_trans)
{
  ora_transaction_t trans(session.get());
  std::auto_ptr<logical_transaction_t> log_trans;
  if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 0));

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(
  "delete \
   from ksi.order_check \
   where cafe_id = ksi.util.get_cur_cafe_id \
     and order_id = :order_id \
     and trans_id = \
      ( \
        select max(trans_id) \
        from ksi.order_check \
        where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id \
      )"
	);

	query->set("order_id", order_id);

	query->execute_statement();
  
	query->create_statement
	(
	"update ksi.orders set payment_type_id = null \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);
	query->set("order_id", order_id);

	query->execute_statement();
  
	log_trans.reset();
  trans.commit();
}

void credit_payment_t::auth_payment(int order_id, bool logical_trans)
{
	ora_transaction_t trans(session.get());
	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 152));
	credit_auth_processing(*cc_auth_, order_id, amount());
  
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"update ksi.orders set payment_type_id = 2 \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);

	query->set("order_id", order_id);

	query->execute_statement();

	log_trans.reset();
	trans.commit();    
}

void credit_vpayment_t::payment(int order_id, bool logical_trans) {
	ora_transaction_t trans(session.get());
	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 152));
	
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(
	"update ksi.orders set  payment_type_id = 2 \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = " + boost::lexical_cast<std::string>(order_id)
	);
	query->execute_statement();
	log_trans.reset();
	trans.commit();
}

void credit_vpayment_t::cancel(int order_id, bool logical_trans) {
	ora_transaction_t trans(session.get());
	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 152));
	
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(
	"update ksi.orders set payment_type_id = null, \
	tendered_amount = null \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = " + boost::lexical_cast<std::string>(order_id)
	);
	query->execute_statement();
	log_trans.reset();
	trans.commit();
}

void credit_payment_t::payment(int order_id, bool logical_trans) {
	ora_transaction_t trans(session.get());
	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 152));
	credit_batch_processing(*cc_batch_, order_id, amount(), tip_);
	log_trans.reset();
	trans.commit();
}

void credit_payment_t::cancel(int order_id, bool logical_trans)
{
  ora_transaction_t trans(session.get());
  std::auto_ptr<logical_transaction_t> log_trans;
  if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 0));

	if (cc_batch_.get()) { // Был add_to_batch
		credit_void_processing(credit_void_t<credit_batch_t>(*cc_batch_, void_txn_id_), order_id, amount() + tip_);
	}
	else { // Не было add_to_batch
		credit_void_processing(credit_void_t<credit_auth_t>(*cc_auth_, void_txn_id_), order_id, amount());
	}
  
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(  
	"update ksi.orders set payment_type_id = null \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);
	query->set("order_id", order_id);

	query->execute_statement();
	log_trans.reset();
	trans.commit();
}

void cash_prepayment_t::payment(int order_id, bool logical_trans)
{
  ora_transaction_t trans(session.get());
  std::auto_ptr<logical_transaction_t> log_trans;
  if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 166));
  
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();  
	query->create_statement
	(	
  "update ksi.orders set prepayment_type_id = 1, \
  prepayment = :prepayment, prepaid = sysdate, prepayment_seance_id = ksi.util.get_cur_seance_id \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);
  
	query->set("order_id", order_id);
	query->set("prepayment", amount());
	
	query->execute_statement();  
  log_trans.reset();
  trans.commit();
}

void cash_prepayment_t::cancel(int order_id, bool logical_trans)
{
  ora_transaction_t trans(session.get());
  std::auto_ptr<logical_transaction_t> log_trans;
  if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 0));
  
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(  
  "update ksi.orders set prepayment_type_id = null, \
  prepayment = null, prepaid = null, prepayment_seance_id = null \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);

	query->set("order_id", order_id);
  
	query->execute_statement();
  log_trans.reset();
  trans.commit();
}

void check_prepayment_t::payment(int order_id, bool logical_trans)
{
  ora_transaction_t trans(session.get());
  std::auto_ptr<logical_transaction_t> log_trans;
  if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 166));
  
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(  
  "insert into ksi.order_check \
  (trans_id, cafe_id, order_id, check_number, amount, manager_id) \
  values \
  (ksi.order_check_seq.nextval, ksi.util.get_cur_cafe_id, \
  :order_id, :check_number, :amount, :manager_id)"
	);

	query->set("order_id", order_id);
	query->set("check_number", check_num_);
	query->set("amount", amount());
	query->set("manager_id", mng_);

	query->execute_statement();

	query->create_statement
	(
  "update ksi.orders set prepayment_type_id = 3, \
  prepayment = :prepayment, prepaid = sysdate, prepayment_seance_id = ksi.util.get_cur_seance_id \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);

	query->set("order_id", order_id);
	query->set("prepayment", amount());

	query->execute_statement();
    
  log_trans.reset();
  trans.commit();
}

void check_prepayment_t::cancel(int order_id, bool logical_trans)
{
  ora_transaction_t trans(session.get());
  std::auto_ptr<logical_transaction_t> log_trans;
  if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 0));
  
  
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(  
  "delete \
   from ksi.order_check \
   where cafe_id = ksi.util.get_cur_cafe_id \
     and order_id = :order_id \
     and trans_id = \
      ( \
        select min(trans_id) \
        from ksi.order_check \
        where cafe_id = ksi.util.get_cur_cafe_id \
          and order_id = :order_id \
      )"
	);

	query->set("order_id", order_id);

	query->execute_statement();
  
	query->create_statement
	(
  "update ksi.orders set prepayment_type_id = null, \
  prepayment = null, prepaid = null, prepayment_seance_id = null \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id)"
	);
  
	query->execute_statement();

  log_trans.reset();
  trans.commit(); 
}

void credit_prepayment_t::auth_payment(int order_id, bool logical_trans)
{
	ora_transaction_t trans(session.get());
	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 166));
	credit_auth_processing(*cc_auth_, order_id, amount());
  
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(
	"update ksi.orders set prepayment_type_id = 2, \
	prepayment = :prepayment, prepaid = sysdate, prepayment_seance_id = ksi.util.get_cur_seance_id \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);
  
	query->set("order_id", order_id);
	query->set("prepayment", amount());
	
	query->execute_statement();
  
	log_trans.reset();
	trans.commit();
}

void credit_prepayment_t::payment(int order_id, bool logical_trans)
{
  ora_transaction_t trans(session.get());
  std::auto_ptr<logical_transaction_t> log_trans;
  if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 166));
  credit_batch_processing(*cc_batch_, order_id, amount(), tip_);
  log_trans.reset();
  trans.commit();
}

void credit_prepayment_t::cancel(int order_id, bool logical_trans) {
	ora_transaction_t trans(session.get());
	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 0));

	if (cc_batch_.get()) { // Был add_to_batch
		credit_void_processing(credit_void_t<credit_batch_t>(*cc_batch_, void_txn_id_), order_id, amount() + tip_);
	}
	else { // Не было add_to_batch
		credit_void_processing(credit_void_t<credit_auth_t>(*cc_auth_, void_txn_id_), order_id, amount());
	}

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(
	"update ksi.orders set prepayment_type_id = null, \
	prepayment = null, prepaid = null, prepayment_seance_id = null \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id"
	);
	query->set("order_id", order_id);

	query->execute_statement();

	log_trans.reset();
	trans.commit();
}


};




