#ifndef ORDER_PAYMENT_H_DERGACHEV_15_01_2008
#define ORDER_PAYMENT_H_DERGACHEV_15_01_2008

#include "credit_card.hpp"
#include <string>
#include <memory>


namespace ksi_cafe {

class base_cancel_fee_t
{
public:
	explicit base_cancel_fee_t(double fee_): fee_(fee_) {}
	virtual ~base_cancel_fee_t() {}
	virtual void cancel_fee(int order_id, bool logical_trans)=0;
	double fee() const { return fee_; }
protected:
	void simple_cancel_fee(int order_id, int payment_type_id, bool logical_trans);
private:
	base_cancel_fee_t();
	base_cancel_fee_t& operator=(const base_cancel_fee_t&);
	base_cancel_fee_t(const base_cancel_fee_t&);
	double fee_;
};

class dummy_cancel_fee_t: public base_cancel_fee_t
{
public:
	dummy_cancel_fee_t(): base_cancel_fee_t(0) {}
	virtual ~dummy_cancel_fee_t() {}
	virtual void cancel_fee(int order_id, bool logical_trans) { base_cancel_fee_t::simple_cancel_fee(order_id, 4, logical_trans); }
private:
};

class cash_cancel_fee_t: public base_cancel_fee_t
{
public:
	explicit cash_cancel_fee_t(double fee_): base_cancel_fee_t(fee_) {}
	virtual ~cash_cancel_fee_t() {}
	virtual void cancel_fee(int order_id, bool logical_trans) { base_cancel_fee_t::simple_cancel_fee(order_id, 1, logical_trans); }
private:
};

class credit_cancel_fee_t: public base_cancel_fee_t
{
public:
	credit_cancel_fee_t(const credit_return_t& cc_return_, double fee_): base_cancel_fee_t(fee_), cc_return_(new credit_return_t(cc_return_)) {}
	credit_cancel_fee_t(const credit_batch_t& cc_batch, double fee_): base_cancel_fee_t(fee_), cc_batch_(new credit_batch_t(cc_batch)) {}
	credit_cancel_fee_t(const credit_auth_t& cc_auth, int txn_id, int batch_id, double fee_): base_cancel_fee_t(fee_), cc_batch_(new credit_batch_t(cc_auth, txn_id, batch_id)) {}
	virtual ~credit_cancel_fee_t() {}
	virtual void cancel_fee(int order_id, bool logical_trans);
private:
	std::auto_ptr<credit_return_t> cc_return_;
	std::auto_ptr<credit_batch_t> cc_batch_;
};

class base_payment_t
{
public:
	explicit base_payment_t(double amount_): amount_(amount_) {}
	virtual ~base_payment_t() {}
	virtual void payment(int order_id, bool logical_trans)=0;
	virtual void cancel(int order_id, bool logical_trans)=0;
public:
	double amount() const { return amount_; }
	void change_amount(double new_amount) { amount_ = new_amount; }
private:
	base_payment_t();
	base_payment_t& operator=(const base_payment_t&);
	base_payment_t(const base_payment_t&);
	double amount_;
};

class dummy_payment_t: public base_payment_t
{
public:
	explicit dummy_payment_t(): base_payment_t(0) {}
	virtual ~dummy_payment_t() {}
	virtual void payment(int order_id, bool logical_trans);
	virtual void cancel(int order_id, bool logical_trans);
private:
};

class cash_payment_t : public base_payment_t
{
public:
	cash_payment_t(double cash_, double change_): base_payment_t(cash_-change_), change_(change_) {}
	virtual ~cash_payment_t() {}
	virtual void payment(int order_id, bool logical_trans);
	virtual void cancel(int order_id, bool logical_trans);
public:
	double change() const { return change_; }
	double cash() const { return amount() + change_; }
private:
	double change_;
};

class check_payment_t : public base_payment_t
{
public:
	check_payment_t(int mng_, int check_num_, double amount_): base_payment_t(amount_), mng_(mng_), check_num_(check_num_) {}
	virtual ~check_payment_t() {}
	virtual void payment(int order_id, bool logical_trans);
	virtual void cancel(int order_id, bool logical_trans);
public:
	int mng_id() const { return mng_; }
	int check_num() const { return check_num_; }
private:
	int mng_;
	int check_num_;
};

class credit_vpayment_t: public base_payment_t {
public:
	explicit credit_vpayment_t(double amount_): base_payment_t(amount_) {}
	virtual ~credit_vpayment_t() {}
	virtual void payment(int order_id, bool logical_trans);
	virtual void cancel(int order_id, bool logical_trans);
public:
	double amount() const { return amount(); }
private:
};

class credit_payment_t : public base_payment_t
{
public:
  	credit_payment_t(const credit_auth_t& cc_auth, double amount_): base_payment_t(amount_), cc_auth_(new credit_auth_t(cc_auth)) {}
  	void credit_auth(const credit_auth_t& cc_auth, double new_amount) { cc_auth_.reset(new credit_auth_t(cc_auth)); change_amount(new_amount); cc_batch_.reset(); }
  	void credit_add_to_batch(const credit_batch_t& cc_batch, double new_amount, double new_tip) { cc_batch_.reset(new credit_batch_t(cc_batch)); change_amount(new_amount); tip_ = new_tip; }
  	void credit_add_to_batch(int txn_id, int batch_id, double new_amount, double new_tip) { credit_add_to_batch(credit_batch_t(*cc_auth_, txn_id, batch_id), new_amount, new_tip); }
  	void credit_void(int txn_id_) { void_txn_id_ = txn_id_; }
  	const card_info_t& card_info() const { return cc_auth_->card_info(); }
  	virtual ~credit_payment_t() {}
  	void auth_payment(int order_id, bool logical_trans);
  	const credit_auth_t& cur_auth() const { return *cc_auth_; }
  	virtual void payment(int order_id, bool logical_trans);
  	virtual void cancel(int order_id, bool logical_trans);
public:
  	double tip() const { return tip_; }
  	int auth_txn_id() const { return cc_auth_->txn(); }
	const std::string& num() const { return cc_auth_->num(); }
	const std::string& cc_type() const { return cc_auth_->type(); }
	bool swiped() const { return cc_auth_->swiped(); }
private:
  	std::auto_ptr<credit_auth_t> cc_auth_;
  	std::auto_ptr<credit_batch_t> cc_batch_;
  	double tip_;
  	int void_txn_id_;
};

class cash_prepayment_t: public base_payment_t
{
public:
	cash_prepayment_t(double cash_): base_payment_t(cash_) {}
	virtual ~cash_prepayment_t() {}
	virtual void payment(int order_id, bool logical_trans);
	virtual void cancel(int order_id, bool logical_trans);
private:
};

class check_prepayment_t: public base_payment_t
{
public:
	check_prepayment_t(int mng_, int check_num_, double amount_): base_payment_t(amount_), mng_(mng_), check_num_(check_num_) {}
	virtual ~check_prepayment_t() {}
	virtual void payment(int order_id, bool logical_trans);
	virtual void cancel(int order_id, bool logical_trans);
private:
	int mng_;
	int check_num_;
};

class credit_prepayment_t: public base_payment_t
{
public:
	credit_prepayment_t(const credit_auth_t& cc_auth, double amount_): base_payment_t(amount_), cc_auth_(new credit_auth_t(cc_auth)) {}
	void credit_auth(const credit_auth_t& cc_auth, double new_amount) { cc_auth_.reset(new credit_auth_t(cc_auth)); change_amount(new_amount); cc_batch_.reset(); }
	void credit_add_to_batch(const credit_batch_t& cc_batch, double new_amount, double new_tip) { cc_batch_.reset(new credit_batch_t(cc_batch)); change_amount(new_amount); tip_ = new_tip; }
	void credit_add_to_batch(int txn_id, int batch_id, double new_amount, double new_tip) { credit_add_to_batch(credit_batch_t(*cc_auth_, txn_id, batch_id), new_amount, new_tip); }
	void credit_void(int txn_id_) { void_txn_id_ = txn_id_; }
	const card_info_t& card_info() const { return cc_auth_->card_info(); }
	virtual ~credit_prepayment_t() {}
	void auth_payment(int order_id, bool logical_trans);
	virtual void payment(int order_id, bool logical_trans);
	virtual void cancel(int order_id, bool logical_trans);
public:
	double tip() const { return tip_; }
	int auth_txn_id() const { return cc_auth_->txn(); }
	const std::string& num() const { return cc_auth_->num(); }
private:
	std::auto_ptr<credit_auth_t> cc_auth_;
	std::auto_ptr<credit_batch_t> cc_batch_;
	double tip_;
	int void_txn_id_;
};

};

#endif
