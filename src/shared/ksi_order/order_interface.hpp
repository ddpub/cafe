#ifndef ORDER_INTERFACE_H_DERGACHEV_29_11_2007
#define ORDER_INTERFACE_H_DERGACHEV_29_11_2007

#include "menu_interface.hpp"
#include <memory>
#include <vector>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "cafe_container.hpp"
#include "order_payment.hpp"
#include "credit_card.hpp"
#include <shared_modules/connect.hpp>
#include <ksi_util/util.hpp>
#include <string>
#include "delivery_info.hpp"


namespace ksi_cafe {

extern std::string pos_version;

class order_dish_modifier_t: private menu_dish_modifier_t
{
public:
	order_dish_modifier_t(const menu_dish_modifier_t& mm, int order_dish_id_, double price_, int count_, int count_wasted_, int count_registered_, int count_ready_);
	order_dish_modifier_t(const menu_dish_modifier_t& mm, double price_, int count_);
	order_dish_modifier_t(const menu_dish_modifier_t& mm, int count_);
public:
	const std::string type_check_sum() const;
	const std::string check_sum() const;
	const std::string algo_type_check_sum(int) const;
	const std::string algo_check_sum(int) const;
public:
	void set_count_ready(int cr) { count_ready_ = cr; }
	void set_price(double pr) { price_ = pr; }
	void set_coupon_id(int id, int mng_id) { coupon_id_ = id; coupon_mng_id_ = mng_id; }
	void clear_coupon() { coupon_id_ = 0; coupon_mng_id_ = 0; }
	void reset_price() { price_ = def_price(); }
	void renew() { order_dish_id_ = 0; }
public:
	int count() const { return count_; }
	int count_wasted() const { return count_wasted_; }
	int count_registered() const { return count_registered_; }
	int count_ready() const { return count_ready_; }
	int order_dish_id() const { return order_dish_id_; }
	int kitchen_type_id() const { return menu_dish_modifier_t::kitchen_type_id(); }
	int modifier_id() const { return menu_dish_modifier_t::modifier_id(); }
	int mod_dish_id() const { return modifier_t::dish_id(); }
	int coupon_id() const { return coupon_id_; }
	int coupon_mng_id() const { return coupon_mng_id_; }
	double price() const { return round(price_); }
	double def_price() const { return round(menu_dish_modifier_t::price()); }
	const std::string& name() const { return menu_dish_modifier_t::name(); }
	double discount(int c_id) const;
private:
	int order_dish_id_;
	double price_;
	int count_;
	int count_wasted_;
	int count_registered_;
	int count_ready_;
	int coupon_id_;
	int coupon_mng_id_;
};

class order_dish_t : private menu_dish_t
{
public:
	order_dish_t(const menu_dish_t& md, int count_, int count_wasted_, int count_registered_, int count_ready_, double price_, const std::vector<order_dish_modifier_t>& mods_, int order_dish_id_, const boost::posix_time::ptime& time_);

	order_dish_t(const menu_dish_t& md, int count_, double price_,
	const std::vector<order_dish_modifier_t>& mods_, int order_dish_id, const boost::posix_time::ptime& time_);

	order_dish_t(const menu_dish_t& md, const std::vector<order_dish_modifier_t>& mods_, int order_dish_id_, const boost::posix_time::ptime& time_);

	order_dish_t(const menu_dish_t& md, const std::vector<order_dish_modifier_t>& mods_);

	order_dish_t(const menu_dish_t& md);

	order_dish_t(const order_dish_t& od, int count_);
public:
	const std::string& name() const { return menu_dish_t::name(); }
	int dish_id() const { return menu_dish_t::dish_id(); }
	const boost::posix_time::ptime& time() const { return time_; }
	const std::string& description() const { return description_; }
	void set_description(const std::string& desc) { description_ = desc; }
	int can_add_employee_meal() const { return menu_dish_t::employee_meal(); }
	int order_dish_id() const { return order_dish_id_; }
	int kitchen_type_id() const { return menu_dish_t::kitchen_type_id(); }
	double def_price() const { return round(menu_dish_t::price()); }
	double price() const { return round(price_); }
	void set_price(double val) { price_ = round(val); }
	void reset_price() { price_ = def_price(); }
	int employee_id() const { return employee_id_; }
	void set_employee_id(int id) { employee_id_ = id; }

	int new_count() const { return new_count_; }
	void new_count_inc() { new_count_++; }
	void new_count_dec() { new_count_--; }
	int count() const { return count_; }
	void count_inc() { count_++; }
	void count_dec() { count_--; }
	void clear_count() { count_ = 0; new_count_ = 0; count_wasted_ = 0; count_registered_ = 0; }
	void clear_new_count() { new_count_ = 0; }
	int count_wasted() const { return count_wasted_; }
	void set_count_wasted(int cnt) { count_wasted_ = cnt; }
	void wasted_inc() { ++count_wasted_; }
	int count_registered() const { return count_registered_; }
	int count_ready() const { return count_ready_; }
	void set_count_ready(int cnt) { count_ready_ = cnt; }
	int total_count() const { return count_ + new_count_; }

	int coupon_id() const { return coupon_id_; }
	int coupon_mng_id() const { return coupon_mng_id_; }
	void set_coupon_id(int id, int mng_id) { coupon_id_ = id; coupon_mng_id_ = 0; }
public:
	const std::string type_check_sum() const;
	const std::string check_sum() const;
	const std::string algo_type_check_sum(int) const;
	const std::string algo_check_sum(int) const;
	double amount() const;
	double def_amount() const;
	double discount(int c_id) const;
	
	void clear_employee();
	void clear_coupon();	
	void renew();
public:
	std::vector<order_dish_modifier_t>& modifier_vec() { return mods_; }
	const std::vector<order_dish_modifier_t>& modifier_vec() const { return mods_; }
private:
	int new_count_;
	int count_;
	int count_wasted_;
	int count_registered_;
	int count_ready_;
	double price_;
	std::vector<order_dish_modifier_t> mods_;
	int order_dish_id_;
	boost::posix_time::ptime time_;
	int employee_id_;
	int coupon_id_;
	int coupon_mng_id_;
	std::string description_;
};

order_dish_t make_misc_charges(double amount, const std::string& description);
order_dish_t make_fixed_discount(double def_amount, double amount, int coupon_id, const std::string& coupon_name, int coupon_mng_id);


namespace order_type {
	enum order_type_t {
		UNKNOWN=0,
		FOR_HERE=1,
		TO_GO=2,
		DELIVERY=3,
		PICKUP=4,
		EMPLOYEE_MEAL=5,
		TABLE_BOOKING=6
	};
}

class order_t
{
public:
	order_t(const cafe_t& cafe_, int order_id_);
	order_t(const cafe_t& cafe_, int order_id_, bool for_update, bool nowait);
	order_t(const cafe_t& cafe_, const token_t& token_, int order_id_, bool for_update, bool nowait);
	order_t(const cafe_t& cafe_, const table_t& table_, int order_id_, bool for_update, bool nowait);
	order_t(const cafe_t& cafe_, const table_t& table_, const token_t& token_, int order_id_, bool for_update, bool nowait);
public:
	int order_id() const { return order_id_; }
	int status_id() const { return status_id_; }
	int type_id() const { return type_id_; }
	const boost::posix_time::ptime& order_created() const { return order_created_; }
	const boost::posix_time::ptime& order_closed() const { return order_closed_; }
	const std::string& memo() const { return memo_; }
	const std::string& refund_note() const { return refund_note_; }
	const cafe_t& cafe() const { return cafe_; }
	double sales_tax() const { return cafe_.sales_tax(); }
	double gratuity_tax() const { return cafe_.gratuity(); }
	double delivery_min_subtotal() const { return cafe_.delivery_min_subtotal(); }
	double meal_free_limit() const { return cafe_.meal_free_limit(); }
	const table_t& table() const { return table_; }
	const token_t& token() const { return token_; }
	int guest_count() const { return guest_count_; }
	void set_guest_count(int new_guest_count) { guest_count_ = new_guest_count; }
	int creator_id() const { return creator_id_; }
	void change_creator_id(int ext_creator_id) { creator_id_ = ext_creator_id; }
public:
	void change_token(const token_t& new_token) { token_ = new_token; }
	void change_table(const table_t& new_table) { table_ = new_table; }
	void set_memo(const std::string& str) { memo_ = str; }
	void set_refund_note(const std::string& str) { refund_note_ = str; }
	void set_hide_mode() { flags_ = flags_ | 0x8; }
	void unset_hide_mode() { flags_ = flags_ & 0x7; }
	void set_hide(bool hide) { if (hide) set_hide_mode(); else unset_hide_mode(); }
	bool is_hide() const { if (flags_ & 0x8) return true; return false; }
public:
	const base_payment_t* cur_payment() const { return payment_.get(); }
	const delivery_info_t* get_delivery_info() const { return delivery_info_.get(); }
	base_payment_t* cur_payment() { return payment_.get(); }
	const base_payment_t* cur_prepayment() const { return prepayment_.get(); }
	base_payment_t* cur_prepayment() { return prepayment_.get(); }
public:
	bool prepayment_empty() const { if (prepayment_type_id_ == 0) return true; return false; }
	bool prepayment_cash() const { if (prepayment_type_id_ == 1) return true; return false; }
	bool prepayment_credit_card() const { if (prepayment_type_id_ == 2) return true; return false; }
	bool prepayment_check() const { if (prepayment_type_id_ == 3) return true; return false; }
	bool prepayment_dummy() const { if (prepayment_type_id_ == 4) return true; return false; }
	bool prepayment_void() const { if (prepayment_type_id_ == 0) return true; return false; }
	int prepayment_type_id() const { return prepayment_type_id_; }
	bool payment_empty() const { if (payment_type_id_ == 0) return true; return false; }
	bool payment_cash() const { if (payment_type_id_ == 1) return true; return false; }
	bool payment_credit_card() const { if (payment_type_id_ == 2) return true; return false; }
	bool payment_check() const { if (payment_type_id_ == 3) return true; return false; }
	bool payment_dummy() const { if (payment_type_id_ == 4) return true; return false; }
	bool payment_void() const { if (payment_type_id_ == 0) return true; return false; }
	int payment_type_id() const { return payment_type_id_; }
public:
	bool status_empty() const { if (status_id_ == 0) return true; return false; }
	bool status_accepted() const  { if (status_id_ == 1 || status_id_ == 6) return true; return false; }
	bool status_ready() const { if (status_id_ == 2) return true; return false; }
	bool status_closed() const { if (status_id_ == 3) return true; return false; }
	bool status_void() const { if (status_id_ == 4) return true; return false; }
	bool status_send_to_kitchen() const { if (status_id_ == 6) return true; return false; }
public:
	bool type_empty() const { if (type_id_ == 0) return true; return false; }
	bool type_pickup() const { if (type_id_ == 4) return true; return false; }
	bool type_for_here() const { if (type_id_ == 1) return true; return false; }
	bool type_to_go() const { if (type_id_ == 2) return true; return false; }
	bool type_delivery() const { if (type_id_ == 3) return true; return false; }
	bool type_employee_meal() const { if (type_id_ == 5) return true; return false; }
	bool type_table_booking() const { if (type_id_ == 6) return true; return false; }
	int employee_id() const { return employee_id_; }
public:
	void set_payment_empty(int txn_id=0);
	void set_payment_cash(cash_payment_t *ptr);
	void set_payment_credit_card(credit_payment_t *ptr);
	void set_vpayment_credit_card(credit_vpayment_t *ptr);
	void set_payment_check(check_payment_t *ptr);
	void set_payment_dummy(dummy_payment_t *ptr);

	void set_prepayment_empty(int txn_id=0);
	void set_prepayment_cash(cash_prepayment_t *ptr);
	void set_prepayment_credit_card(credit_prepayment_t *ptr);
	void set_prepayment_check(check_prepayment_t *ptr);
public:
	void set_status_empty() { status_id_ = 0; }
	void set_status_accepted() { status_id_ = 1; }
	void set_status_ready() { status_id_ = 2; }
	void set_status_closed() { status_id_ = 3; }
	void set_status_void() { status_id_ = 4; }
public:
	void set_type_for_here(delivery_info_t* info);
	void set_type_to_go();
	void set_type_delivery(delivery_info_t* info);
	void set_type_pickup(delivery_info_t* info);
	void set_type_employee_meal(int new_employee_id, const std::string& fio="", bool erase_dish=true);
	void set_type_table_booking(delivery_info_t* info);
public:
	int dish_count() const { return int(order_dish_.size()); }
	int real_dish_count() const;
	int order_dish_count(const std::string& check_sum) const;
public:
	order_dish_t& dish(int i) { return order_dish_[i]; }
	const order_dish_t& dish(int i) const { return order_dish_[i]; }
	std::vector<order_dish_t*> all_dish();
public:
	void sync_order_created();

	void clear_wasted();
	void save_wasted();
	void load_wasted();

	void save_misc_charges();
	void load_misc_charges();

	void save_adjustable_discount();
	void load_adjustable_discount();
	bool adjustable_discount_exist() const;

	void set_accept(int cafe_id2, bool logical_trans);
	void set_close(bool logical_trans);
	void set_simply_close(bool logical_trans);
	void set_void(bool logical_trans);

	bool commit(bool force=true) { if (order_trans_.get()) return order_trans_->commit(force); return true; }
public:
	const order_dish_t& add_dish(const order_dish_t& odish, bool sync_time);
	const order_dish_t& add_misc_charges(double amount, const std::string& discription);
	const order_dish_t& add_fixed_discount(double discount_amount, int coupon_id, const std::string& coupon_name, int coupon_mng_id);

	void remove_dish(const order_dish_t& dish, bool wasted=false);
	void remove_new_dish(const order_dish_t& dish);
public:
	void clear_employee();
	void clear_coupon();
public:
	double prepayment_amount() const;
	double payment_amount() const;
	double discount() const;
	double subtotal() const;
	double def_subtotal() const;
	double tax() const;
	double def_tax() const;
	double def_gratuity() const;
	double gratuity() const;
	double total() const;
	double def_total() const;
	double delivery_fee() const;
public:
	void compress();
private:
	void uncompress();
	void custom_load(bool for_update, bool nowait);
	void sync_order(bool trans_log);
public:
	void splitByOrder(int order_id);
private:
	cafe_t cafe_;
	int order_id_;
	int prepayment_type_id_; // 0-new, 1-cash, 2-credit card, 3-check
	int payment_type_id_; // 0-new, 1-cash, 2-credit card, 3-check
	int status_id_; // 0-new, 1-Accepted, 2-Ready, 3-Closed/Delivered, 4-void
	int type_id_; // 0-new, 1-for here, 2-to go, 3-delivery, 4-pickup, 5-employee meal
	int delivery_id_;
	int employee_id_;
	ksi_cafe::table_t table_;
	ksi_cafe::token_t token_;
	int guest_count_;
	int creator_id_;
	int flags_;
	boost::posix_time::ptime order_created_;
	boost::posix_time::ptime order_closed_;
	std::string memo_;
	std::string refund_note_;
private:
	std::auto_ptr<ora_transaction_t> order_trans_;
	std::auto_ptr<delivery_info_t> delivery_info_;
	std::auto_ptr<base_payment_t> prepayment_;
	std::auto_ptr<base_payment_t> payment_;
private:
	std::vector<order_dish_t> order_dish_;
	std::vector<order_dish_t> misc_charges_;
	std::vector<order_dish_t> adjustable_discount_;
	std::map<std::string, int> count_wasted_; // check_sum, count_wasted
private:
	order_t();
	order_t(const order_t&);
	const order_t& operator=(const order_t&);
};

};

#endif
