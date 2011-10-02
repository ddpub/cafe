

#include "credit_card.hpp"
#include <shared_modules/connect.hpp>
#include <memory>


namespace ksi_cafe {

void credit_return_processing(const credit_return_t& cc_return, int order_id, double amount)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
  "insert into ksi.order_cctrans \
  ( \
     cafe_id \
   , order_id \
   , trans_id \
   , type_id \
   , batch_id \
   , amount \
   , transaction_id \
   , tran_date \
   , cardholder_name \
   , last_4_digits \
   , card_type \
   , person_id \
   , swiped \
  ) \
  values \
  ( \
     ksi.util.get_cur_cafe_id \
   , :order_id \
   , ksi.cctrans_seq.nextval \
   , 3 \
   , :batch_id \
   , :amount \
   , :transaction_id \
   , sysdate \
   , :name \
   , :num \
   , :card_type \
   , ksi.util.get_cur_user_id \
   , :swiped \
  )"
	);
	query->set("order_id", order_id);
	query->set("amount", amount);
	query->set("name", cc_return.holder());
	query->set("card_type", cc_return.type());
	query->set("num", cc_return.num());
	query->set("transaction_id", cc_return.txn());
	query->set("swiped", int(cc_return.swiped()));
	query->set("batch_id", cc_return.batch());

	query->execute_statement();
}

void credit_auth_processing(const credit_auth_t& cc_auth, int order_id, double amount)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"insert into ksi.order_cctrans \
	( \
     cafe_id \
	, order_id \
	, trans_id \
	, type_id \
	, amount \
	, transaction_id \
	, tran_date \
	, cardholder_name \
	, last_4_digits \
	, card_type \
	, person_id \
	, swiped \
	, auth_code \
	) \
	values \
	( \
     ksi.util.get_cur_cafe_id \
	, :order_id \
	, ksi.cctrans_seq.nextval \
	, 5 \
	, :amount \
	, :transaction_id \
	, sysdate \
	, :name \
	, :num \
	, :card_type \
	, ksi.util.get_cur_user_id \
	, :swiped \
	, :auth_code \
	)"
	);

	query->set("order_id", order_id);
	query->set("amount", amount);
	query->set("name", cc_auth.holder());
	query->set("card_type", cc_auth.type());
	query->set("num", cc_auth.num());
	query->set("transaction_id", cc_auth.txn());
	query->set("auth_code", cc_auth.auth_code());
	query->set("swiped", int(cc_auth.swiped()));

	query->execute_statement();
}

void credit_batch_processing(const credit_batch_t& cc_batch, int order_id, double amount, double tip)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
  "insert into ksi.order_cctrans \
  ( \
     cafe_id \
   , order_id \
   , trans_id \
   , type_id \
   , amount \
   , tips \
   , transaction_id \
   , tran_date \
   , cardholder_name \
   , last_4_digits \
   , card_type, batch_id \
   , person_id \
   , swiped \
   , ref_transaction_id) \
  values \
  ( \
     ksi.util.get_cur_cafe_id \
   , :order_id \
   , ksi.cctrans_seq.nextval \
   , 1 \
   , :amount \
   , :tips \
   , :transaction_id \
   , sysdate \
   , :name \
   , :num \
   , :card_type \
   , :batch_id \
   , ksi.util.get_cur_user_id \
   , :swiped \
   , decode(:ref_transaction_id, 0, null, :ref_transaction_id) \
	)"
	);

	query->set("order_id", order_id);
	query->set("amount", amount+tip);
	query->set("tips", tip);
	query->set("name", cc_batch.holder());
	query->set("card_type", cc_batch.type());
	query->set("num", cc_batch.num());
	query->set("transaction_id", cc_batch.txn());
	query->set("ref_transaction_id", cc_batch.ref_txn());
	query->set("batch_id", cc_batch.batch());
	query->set("swiped", cc_batch.swiped());

	query->execute_statement();
}

template <typename T> void credit_void_processing_(const credit_void_t<T>& cc_void, int order_id, double amount)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
  "insert into ksi.order_cctrans \
  ( \
     cafe_id \
   , order_id \
   , trans_id \
   , type_id \
   , transaction_id \
   , amount \
   , tran_date \
   , last_4_digits \
   , card_type \
   , batch_id \
   , person_id \
   , swiped \
   , ref_transaction_id) \
  values \
  ( \
     ksi.util.get_cur_cafe_id \
   , :order_id \
   , ksi.cctrans_seq.nextval \
   , 2 \
   , :txn_id \
   , :amount \
   , sysdate \
   , :last_4_digits \
   , :card_type \
   , decode(:batch_id, 0, null, :batch_id) \
   , ksi.util.get_cur_user_id \
   , :swiped \
   , :ref_transaction_id \
  )"
	);

	query->set("txn_id", cc_void.txn());
	query->set("amount", amount);
	query->set("card_type", cc_void.type());
	query->set("batch_id", cc_void.batch());
	query->set("last_4_digits", cc_void.num());
	query->set("swiped", int (cc_void.swiped()));
	query->set("ref_transaction_id", cc_void.ref_txn());
	query->set("order_id", order_id);

	query->execute_statement();
}


void credit_void_processing(const credit_void_t<credit_auth_t>& cc_void, int order_id, double amount)
{
  credit_void_processing_<credit_auth_t>(cc_void, order_id, amount);
}

void credit_void_processing(const credit_void_t<credit_batch_t>& cc_void, int order_id, double amount)
{
  credit_void_processing_<credit_batch_t>(cc_void, order_id, amount);
}

};
