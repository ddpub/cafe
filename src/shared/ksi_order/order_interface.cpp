

#include "order_interface.hpp"
#include <algorithm>
#include <boost/bind.hpp>
#include <numeric>
#include <ksi_include/algo_functor.hpp>
#include <boost/lexical_cast.hpp>
#include <shared_modules/connect.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <oracle_client/ksi_exception.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>


namespace ksi_cafe {

std::string pos_version = "set ksi_cafe::pos_version";

order_dish_modifier_t::order_dish_modifier_t(const menu_dish_modifier_t& mm, int order_dish_id_, double price_, int count_, int count_wasted_, int count_registered_, int count_ready_)
: menu_dish_modifier_t(mm), order_dish_id_(order_dish_id_), price_(price_), count_(count_), count_wasted_(count_wasted_), count_registered_(count_registered_), count_ready_(count_ready_), coupon_id_(0), coupon_mng_id_(0)
{
}

order_dish_modifier_t::order_dish_modifier_t(const menu_dish_modifier_t& mm, double price_, int count_)
: menu_dish_modifier_t(mm), order_dish_id_(0), price_(price_), count_(count_), count_wasted_(0), count_registered_(0), count_ready_(0), coupon_id_(0), coupon_mng_id_(0)
{
}

order_dish_modifier_t::order_dish_modifier_t(const menu_dish_modifier_t& mm, int count_)
: menu_dish_modifier_t(mm), order_dish_id_(0), price_(mm.price()), count_(count_), count_wasted_(0), count_registered_(0), count_ready_(0), coupon_id_(0), coupon_mng_id_(0)
{
}

const std::string order_dish_modifier_t::type_check_sum() const
{
	std::string check_sum = boost::lexical_cast<std::string>(modifier_id()) + "_" + boost::lexical_cast<std::string>(count());
	return check_sum;
}

const std::string order_dish_modifier_t::algo_type_check_sum(int) const
{
	return type_check_sum();
}

const std::string order_dish_modifier_t::check_sum() const
{
	std::string check_sum = boost::lexical_cast<std::string>(int(price()*100)) + "_" + boost::lexical_cast<std::string>(coupon_id_) + "_" + boost::lexical_cast<std::string>(modifier_id()) + "_" + boost::lexical_cast<std::string>(count());
	return check_sum;
}

const std::string order_dish_modifier_t::algo_check_sum(int) const
{
	return check_sum();
}

double order_dish_modifier_t::discount(int c_id) const {
	if (c_id <= 0) THROW_LOCATION(0, "double order_dish_modifier_t::discount(int) const");
	double res = 0;
	if (coupon_id_ == c_id) res = (price() - def_price())*count();
	return round(res);
}

order_dish_t::order_dish_t(const menu_dish_t& md, int count_, int count_wasted_, int count_registered_, int count_ready_, double price_, const std::vector<order_dish_modifier_t>& mods_, int order_dish_id_, const boost::posix_time::ptime& time_)
: menu_dish_t(md), new_count_(0), count_(count_), count_wasted_(count_wasted_), count_registered_(count_registered_), count_ready_(count_ready_), price_(price_), mods_(mods_), order_dish_id_(order_dish_id_), time_(time_), employee_id_(0), coupon_id_(0), coupon_mng_id_(0)
{
}

order_dish_t::order_dish_t(const menu_dish_t& md, int count_, double price_, const std::vector<order_dish_modifier_t>& mods_, int order_dish_id, const boost::posix_time::ptime& time_)
: menu_dish_t(md), new_count_(0), count_(count_), count_wasted_(0), count_registered_(0), count_ready_(0), price_(price_), mods_(mods_), order_dish_id_(order_dish_id_), time_(time_), employee_id_(0), coupon_id_(0), coupon_mng_id_(0)
{
}

order_dish_t::order_dish_t(const menu_dish_t& md, const std::vector<order_dish_modifier_t>& mods_, int order_dish_id_, const boost::posix_time::ptime& time_)
: menu_dish_t(md), new_count_(0), count_(1), count_wasted_(0), count_registered_(0), count_ready_(0), price_(md.price()), mods_(mods_), order_dish_id_(order_dish_id_), time_(time_), employee_id_(0), coupon_id_(0), coupon_mng_id_(0)
{
}

order_dish_t::order_dish_t(const menu_dish_t& md, const std::vector<order_dish_modifier_t>& mods_)
: menu_dish_t(md), new_count_(1), count_(0), count_wasted_(0), count_registered_(0), count_ready_(0), price_(md.price()),
mods_(mods_), order_dish_id_(0), time_(boost::posix_time::second_clock::local_time()), employee_id_(0), coupon_id_(0), coupon_mng_id_(0)
{
}

order_dish_t::order_dish_t(const menu_dish_t& md)
: menu_dish_t(md), new_count_(0), count_(1), count_wasted_(0), count_registered_(0), count_ready_(0), price_(md.price()),
order_dish_id_(0), time_(boost::posix_time::second_clock::local_time()), employee_id_(0), coupon_id_(0), coupon_mng_id_(0)
{
}

order_dish_t::order_dish_t(const order_dish_t& od, int count_)
: menu_dish_t(od), new_count_(count_), count_(0), count_wasted_(0), count_registered_(0), count_ready_(0), price_(od.def_price()),
mods_(od.modifier_vec()), order_dish_id_(od.order_dish_id()), time_(od.time()), employee_id_(0), coupon_id_(0), coupon_mng_id_(0)
{
}



const std::string order_dish_t::type_check_sum() const
{
	std::string check_sum = boost::lexical_cast<std::string>(dish_id()) + "_" + name();
	std::vector<std::string> tmp;

	std::transform(
		  mods_.begin()
		, mods_.end()
		, std::back_inserter(tmp)
		, boost::mem_fn(&order_dish_modifier_t::type_check_sum)
		);
/*
	std::transform(
		  mods_.begin()
		, mods_.end()
		, std::back_inserter(tmp)
		, std::mem_fun_ref(&order_dish_modifier_t::type_check_sum)
	);
*/
	std::sort(tmp.begin(), tmp.end());

	for (size_t i=0; i<tmp.size(); ++i) check_sum += "_" + tmp[i];

	return check_sum;
}

const std::string order_dish_t::algo_type_check_sum(int) const
{
	return type_check_sum();
}

const std::string order_dish_t::check_sum() const
{
	std::string check_sum = boost::lexical_cast<std::string>(employee_id_) + "_" + boost::lexical_cast<std::string>(coupon_id_) +
		"_" + boost::lexical_cast<std::string>(int(price()*100)) + "_" + boost::lexical_cast<std::string>(dish_id()) + "_" + name();
	std::vector<std::string> tmp;

	std::transform(
		  mods_.begin()
		, mods_.end()
		, std::back_inserter(tmp)
		, boost::mem_fn(&order_dish_modifier_t::check_sum)
		);
/*
	std::transform(
		  mods_.begin()
		, mods_.end()
		, std::back_inserter(tmp)
		, std::mem_fun_ref(&order_dish_modifier_t::check_sum)
	);
*/
	std::sort(tmp.begin(), tmp.end());

	for (size_t i=0; i<tmp.size(); ++i) check_sum += "_" + tmp[i];

	return check_sum;
}

const std::string order_dish_t::algo_check_sum(int) const
{
	return check_sum();
}

double order_dish_t::amount() const
{
	double sum = accumulate_proc
	(
		  mods_.begin()
		, mods_.end()
		, price()
		,
		  boost::bind
		  (
				  std::multiplies<double>()
				, boost::bind(&order_dish_modifier_t::price, _1)
				, boost::bind(&order_dish_modifier_t::count, _1)
		  )
	);

	return round(total_count() * sum);
}

double order_dish_t::discount(int c_id) const {
	if (c_id <= 0) THROW_LOCATION(0, "double order_dish_t::discount(int) const");
	double res = 0;
	if (coupon_id_ == c_id) res += (price()-def_price())*total_count();
	for (size_t i=0; i<mods_.size(); ++i) res += mods_[i].discount(c_id)*total_count();
	return round(res);
}

double order_dish_t::def_amount() const
{
	double sum = accumulate_proc
	(
		  mods_.begin()
		, mods_.end()
		, def_price()
		,
		  boost::bind
		  (
			  std::multiplies<double>()
			, boost::bind(&order_dish_modifier_t::def_price, _1)
			, boost::bind(&order_dish_modifier_t::count, _1)
		  )
	);

	return round(total_count() * sum);
}

void order_dish_t::clear_employee()
{
	employee_id_ = 0;
	std::for_each(mods_.begin(), mods_.end(), boost::bind(&order_dish_modifier_t::reset_price, _1));
	std::for_each(mods_.begin(), mods_.end(), boost::bind(&order_dish_modifier_t::clear_coupon, _1));
	reset_price();
}

void order_dish_t::clear_coupon()
{
	coupon_id_ = 0;
	coupon_mng_id_ = 0;
	reset_price();
	
	std::for_each(mods_.begin(), mods_.end(), boost::bind(&order_dish_modifier_t::clear_coupon, _1));
	std::for_each(mods_.begin(), mods_.end(), boost::bind(&order_dish_modifier_t::reset_price, _1));
}

void order_dish_t::renew()
{
	order_dish_id_ = 0;
	std::for_each(mods_.begin(), mods_.end(), boost::bind(&order_dish_modifier_t::renew, _1));
}

order_dish_t make_misc_charges(double amount, const std::string& description)
{
	dish_t dish = dish_t(0, "misc charges: " + description, "", 0, 0, 1);
	menu_dish_t mdish = menu_dish_t(dish, 0, amount, 0, false);
	order_dish_t odish = order_dish_t(mdish, std::vector<order_dish_modifier_t>(), 0, boost::posix_time::second_clock::local_time());
	odish.set_description(description);
	return odish;
}

order_dish_t make_fixed_discount(double def_amount, double amount, int coupon_id, const std::string& coupon_name, int coupon_mng_id) {
	dish_t dish = dish_t(0, coupon_name, "", 0, 0, 1);
	menu_dish_t mdish = menu_dish_t(dish, 0, -1*def_amount, 0, false);
	order_dish_t odish = order_dish_t(mdish, 1, -1*amount, std::vector<order_dish_modifier_t>(), 0, boost::posix_time::second_clock::local_time());
	odish.set_coupon_id(coupon_id, coupon_mng_id);
	return odish;
}

void order_t::set_payment_empty(int txn_id)
{
	payment_type_id_ = 0;
	if (txn_id) dynamic_cast<credit_payment_t*>(payment_.get())->credit_void(txn_id);
	if (payment_.get()) payment_->cancel(order_id_, true);
	payment_.reset();
}

void order_t::set_payment_cash(cash_payment_t *ptr)
{
	payment_type_id_ = 1;
	payment_.reset(ptr);
}

void order_t::set_payment_credit_card(credit_payment_t *ptr)
{
	payment_type_id_ = 2;
	payment_.reset(ptr);
}

void order_t::set_vpayment_credit_card(credit_vpayment_t *ptr) {
	payment_type_id_ = 2;
	payment_.reset(ptr);
}

void order_t::set_payment_check(check_payment_t *ptr) {
	payment_type_id_ = 3;
	payment_.reset(ptr);
}

void order_t::set_payment_dummy(dummy_payment_t *ptr) {
	payment_type_id_ = 4;
	payment_.reset(ptr);
}

void order_t::set_prepayment_empty(int txn_id) {
	prepayment_type_id_ = 0;
	if (txn_id) dynamic_cast<credit_prepayment_t*>(prepayment_.get())->credit_void(txn_id);
	if (prepayment_.get()) prepayment_->cancel(order_id_, true);
	prepayment_.reset();
}

void order_t::set_prepayment_cash(cash_prepayment_t *ptr) {
	prepayment_type_id_ = 1;
	prepayment_.reset(ptr);
}

void order_t::set_prepayment_credit_card(credit_prepayment_t *ptr) {
	prepayment_type_id_ = 2;
	prepayment_.reset(ptr);
}

void order_t::set_prepayment_check(check_prepayment_t *ptr) {
	prepayment_type_id_ = 3;
	prepayment_.reset(ptr);
}

void order_t::set_type_for_here(delivery_info_t* info) {
	type_id_ = 1;
	delivery_info_.reset(info);
}

void order_t::set_type_to_go() {
	type_id_ = 2;
	delivery_info_.reset();
}

std::vector<order_dish_t*> order_t::all_dish() {
	uncompress();  
	std::vector<order_dish_t*> result;
	std::transform(order_dish_.begin(), order_dish_.end(), std::back_inserter(result), ref_to_ptr<order_dish_t>());
	return result;
}

void order_t::set_type_delivery(delivery_info_t* info)
{
	type_id_ = 3;
	delivery_info_.reset(info);
}

void order_t::set_type_pickup(delivery_info_t* info)
{
	type_id_ = 4;
	delivery_info_.reset(info);
}

void order_t::set_type_employee_meal(int new_employee_id, const std::string& fio, bool erase_dish)
{
	type_id_ = 5;
	memo_ = fio;
	employee_id_ = new_employee_id;
	std::vector<order_dish_t>::iterator end = order_dish_.end();
	for (std::vector<order_dish_t>::iterator i=order_dish_.begin(); i!=end; ++i)
	{
		i->set_price(0);
		i->set_employee_id(employee_id_);
		std::vector<order_dish_modifier_t>& mod_vec = i->modifier_vec();
		std::for_each(mod_vec.begin(), mod_vec.end(), boost::bind(&order_dish_modifier_t::set_price, _1, 0.0));
	}

	std::vector<order_dish_t> tmp;
	if (erase_dish) {
		copy_if
		(
			  order_dish_.begin()
			, order_dish_.end()
			, std::back_inserter(tmp)
			, boost::bind
			 (
				  std::logical_and<bool>()
				, boost::bind(std::not_equal_to<int>(), boost::bind(&order_dish_t::dish_id, _1), 0)
				, boost::bind(&order_dish_t::can_add_employee_meal, _1)
			 )
		);
	}
	else {
		copy_if
		(
			  order_dish_.begin()
			, order_dish_.end()
			, std::back_inserter(tmp)
			, boost::bind(std::not_equal_to<int>(), boost::bind(&order_dish_t::dish_id, _1), 0)
		);
	}
	
	order_dish_.clear();
	end = tmp.end();
	for (std::vector<order_dish_t>::iterator i=tmp.begin(); i!=end; ++i)
	{
		add_dish(*i, false);
	}

	delivery_info_.reset();
}

void order_t::set_type_table_booking(delivery_info_t* info) {
	type_id_ = 6;
	delivery_info_.reset(info);
}

int order_t::real_dish_count() const
{
	size_t cnt = 
	std::count_if 
	(
		order_dish_.begin()
		, order_dish_.end()
		, boost::bind
		(
			std::greater<int>()
			, boost::bind(&order_dish_t::total_count, _1)
			, 0
		)
	);

	return int(cnt + misc_charges_.size());
}

int order_t::order_dish_count(const std::string& check_sum) const
{
	int accumulate_count =
		accumulate_proc_if
		(
				order_dish_.begin()
			, order_dish_.end()
			, 0
			, boost::bind(&order_dish_t::total_count, _1)
			,	boost::bind(std::equal_to<std::string>(), boost::bind(&order_dish_t::algo_check_sum, _1, 0), check_sum)
		);

/*
  std::vector<order_dish_t>::const_iterator end = order_dish_.end();
  for (std::vector<order_dish_t>::const_iterator i=order_dish_.begin(); i != end; ++i)
  {
    if (i->check_sum() == check_sum) accumulate_count += i->total_count();
  }
*/
	return accumulate_count;
}

void order_t::sync_order_created()
{
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(
	"select sysdate from dual"
	);
	query->execute_statement();

	if (!query->next()) 
		order_created_ = boost::get<boost::posix_time::ptime>(query->get(0));
	else 
		throw std::runtime_error("Can't fetch from dual");

	std::cout << boost::lexical_cast<std::string>(order_created_) << std::endl;
}

void order_t::clear_wasted()
{
	std::for_each
	(
		  order_dish_.begin()
		, order_dish_.end()
		, boost::bind(&order_dish_t::set_count_wasted, _1, 0)
	);
/*
  for (size_t i=0; i<order_dish.size(); i++)
  {
    order_dish[i].set_count_wasted(0);
  }
*/
}

void order_t::save_wasted()
{
	count_wasted_.clear();
	std::vector<order_dish_t>::iterator end = order_dish_.end();

	for (std::vector<order_dish_t>::iterator i=order_dish_.begin(); i!=end; ++i) {
		count_wasted_[i->type_check_sum()] += i->count_wasted();
	}
}

void order_t::load_wasted()
{
	std::map<std::string, int> fetch;
	std::vector<order_dish_t>::iterator end = order_dish_.end();

	for (std::vector<order_dish_t>::iterator i=order_dish_.begin(); i!=end; ++i) {
		std::string type_check_sum = i->type_check_sum();
		if (!fetch[type_check_sum]) {
			i->set_count_wasted(count_wasted_[type_check_sum]);
			fetch[type_check_sum] = 1;
		}
	}
}

void order_t::save_misc_charges()
{
	misc_charges_.clear();
	for (std::vector<order_dish_t>::iterator i=order_dish_.begin(); i!=order_dish_.end(); ) {
		if (i->dish_id() == 0 && i->coupon_id() == 0) {
			misc_charges_.push_back(*i);
			order_dish_.erase(i);
		}
		else ++i;
	}
	
/*
  misc_charges.clear();
  for (int i=0; i<dish_count(); )
  {
    if (order_dish[i].dish_id() == 0 && order_dish[i].coupon_id() == 0)
    {
      misc_charges.push_back(order_dish[i]);
      order_dish.erase(order_dish.begin()+i);
    }
    else
      i++;
  }
*/
}

void order_t::load_misc_charges()
{
	std::vector<order_dish_t>::iterator pos =
		std::find_if
		(
				order_dish_.begin()
			, order_dish_.end()
			, boost::bind
			(
					std::equal_to<int>()
				, boost::bind(&order_dish_t::dish_id, _1)
				, 0
			)
		);

	std::copy(misc_charges_.begin(), misc_charges_.end(), std::inserter(order_dish_, pos));
	
/*
  size_t ins = 0;
  for (int i=0; i<dish_count(); i++)
  {
    if (order_dish[i].dish_id() == 0)
    {
      ins = i;
      break;
    }
  }
  for (size_t i=0; i<misc_charges.size(); i++)
  {
    order_dish.insert(order_dish.begin()+ins+i, misc_charges[i]);
  }
*/
}

void order_t::save_adjustable_discount()
{
	adjustable_discount_.clear();

	for (std::vector<order_dish_t>::iterator i=order_dish_.begin(); i!=order_dish_.end(); ) {
		if (i->dish_id() == 0 && i->coupon_id() == -1) {
			adjustable_discount_.push_back(*i);
			order_dish_.erase(i);
		}
		else i++;
	}
}

void order_t::load_adjustable_discount()
{
	std::vector<order_dish_t>::iterator end = adjustable_discount_.end();

	for (std::vector<order_dish_t>::iterator i=adjustable_discount_.begin(); i!=end; ++i) {
		add_fixed_discount(fabs(i->def_price()), i->coupon_id(), i->name(), i->coupon_mng_id());
	}
}

bool order_t::adjustable_discount_exist() const
{
	std::vector<order_dish_t>::const_iterator It;
	It = std::find_if
			(
			  order_dish_.begin()
			, order_dish_.end()
			, boost::bind
			(
				  std::logical_and<bool>()
				, boost::bind(std::equal_to<int>(), 0, boost::bind(&order_dish_t::dish_id, _1))
				, boost::bind(std::equal_to<int>(), -1, boost::bind(&order_dish_t::coupon_id, _1))
			)

/*
            and_pred
            (
            compose(std::bind2nd(std::equal_to<int> (), 0), std::mem_fun_ref(order_dish_t::dish_id)),
            compose(std::bind2nd(std::equal_to<int> (), -1), std::mem_fun_ref(order_dish_t::coupon_id))
            )
*/
            );

	if (It == order_dish_.end()) return false;
	return true;
}


const order_dish_t& order_t::add_dish(const order_dish_t& odish, bool sync_time)
{
	if (sync_time && real_dish_count() == 0) sync_order_created();

	std::string check_sum = odish.check_sum();
	std::vector<order_dish_t>::iterator end = order_dish_.end();

	for (std::vector<order_dish_t>::iterator i=order_dish_.begin(); i!=end; ++i) {
		if (i->check_sum() == check_sum) {
			if (odish.new_count() == 1) i->new_count_inc();
			else if (odish.count() == 1) i->count_inc();
			return *i;
		}
	}
	order_dish_.push_back(odish);
	return *(order_dish_.end()-1);
}

const order_dish_t& order_t::add_misc_charges(double amount, const std::string& description)
{
	return add_dish(make_misc_charges(amount, description), true);
}

const order_dish_t& order_t::add_fixed_discount(double discount_amount, int coupon_id, const std::string& coupon_name, int coupon_mng_id)
{
	double discount = 0;
	if (subtotal() < discount_amount) discount = subtotal();
	else discount = discount_amount;

	return add_dish(make_fixed_discount(discount_amount, discount, coupon_id, coupon_name, coupon_mng_id), true);
}

void order_t::remove_dish(const order_dish_t& dish, bool wasted)
{
	std::string check_sum = dish.check_sum();

	std::vector<order_dish_t>::iterator end = order_dish_.end();

	for (std::vector<order_dish_t>::iterator i=order_dish_.begin(); i!=end; ++i) {
		if (i->check_sum() == check_sum && i->total_count() > 0) {
			if (i->new_count() > 0) 
				i->new_count_dec();
			else {
				i->count_dec();
				if (wasted) i->wasted_inc();
			}
		}
	}
}

void order_t::remove_new_dish(const order_dish_t& dish)
{
	std::string check_sum = dish.check_sum();

	std::vector<order_dish_t>::iterator end = order_dish_.end();

	for (std::vector<order_dish_t>::iterator i=order_dish_.begin(); i!=end; ++i) {
		if (i->check_sum() == check_sum && i->total_count() > 0) {
			i->clear_new_count();
		}
	}
}

void order_t::clear_employee()
{
	employee_id_ = 0;
	std::for_each(order_dish_.begin(), order_dish_.end(), boost::bind(&order_dish_t::clear_employee, _1));
}

void order_t::clear_coupon()
{
	std::vector<order_dish_t> tmp;
	std::vector<order_dish_t>::iterator end = order_dish_.end();

	for (std::vector<order_dish_t>::iterator i=order_dish_.begin(); i!=end; ++i) {
		if (i->dish_id() != 0) {
			int count = i->count();
			int new_count = i->new_count();
			i->clear_coupon();
			if (count+new_count == 0) {
			tmp.push_back(*i);
			}
			while (count + new_count > 0) {
				i->clear_count();
				if (count > 0) {
					i->count_inc();
					count--;
				}
				else if (new_count > 0) {
					i->new_count_inc();
					new_count--;
				}
				tmp.push_back(*i);
			}
		}
	}
	order_dish_.clear();

	end = tmp.end();
	for (std::vector<order_dish_t>::iterator i=tmp.begin(); i!=end; ++i) add_dish(*i, false);
}

double order_t::prepayment_amount() const
{
	if (prepayment_.get()) return round(prepayment_->amount());
	return 0;
}

double order_t::payment_amount() const
{
	if (payment_.get()) return round(payment_->amount());
	return 0;
}

double order_t::discount() const
{
	double accumulate_discount = 0;
	std::vector<order_dish_t>::const_iterator end = order_dish_.end();
	for (std::vector<order_dish_t>::const_iterator i=order_dish_.begin(); i != end; ++i) {
		if (i->coupon_id() != 0 && i->dish_id() == 0) {
			accumulate_discount -= i->price();
		}
		else {
			accumulate_discount += i->def_amount() - i->amount();
		}
	}
	return round(accumulate_discount);
/*
  double result = 0;
  for (int i=0; i<dish_count(); i++)
  {
    if(order_dish[i].coupon_id() != 0 && order_dish[i].dish_id() == 0)
    {
      result -= order_dish[i].price();
    }
    else
    {
      result += order_dish[i].def_amount() - order_dish[i].amount();
    }
  }
  return result;
*/
}

double order_t::subtotal() const
{
	double accumulate_subtotal =
		accumulate_proc(order_dish_.begin(), order_dish_.end(), 0.0, boost::bind(&order_dish_t::amount, _1));
/*
  double accumulate_subtotal = 0;
  std::vector<order_dish_t>::const_iterator end = order_dish_.end();
  for (std::vector<order_dish_t>::const_iterator i = order_dish_.begin(); i != end; ++i)
  {
    accumulate_subtotal += i->amount();
  }
*/
	return round(accumulate_subtotal);
}

double order_t::def_subtotal() const
{
	double accumulate_def_subtotal =
		accumulate_proc(order_dish_.begin(), order_dish_.end(), 0.0, boost::bind(&order_dish_t::def_amount, _1));
	return round(accumulate_def_subtotal);
}

double order_t::tax() const
{
	return round(sales_tax()*subtotal());
}

double order_t::def_tax() const
{
	return round(sales_tax()*def_subtotal());
}

double order_t::gratuity() const {
	return round(gratuity_tax()*subtotal());
}

double order_t::def_gratuity() const {
	return round(gratuity_tax()*def_subtotal());
}

double order_t::total() const {
	if (type_employee_meal()) return 0;
	if (type_delivery()) return subtotal()+tax()+delivery_fee()+gratuity();
	return subtotal()+tax()+gratuity();
}

double order_t::def_total() const {
	if (type_employee_meal()) return 0;
	if (type_delivery()) return def_subtotal()+tax()+delivery_fee()+def_gratuity();
	return round(def_subtotal()+def_tax()+def_gratuity());
}

double order_t::delivery_fee() const {
  if (!type_delivery() || subtotal() > cafe_.delivery_free_subtotal()) return 0;
  return cafe_.delivery_fee();
}

struct base_dish_t {
	base_dish_t(): change_(false), checked_(false) {}
	int order_dish_id_;
	int dish_id_;
	int count_;
	int count_wasted_;
	int count_registered_;
	double price_;
	double base_price_;
	int coupon_id_;
	int coupon_authoriser_id_;
	int ready_;
	std::string description_;
	std::string ki_version_;
	int kitchen_id_;
	mutable bool change_;
	mutable bool checked_;
};

std::vector<base_dish_t> loadDishes(int cafe_id, int order_id) {
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	
	std::string sqlStr = "SELECT "
	" ORDER_DISH_ID, "
	" DISH_ID, "
	" COUNT, "
	" PRICE, "
	" BASE_PRICE, "
	" COUPON_ID, "
	" COUNT_WASTED, "
	" READY, "
	" DESCRIPTION, "
	" KI_VERSION, "
	" COUPON_AUTHORISER_ID, "
	" KITCHEN_ID, "
	" COUNT_REGISTERED "
	" FROM KSI.ORDER_DISH "
	" WHERE ORDER_ID = :order_id and CAFE_ID = :cafe_id";
	query->create_statement(sqlStr);
	query->set("order_id", order_id);
	query->set("cafe_id", cafe_id);
	query->execute_statement();
	
	std::vector<base_dish_t> base_dish_vec;
	while (!query->next()) {
		base_dish_t base_dish;
		base_dish.order_dish_id_ = static_cast<int>(boost::get<double>(query->get(0)));
		base_dish.dish_id_ = static_cast<int>(boost::get<double>(query->get(1)));
		base_dish.count_ = static_cast<int>(boost::get<double>(query->get(2)));
		base_dish.price_ = boost::get<double>(query->get(3));
		base_dish.base_price_ = boost::get<double>(query->get(4));
		base_dish.coupon_id_ = static_cast<int>(boost::get<double>(query->get(5)));
		base_dish.count_wasted_ = static_cast<int>(boost::get<double>(query->get(6)));
		base_dish.ready_ = static_cast<int>(boost::get<double>(query->get(7)));
		base_dish.description_ = boost::get<std::string>(query->get(8));
		base_dish.ki_version_ = boost::get<std::string>(query->get(9));
		base_dish.coupon_authoriser_id_ = static_cast<int>(boost::get<double>(query->get(10)));
		base_dish.kitchen_id_ = static_cast<int>(boost::get<double>(query->get(11)));
		base_dish.count_registered_ = static_cast<int>(boost::get<double>(query->get(12)));
		base_dish_vec.push_back(base_dish);
	}
	return base_dish_vec;
}

struct base_modifier_t {
	base_modifier_t(): change_(false) {}
	int order_dish_id_;
	int modifier_id_;
	double price_;
	double base_price_;
	int count_;
	int count_wasted_;
	int count_registered_;
	int ready_;
	std::string ki_version_;
	int kitchen_id_;
	int coupon_id_;
	int coupon_authoriser_id_;
	int dish_id_;
	mutable bool change_;
};

std::vector<base_modifier_t> loadModifiers(int cafe_id, int order_id) {
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	
	std::string sqlStr = "SELECT "
	" ORDER_DISH_MODIFIER.ORDER_DISH_ID, "
	" ORDER_DISH_MODIFIER.MODIFIER_ID, "
	" ORDER_DISH_MODIFIER.PRICE, "
	" ORDER_DISH_MODIFIER.BASE_PRICE, "
	" ORDER_DISH_MODIFIER.COUNT, "
	" ORDER_DISH_MODIFIER.COUNT_WASTED, "
	" ORDER_DISH_MODIFIER.COUNT_REGISTERED, "
	" ORDER_DISH_MODIFIER.KI_VERSION, "
	" ORDER_DISH_MODIFIER.KITCHEN_ID, "
	" ORDER_DISH_MODIFIER.READY, "
	" ORDER_DISH_MODIFIER.COUPON_ID, "
	" ORDER_DISH_MODIFIER.COUPON_AUTHORISER_ID, "
	" NVL(MODIFIER.DISH_ID, 0) "
	" FROM KSI.ORDER_DISH "
	" INNER JOIN KSI.ORDER_DISH_MODIFIER ON ORDER_DISH_MODIFIER.ORDER_DISH_ID = ORDER_DISH.ORDER_DISH_ID "
	" 	AND ORDER_DISH_MODIFIER.CAFE_ID = ORDER_DISH.CAFE_ID "
	" LEFT JOIN KSI.MODIFIER ON MODIFIER.MODIFIER_ID = ORDER_DISH_MODIFIER.MODIFIER_ID "
	" WHERE ORDER_ID = :order_id AND ORDER_DISH.CAFE_ID = :cafe_id";
	query->create_statement(sqlStr);
	query->set("order_id", order_id);
	query->set("cafe_id", cafe_id);
	query->execute_statement();
	
	std::vector<base_modifier_t> base_modifier_vec;
	
	while (!query->next()) {
		base_modifier_t base_modifier;
		base_modifier.order_dish_id_ = static_cast<int>(boost::get<double>(query->get(0)));
		base_modifier.modifier_id_ = static_cast<int>(boost::get<double>(query->get(1)));
		base_modifier.price_ = boost::get<double>(query->get(2));
		base_modifier.base_price_ = boost::get<double>(query->get(3));
		base_modifier.count_ = static_cast<int>(boost::get<double>(query->get(4)));
		base_modifier.count_wasted_ = static_cast<int>(boost::get<double>(query->get(5)));
		base_modifier.count_registered_ = static_cast<int>(boost::get<double>(query->get(6)));
		base_modifier.ki_version_ = boost::get<std::string>(query->get(7));
		base_modifier.kitchen_id_ = static_cast<int>(boost::get<double>(query->get(8)));
		base_modifier.ready_ = static_cast<int>(boost::get<double>(query->get(9)));
		base_modifier.coupon_id_ = static_cast<int>(boost::get<double>(query->get(10)));
		base_modifier.coupon_authoriser_id_ = static_cast<int>(boost::get<double>(query->get(11)));
		base_modifier.dish_id_ = static_cast<int>(boost::get<double>(query->get(12)));
		base_modifier_vec.push_back(base_modifier);
	}
	return base_modifier_vec;
}

void order_t::splitByOrder(int order_id) {
	try {
		std::auto_ptr<logical_transaction_t> logical_trans;
		logical_trans.reset(new logical_transaction_t(session.get(), 150));
		
		typedef std::vector<base_dish_t>::const_iterator constBaseDishIter_t;
		typedef std::vector<base_dish_t>::iterator BaseDishIter_t;
		typedef std::vector<base_modifier_t>::const_iterator constBaseModifierIter_t;
		typedef std::vector<base_modifier_t>::iterator BaseModifierIter_t;

		std::vector<base_dish_t> base_dish_vec = loadDishes(cafe_.cafe_id(), order_id_);
		std::vector<base_modifier_t> base_modifier_vec = loadModifiers(cafe_.cafe_id(), order_id_);
		std::map<int, std::vector<BaseModifierIter_t> > base_modifier_map;
		for (constBaseDishIter_t dishIt=base_dish_vec.begin(); dishIt!=base_dish_vec.end(); ++dishIt) {
			int order_dish_id = dishIt->order_dish_id_;
			for (BaseModifierIter_t modIt=base_modifier_vec.begin(); modIt!=base_modifier_vec.end(); ++modIt) {
				if (modIt->order_dish_id_ == order_dish_id) {
					base_modifier_map[order_dish_id].push_back(modIt);
				}
			}
		}
		
		std::vector<base_dish_t> dest_base_dish_vec = loadDishes(cafe_.cafe_id(), order_id);
		std::vector<base_modifier_t> dest_base_modifier_vec = loadModifiers(cafe_.cafe_id(), order_id);
		std::map<int, std::vector<BaseModifierIter_t> > dest_base_modifier_map;
		for (constBaseDishIter_t dishIt=dest_base_dish_vec.begin(); dishIt!=dest_base_dish_vec.end(); ++dishIt) {
			int order_dish_id = dishIt->order_dish_id_;
			for (BaseModifierIter_t modIt=dest_base_modifier_vec.begin(); modIt!=dest_base_modifier_vec.end(); ++modIt) {
				if (modIt->order_dish_id_ == order_dish_id) {
					dest_base_modifier_map[order_dish_id].push_back(modIt);
				}
			}
		}
		
		typedef std::map<BaseDishIter_t, std::vector<BaseDishIter_t> > link_t;
		link_t link;
		for (BaseDishIter_t destDishIt=dest_base_dish_vec.begin(); destDishIt!=dest_base_dish_vec.end(); ++destDishIt) {
			for (BaseDishIter_t dishIt=base_dish_vec.begin(); dishIt!=base_dish_vec.end(); ++dishIt) {
				if (destDishIt->dish_id_ == dishIt->dish_id_ 
				&& !dishIt->checked_
				&& dishIt->count_ > 0
				&& dest_base_modifier_map[destDishIt->order_dish_id_].size() == base_modifier_map[dishIt->order_dish_id_].size()) {
					bool modifierEqual = true;
					std::vector<BaseModifierIter_t> destBaseModifiers = dest_base_modifier_map[destDishIt->order_dish_id_];
					std::vector<BaseModifierIter_t> baseModifiers = base_modifier_map[dishIt->order_dish_id_];
					for (size_t i=0; i<destBaseModifiers.size(); ++i) {
						int modifier_id = destBaseModifiers[i]->modifier_id_;
						int count = destBaseModifiers[i]->count_;
						bool finded = false;
						for (size_t j=0; j<baseModifiers.size(); ++j) {
							int currModifier_id = baseModifiers[j]->modifier_id_;
							int currCount = baseModifiers[j]->count_;
							if (currModifier_id == modifier_id && currCount == count) {
								finded = true;
								break;
							}
						}
						if (!finded) {
							modifierEqual = false;
							break;
						}
					}
					if (modifierEqual) {
						for (size_t i=0; i<baseModifiers.size(); ++i) {
							int modifier_id = baseModifiers[i]->modifier_id_;
							int count = baseModifiers[i]->count_;
							bool finded = false;
							for (size_t j=0; j<destBaseModifiers.size(); ++j) {
								int currModifier_id = destBaseModifiers[j]->modifier_id_;
								int currCount = destBaseModifiers[j]->count_;
								if (currModifier_id = modifier_id && currCount == count) {
									finded = true;
									break;
								}
							}
							if (!finded) {
								modifierEqual = false;
								break;
							}
						}
					}
					if (modifierEqual) {
						dishIt->checked_ = true;
						link[destDishIt].push_back(dishIt);
					}
				}
			}
		}
		
		BOOST_FOREACH(const link_t::value_type& pair, link) {
			BaseDishIter_t destDishIter = pair.first;
			int MaxCount = destDishIter->count_;
			const std::vector<BaseDishIter_t>& order_dish_vec = pair.second;

			for (size_t i=0; i<order_dish_vec.size(); ++i) {
				BaseDishIter_t currDishIter = order_dish_vec[i];
				int delta = currDishIter->count_registered_ - currDishIter->count_ - currDishIter->count_wasted_;
				if (delta > 0 && MaxCount > 0) {
					int effectedCount = delta <= MaxCount ? delta : MaxCount;
					MaxCount -= effectedCount;
					destDishIter->count_registered_ += effectedCount;
					currDishIter->count_registered_ -= effectedCount;
					if (effectedCount > 0) {
						destDishIter->change_ = true;
						currDishIter->change_ = true;
						std::vector<BaseModifierIter_t> destModifiers = dest_base_modifier_map[destDishIter->order_dish_id_];
						std::vector<BaseModifierIter_t> currModifiers = base_modifier_map[currDishIter->order_dish_id_];
						for (size_t j=0; j<destModifiers.size(); ++j) {	
							BaseModifierIter_t destModifierIter = destModifiers[j];
							if (destModifierIter->dish_id_ != 0) {
								for (size_t k=0; k<currModifiers.size(); ++k) {
									BaseModifierIter_t currModifierIter = currModifiers[k];
									if (currModifierIter->count_ == destModifierIter->count_ 
									&& currModifierIter->modifier_id_ == destModifierIter->modifier_id_) {
										int mod_delta = currModifierIter->count_registered_-currDishIter->count_*currModifierIter->count_-currModifierIter->count_wasted_;
										if (mod_delta > 0) {
											int destModCount = destDishIter->count_*destModifierIter->count_+destModifierIter->count_wasted_;
											int effectedCnt = mod_delta > destModCount ? destModCount : mod_delta;
											if (effectedCnt > 0) {
												destModifierIter->count_registered_ += effectedCnt;
												currModifierIter->count_registered_ -= effectedCnt;
												destModifierIter->change_ = true;
												currModifierIter->change_ = true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		std::string sqlStr = "UPDATE KSI.ORDER_DISH SET COUNT_REGISTERED=:count_registered "
		" WHERE CAFE_ID=:cafe_id AND ORDER_DISH_ID=:order_dish_id";
		std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
		query->create_statement(sqlStr);
		
		sqlStr = "UPDATE KSI.ORDER_DISH_MODIFIER SET COUNT_REGISTERED=:count_registered "
		" WHERE CAFE_ID=:cafe_id AND ORDER_DISH_ID=:order_dish_id AND MODIFIER_ID=:modifier_id";
		std::auto_ptr<ksi_client::oracle_query_i> mquery = session->create_query();
		mquery->create_statement(sqlStr);
		
		BOOST_FOREACH(const link_t::value_type& pair, link) {
			BaseDishIter_t destDishIter = pair.first;
			if (destDishIter->change_) {
				query->set("cafe_id", cafe_.cafe_id());
				query->set("order_dish_id", destDishIter->order_dish_id_);
				query->set("count_registered", destDishIter->count_registered_);
				query->execute_statement();
				std::vector<BaseModifierIter_t> modifier_vec = dest_base_modifier_map[destDishIter->order_dish_id_];
				for (size_t i=0; i<modifier_vec.size(); ++i) {
					BaseModifierIter_t modifierIter = modifier_vec[i];
					if (modifierIter->change_) {
						mquery->set("cafe_id", cafe_.cafe_id());
						mquery->set("order_dish_id", destDishIter->order_dish_id_);
						mquery->set("modifier_id", modifierIter->modifier_id_);
						mquery->set("count_registered", modifierIter->count_registered_);
						mquery->execute_statement();
					}
				}
				const std::vector<BaseDishIter_t>& order_dish_vec = pair.second;
				for (size_t i=0; i<order_dish_vec.size(); ++i) {
					BaseDishIter_t currDishIter = order_dish_vec[i];
					if (currDishIter->change_) {
						query->set("cafe_id", cafe_.cafe_id());
						query->set("order_dish_id", currDishIter->order_dish_id_);
						query->set("count_registered", currDishIter->count_registered_);
						query->execute_statement();
						std::vector<BaseModifierIter_t> modifier_vec = base_modifier_map[currDishIter->order_dish_id_];
						for (size_t j=0; j<modifier_vec.size(); ++j) {
							BaseModifierIter_t modifierIter = modifier_vec[j];
							if (modifierIter->change_) {
								mquery->set("cafe_id", cafe_.cafe_id());
								mquery->set("order_dish_id", currDishIter->order_dish_id_);
								mquery->set("modifier_id", modifierIter->modifier_id_);
								mquery->set("count_registered", modifierIter->count_registered_);
								mquery->execute_statement();
							}
						}
					}
				}
				
			}
		}
		
	}
	catch (...) {
		throw;
	}
}

void order_t::sync_order(bool trans_log) {
	if (!order_id_) return;

	std::auto_ptr<logical_transaction_t> logical_trans;

	try {
		if (trans_log) logical_trans.reset(new logical_transaction_t(session.get(), 150));
		order_t order(cafe_, order_id_);

		std::map<std::string, int> base_count;
		std::map<std::string, int> base_count_wasted;
		std::map<std::string, int> base_count_registered;
		std::map<std::string, int> base_count_ready;

		std::map<std::string, int> base_mod_count;
		std::map<std::string, int> base_mod_count_wasted;
		std::map<std::string, int> base_mod_count_registered;
		std::map<std::string, int> base_mod_count_ready;

		std::vector<int> base_order_dish_id;

		for (int i=0; i<order.dish_count(); ++i) {
			base_order_dish_id.push_back(order.dish(i).order_dish_id());
			std::string type_check_sum = order.dish(i).type_check_sum();
			base_count[type_check_sum] += order.dish(i).total_count();
			base_count_wasted[type_check_sum] += order.dish(i).count_wasted();
			base_count_registered[type_check_sum] += order.dish(i).count_registered();
			base_count_ready[type_check_sum] += order.dish(i).count_ready();

			if (order.dish(i).dish_id() != 0)
				for (size_t j=0; j<order.dish(i).modifier_vec().size(); ++j) {
					std::string mod_type_check_sum = order.dish(i).modifier_vec()[j].type_check_sum();
					base_mod_count[type_check_sum+mod_type_check_sum] += order.dish(i).modifier_vec()[j].count();
					base_mod_count_wasted[type_check_sum+mod_type_check_sum] += order.dish(i).modifier_vec()[j].count_wasted();
					base_mod_count_registered[type_check_sum+mod_type_check_sum] += order.dish(i).modifier_vec()[j].count_registered();
					base_mod_count_ready[type_check_sum+mod_type_check_sum] += order.dish(i).modifier_vec()[j].count_ready();
				}
		}

		for (int i=0; i<dish_count(); ++i) {
			std::string type_check_sum = dish(i).type_check_sum();
			base_count_wasted[type_check_sum] += dish(i).count_wasted();
			
			if (dish(i).dish_id() != 0)
				for (size_t j=0; j<dish(i).modifier_vec().size(); ++j) {
					std::string mod_type_check_sum = dish(i).modifier_vec()[j].type_check_sum();
					base_mod_count_wasted[type_check_sum+mod_type_check_sum] += dish(i).modifier_vec()[j].count()*dish(i).count_wasted();
				}
		}

		std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
		std::auto_ptr<ksi_client::oracle_query_i> mquery = session->create_query();
		query->create_statement
		(
		"update ksi.orders set subtotal = :subtotal, sales_tax = :tax, gratuity  = :gratuity\
		where cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end\
		and order_id = :order_id"
		);

		query->set("cafe_id", cafe_.cafe_id());
		query->set("order_id", order_id());
		query->set("subtotal", subtotal());
		query->set("tax", tax());
		query->set("gratuity", gratuity());
		query->execute_statement();

		query->create_statement
		(
		"delete from ksi.order_dish \
		where order_id=:order_id \
		and cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end"
		);

		query->set("order_id", order_id());
		query->set("cafe_id", cafe_.cafe_id());
		query->execute_statement();

		query->create_statement
		(
		"insert into ksi.order_dish \
		( \
		  order_dish_id \
		, order_id \
		, cafe_id \
		, dish_id \
		, price \
		, base_price \
		, coupon_id \
		, coupon_authoriser_id \
		, count \
		, count_wasted \
		, count_registered \
		, ready \
		, description \
		, time \
		, ki_version \
		) \
		values \
		( \
		  :order_dish_id \
		, :order_id \
		, case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
		, case when :dish_id = 0 then null else :dish_id end \
		, :price \
		, :base_price \
		, case when :coupon_id = 0 then null else :coupon_id end \
		, case when :coupon_authoriser_id = 0 then null else :coupon_authoriser_id end \
		, :count \
		, :count_wasted \
		, :count_registered \
		, :ready \
		, case when :description = '' then null else :description end \
		, sysdate \
		, :ki_version \
		)"
		);


		mquery->create_statement
		(
		"insert into ksi.order_dish_modifier \
		( \
		  order_dish_id \
		, modifier_id \
		, cafe_id \
		, price \
		, base_price \
		, count \
		, count_wasted \
		, count_registered \
		, ready \
		, coupon_id \
		, coupon_authoriser_id \
		) \
		values \
		( \
		  :order_dish_id \
		, :modifier_id \
		, case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
		, :price \
		, :base_price \
		, :count \
		, :count_wasted \
		, :count_registered \
		, :ready \
		, case when :coupon_id = 0 then null else :coupon_id end \
		, case when :coupon_authoriser_id = 0 then null else :coupon_authoriser_id end \
		)"
		);

		seq_t seq(session.get(), "order_dish_seq");
		size_t index_seq = 0;

		for (int i=0; i<dish_count(); ++i) {
			std::string type_check_sum = dish(i).type_check_sum();

			int count_wasted = 0;
			int count_registered = 0;
			int count_ready = 0;

			bool last_type = true;
			for (int j=i+1; j<dish_count(); ++j) {
				std::string cur_type_check_sum = dish(j).type_check_sum();
				if (cur_type_check_sum == type_check_sum) last_type = false;
			}

			if (base_count_wasted[type_check_sum] < dish(i).total_count() || last_type) {
				count_wasted = base_count_wasted[type_check_sum];
				base_count_wasted[type_check_sum] = 0;
			}
			else {
				count_wasted = dish(i).total_count();
				base_count_wasted[type_check_sum] -= count_wasted;
			}

			if (base_count_registered[type_check_sum] < dish(i).total_count() || last_type) {
				count_registered = base_count_registered[type_check_sum];
				base_count_registered[type_check_sum] = 0;
			}
			else {
				count_registered = dish(i).total_count();
				base_count_registered[type_check_sum] -= count_registered;
			}

			if (base_count_ready[type_check_sum] < dish(i).total_count() || last_type) {
				count_ready = base_count_ready[type_check_sum];
				base_count_ready[type_check_sum] -= 0;
			}
			else {
				count_ready = dish(i).total_count();
				base_count_ready[type_check_sum] -= count_ready;
			}

			if (!count_registered && !(dish(i).total_count()) && !count_wasted) continue;

			int order_dish_id = (index_seq < base_order_dish_id.size() ? base_order_dish_id[index_seq++] : seq.nextval());

			query->set("cafe_id", cafe_.cafe_id());
			query->set("order_dish_id", order_dish_id);
			query->set("order_id", order_id());
			query->set("dish_id", dish(i).dish_id());
			query->set("count", dish(i).total_count());
			query->set("price", dish(i).price());
			query->set("base_price", dish(i).def_price());
			query->set("coupon_id", dish(i).coupon_id());
			query->set("count_wasted", count_wasted);
			query->set("count_registered", count_registered);
			query->set("ready", count_ready);
			query->set("description", dish(i).description());
			query->set("ki_version", "");
			query->set("coupon_id", dish(i).coupon_id());
			query->set("coupon_authoriser_id", dish(i).coupon_mng_id());
			query->execute_statement();

			for (size_t j=0; j<dish(i).modifier_vec().size(); ++j) {
				int count_registered = 0;
				int count_wasted = 0;
				int count_ready = 0;
				std::string mod_type_check_sum = dish(i).modifier_vec()[j].type_check_sum();
				bool last_type = true;
				for (int k=i+1; k<dish_count(); k++) {
					std::string cur_type_check_sum = dish(k).type_check_sum();
					for (size_t p=0; p<dish(k).modifier_vec().size(); ++p) {
						std::string cur_mod_type_check_sum = dish(k).modifier_vec()[p].type_check_sum();
						if (type_check_sum+mod_type_check_sum == cur_type_check_sum+cur_mod_type_check_sum) last_type = false;
					}
				}
				if (base_mod_count_ready[type_check_sum+mod_type_check_sum] < dish(i).total_count()*dish(i).modifier_vec()[j].count() || last_type) {
					count_ready = base_mod_count_ready[type_check_sum+mod_type_check_sum];
					base_mod_count_ready[type_check_sum+mod_type_check_sum] = 0;
				}
				else {
					count_ready = dish(i).total_count()*dish(i).modifier_vec()[j].count();
					base_mod_count_ready[type_check_sum+mod_type_check_sum] -= count_ready;
				}

				if (dish(i).modifier_vec()[j].mod_dish_id() != 0) {
					if (base_mod_count_wasted[type_check_sum+mod_type_check_sum] < dish(i).total_count()*dish(i).modifier_vec()[j].count() || last_type) {
						count_wasted = base_mod_count_wasted[type_check_sum+mod_type_check_sum];
						base_mod_count_wasted[type_check_sum+mod_type_check_sum] = 0;
					}
					else {
						count_wasted = dish(i).total_count()*dish(i).modifier_vec()[j].count();
						base_mod_count_wasted[type_check_sum+mod_type_check_sum] -= count_wasted;
					}

					if (base_mod_count_registered[type_check_sum+mod_type_check_sum] < dish(i).total_count()*dish(i).modifier_vec()[j].count() || last_type) {
						count_registered = base_mod_count_registered[type_check_sum+mod_type_check_sum];
						base_mod_count_registered[type_check_sum+mod_type_check_sum] = 0;
					}
					else {
						count_registered = dish(i).total_count()*dish(i).modifier_vec()[j].count();
						base_mod_count_registered[type_check_sum+mod_type_check_sum] -= count_registered;
					}
				}
				
				mquery->set("cafe_id", cafe_.cafe_id());
				mquery->set("order_dish_id", order_dish_id);
				mquery->set("count", dish(i).modifier_vec()[j].count());
				mquery->set("modifier_id", dish(i).modifier_vec()[j].modifier_id());
				mquery->set("price", dish(i).modifier_vec()[j].price());
				mquery->set("base_price", dish(i).modifier_vec()[j].def_price());
				mquery->set("count_wasted", count_wasted);
				mquery->set("count_registered", count_registered);
				mquery->set("ready", count_ready);
				mquery->set("coupon_id", dish(i).modifier_vec()[j].coupon_id());
				mquery->set("coupon_authoriser_id", dish(i).modifier_vec()[j].coupon_mng_id());
				mquery->execute_statement();
			}
		}
	}
	catch (...) {
		throw;
	}
}


void order_t::custom_load(bool for_update, bool nowait)
{
	std::auto_ptr<ora_transaction_t> trans;
	if (for_update) {
		order_trans_.reset();
		trans.reset(new ora_transaction_t(session.get()));
	}

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	if (!table_.exist() && table_.table_group_id() == 0) {
		/*
		query->create_statement
		(
		"select table_group_id, name from ksi.table_group \
		where cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
		and deleted is null order by is_default desc"
		);
		query->set("cafe_id", cafe_.cafe_id());
		query->execute_statement();
		int base_table_group_id = static_cast<int>(boost::get<double>(query->get(0)));
		std::string base_table_group_name = boost::get<std::string>(query->get(1));
		*/
		table_ = table_t(cafe_.default_table_group_id(), cafe_.default_table_group_name());
	}		
	
	if (order_id()) {
		std::auto_ptr<ksi_client::oracle_query_i> query2 = session->create_query();

		if (for_update) {
			query2->create_statement
			(
			"select order_id from ksi.order_dish \
			where order_id = :order_id \
			and cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
			for update nowait"
			);
			query2->set("cafe_id", cafe_.cafe_id());
			query2->set("order_id", order_id());
			
			if (!nowait) {
				int i = 0;
				for (; i<10; ++i) {
					try {
						query2->execute_statement();
						i = 22;
					}
					catch (ksi_client::ksi_exception& ex) {
						continue;
					}
				}
				if (i != 23) {
					THROW_LOCATION(10112, "order_t::custom_load(bool, bool)");
				}
			}
			else {
				try {
					query2->execute_statement();
				}
				catch (ksi_client::ksi_exception& ex) {
					THROW_LOCATION(10119, "order_t::custom_load(bool, bool)");
				}
			}
		}

		query->create_statement
		(
		"SELECT "
		" ORDER_DISH.DISH_ID "
		", ORDER_DISH.COUNT "
		", ORDER_DISH.ORDER_DISH_ID "
		", ORDER_DISH.BASE_PRICE "
		", ORDER_DISH.PRICE "
		", DISH.NAME "
		", ORDER_DISH.COUNT_WASTED "
		", ORDER_DISH.COUNT_REGISTERED "
		", ORDER_DISH.READY "
		", TIME "
		", DISH.IS_BEVERAGE "
		", ORDER_DISH.COUPON_ID "
		", COUPON.TITLE "
		", ORDER_DISH.DESCRIPTION "
		", DISH.CODE "
		", DISH.EMPLOYEE_MEAL "
		", ORDER_DISH.COUPON_AUTHORISER_ID "
		" FROM KSI.ORDER_DISH "
		" LEFT JOIN KSI.DISH ON DISH.DISH_ID = ORDER_DISH.DISH_ID "
		" LEFT JOIN KSI.COUPON ON COUPON.COUPON_ID = ORDER_DISH.COUPON_ID "
		" WHERE ORDER_ID = :order_id "
		" AND CAFE_ID = CASE WHEN :cafe_id = 0 THEN KSI.UTIL.GET_CUR_CAFE_ID ELSE :cafe_id END"
		);
		query->set("order_id", order_id());
		query->set("cafe_id", cafe_.cafe_id());
		query->execute_statement();

		query2->create_statement
		(
		"SELECT "
		" ORDER_DISH_MODIFIER.MODIFIER_ID "
		", ORDER_DISH_MODIFIER.COUNT "
		", ORDER_DISH_MODIFIER.PRICE "
		", ORDER_DISH_MODIFIER.BASE_PRICE "
		", MODIFIER.NAME "
		", MODIFIER.DISH_ID "
		", DISH.IS_BEVERAGE "
		", ORDER_DISH_MODIFIER.COUNT_WASTED "
		", ORDER_DISH_MODIFIER.COUNT_REGISTERED "
		", ORDER_DISH_MODIFIER.READY "
		", MODIFIER.GROUP_ID "
		", ORDER_DISH_MODIFIER.COUPON_ID "
		", ORDER_DISH_MODIFIER.COUPON_AUTHORISER_ID "
		" FROM KSI.ORDER_DISH_MODIFIER "
		" LEFT JOIN KSI.MODIFIER ON MODIFIER.MODIFIER_ID = ORDER_DISH_MODIFIER.MODIFIER_ID "
		" LEFT JOIN KSI.DISH ON DISH.DISH_ID = MODIFIER.DISH_ID "
		" WHERE ORDER_DISH_ID = :order_dish_id "
		" AND ORDER_DISH_MODIFIER.CAFE_ID = CASE WHEN :cafe_id = 0 THEN KSI.UTIL.GET_CUR_CAFE_ID ELSE :cafe_id END"
		);

		query2->set("cafe_id", cafe_.cafe_id());

		while (!query->next()) {
			int dish_id = static_cast<int>(boost::get<double>(query->get(0)));
			int count = static_cast<int>(boost::get<double>(query->get(1)));
			int order_dish_id = static_cast<int>(boost::get<double>(query->get(2)));
			double def_price = boost::get<double>(query->get(3));
			double price = boost::get<double>(query->get(4));
			std::string dish_name = boost::get<std::string>(query->get(5));
			int count_wasted = static_cast<int>(boost::get<double>(query->get(6)));
			int count_registered = static_cast<int>(boost::get<double>(query->get(7)));
			int count_ready = static_cast<int>(boost::get<double>(query->get(8)));
			boost::posix_time::ptime time = boost::get<boost::posix_time::ptime>(query->get(9));
			int kitchen_type_id = static_cast<int>(boost::get<double>(query->get(10)));
			int coupon_id = static_cast<int>(boost::get<double>(query->get(11)));
			std::string title = boost::get<std::string>(query->get(12));
			std::string description = boost::get<std::string>(query->get(13));
			std::string dish_code = boost::get<std::string>(query->get(14));
			int appl_employee_meal = static_cast<int>(boost::get<double>(query->get(15)));
			int coupon_mng_id = static_cast<int>(boost::get<double>(query->get(16)));

			if (dish_id == 0 && coupon_id == 0) dish_name = "misc charges: " + description;
			else if (dish_id == 0 && coupon_id != 0) dish_name = title;

			query2->set("order_dish_id", order_dish_id);
			query2->execute_statement();

			std::vector<order_dish_modifier_t> mod_vec;

			while (!query2->next()) {
				int modifier_id = static_cast<int>(boost::get<double>(query2->get(0)));
				int count = static_cast<int>(boost::get<double>(query2->get(1)));
				double price = boost::get<double>(query2->get(2));
				double def_price = boost::get<double>(query2->get(3));
				std::string modifier_name = boost::get<std::string>(query2->get(4));
				int modifier_dish_id = static_cast<int>(boost::get<double>(query2->get(5)));
				int kitchen_type_id = static_cast<int>(boost::get<double>(query2->get(6)));
				int count_wasted = static_cast<int>(boost::get<double>(query2->get(7)));
				int count_registered = static_cast<int>(boost::get<double>(query2->get(8)));
				int count_ready = static_cast<int>(boost::get<double>(query2->get(9)));
				int modifier_group_id = static_cast<int>(boost::get<double>(query2->get(10)));
				int coupon_id = static_cast<int>(boost::get<double>(query2->get(11)));
				int coupon_mng_id = static_cast<int>(boost::get<double>(query2->get(12)));

				modifier_t modifier = modifier_t(modifier_group_id, modifier_id, modifier_dish_id, kitchen_type_id, modifier_name, 0, 0);
				menu_dish_modifier_t menu_dish_modifier = menu_dish_modifier_t(modifier, 0, dish_id, def_price);
				mod_vec.push_back(order_dish_modifier_t(menu_dish_modifier, order_dish_id, price, count, count_wasted, count_registered, count_ready));
				if (coupon_id != 0) {
					(mod_vec.end()-1)->set_coupon_id(coupon_id, coupon_mng_id);
				}
			}

			dish_t dish = dish_t(dish_id, dish_name, dish_code, 0, kitchen_type_id, (appl_employee_meal == 1));
			menu_dish_t menu_dish = menu_dish_t(dish, 0, def_price, 0, false);
			order_dish_.push_back(order_dish_t(menu_dish, count, count_wasted, count_registered, count_ready, price, mod_vec, order_dish_id, time));
			(order_dish_.end()-1)->set_description(description);

			if (coupon_id) {
				(order_dish_.end()-1)->set_coupon_id(coupon_id, coupon_mng_id);
			}
			if (employee_id_) {
				(order_dish_.end()-1)->set_employee_id(employee_id_);
			}
		}	
	
		query->create_statement
		(
		"select payment_type_id \
					, prepayment_type_id \
					, meal_person_id \
					, type_id \
					, guest_count \
					, status_id \
					, creator_id \
					, created \
					, closed \
					, orders.table_group_id \
					, tables \
					, token \
					, prepayment \
					, table_group.name as table_group_name \
					, delivery_id \
					, memo \
					, refund_note \
					, flags \
					, tables.table_id \
					, token.token_id \
		from ksi.orders \
		left join ksi.table_group on table_group.table_group_id = orders.table_group_id \
		left join ksi.tables on tables.name = orders.tables and tables.table_group_id = orders.table_group_id \
		and table_group.cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
		left join ksi.token on token.name = orders.token \
		where order_id = :order_id and orders.cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end"
		);
		query->set("order_id", order_id());
		query->set("cafe_id", cafe_.cafe_id());
		query->execute_statement();
		if (query->next()) THROW_MESSAGE(10208, "order_id = " + boost::lexical_cast<std::string>(order_id()) + " not found", "order_t::custom_load(bool, bool)");
		payment_type_id_ = static_cast<int>(boost::get<double>(query->get(0)));
		prepayment_type_id_ = static_cast<int>(boost::get<double>(query->get(1)));
		employee_id_ = static_cast<int>(boost::get<double>(query->get(2)));
		type_id_ = static_cast<int>(boost::get<double>(query->get(3)));
		guest_count_ = static_cast<int>(boost::get<double>(query->get(4)));
		status_id_ = static_cast<int>(boost::get<double>(query->get(5)));
		creator_id_ = static_cast<int>(boost::get<double>(query->get(6)));
		order_created_ = boost::get<boost::posix_time::ptime>(query->get(7));
		order_closed_ = boost::get<boost::posix_time::ptime>(query->get(8));
		double prepayment_from_base = boost::get<double>(query->get(12));
		delivery_id_ = static_cast<int>(boost::get<double>(query->get(14)));
		memo_ = boost::get<std::string>(query->get(15));
		refund_note_ = boost::get<std::string>(query->get(16));
		flags_ = static_cast<int>(boost::get<double>(query->get(17)));
		
		if (!table_.exist()) {
			int base_table_group_id =	static_cast<int>(boost::get<double>(query->get(9)));
			int base_table_id = static_cast<int>(boost::get<double>(query->get(18)));
			std::string base_table_group_name = boost::get<std::string>(query->get(13));
			std::string base_table_name = boost::get<std::string>(query->get(10));
			table_ = table_t(base_table_group_id, base_table_group_name, base_table_id, base_table_name);
		}
		if (!token_.exist()) {
			std::string base_token_name = boost::get<std::string>(query->get(11));
			int base_token_id = static_cast<int>(boost::get<double>(query->get(19)));
			token_ = token_t(base_token_id, base_token_name);
		}

		if (payment_type_id_ == 2) {
			query->create_statement
			(
			"select type_id \
						, amount \
						, tips \
						, transaction_id \
						, last_4_digits \
						, auth_code \
						, swiped \
						, card_type \
						, cardholder_name \
			from (select type_id \
					, amount, tips \
					, transaction_id \
					, last_4_digits \
					, auth_code \
					, swiped \
					, card_type \
					, cardholder_name \
					from ksi.order_cctrans \
					where order_id = :order_id \
					and cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
					order by trans_id desc, tran_date desc) \
			where ROWNUM = 1"
			);
			query->set("cafe_id", cafe_.cafe_id());
			query->set("order_id", order_id());
			query->execute_statement();
			
			if (!query->next()) {
				int base_type_id = static_cast<int>(boost::get<double>(query->get(0)));
				double amount = boost::get<double>(query->get(1));
				int auth_txn_id = static_cast<int>(boost::get<double>(query->get(3)));
				std::string last_4_digits = boost::get<std::string>(query->get(4));
				std::string auth_code = boost::get<std::string>(query->get(5));
				int swiped = static_cast<int>(boost::get<double>(query->get(6)));
				std::string card_type = boost::get<std::string>(query->get(7));
				std::string holder_name = boost::get<std::string>(query->get(8));

				if (base_type_id == 5) { // credit_auth
					card_info_t card_info = card_info_t(last_4_digits, card_type, holder_name, (swiped == 1));
					credit_auth_t credit_auth = credit_auth_t(card_info, auth_txn_id, auth_code);
					payment_.reset(new credit_payment_t(credit_auth, amount));
				}
			}
		}
		else {
			/* Not supported */
		}

		//if (delivery_id_ && (type_id_ == 3 || type_id_ == 4 || type_id_ == 6))
		if (delivery_id_) {
			delivery_info_t* info = make_delivery_info(delivery_id_, cafe_.cafe_id()).release();
			if (type_id_ == 1) set_type_for_here(info);
			else if (type_id_ == 3) set_type_delivery(info);
			else if (type_id_ == 4) set_type_pickup(info);
			else if (type_id_ == 6) set_type_table_booking(info);

			if (prepayment_from_base != 0) {
				if (prepayment_type_id_ == 1) { // cash 
					prepayment_.reset(new cash_prepayment_t(prepayment_from_base));
				}
				else if (prepayment_type_id_ == 2) { // credit card
					query->create_statement
					(
					"select type_id \
								, amount \
								, tips \
								, transaction_id \
								, ref_transaction_id \
								, last_4_digits \
								, auth_code \
								, swiped \
								, card_type \
								, cardholder_name \
								, batch_id \
					from (select type_id \
									, amount \
									, tips \
									, transaction_id \
									, last_4_digits \
									, auth_code \
									, swiped \
									, card_type \
									, cardholder_name \
									, ref_transaction_id \
									, batch_id \
								from ksi.order_cctrans \
								where order_id = :order_id and cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
								order by tran_date desc) \
					where ROWNUM = 1"
					);
					query->set("cafe_id", cafe_.cafe_id());
					query->set("order_id", order_id());
					query->execute_statement();
					int txn_type_id = static_cast<int>(boost::get<double>(query->get(0)));
					double amount = boost::get<double>(query->get(1));
					double tips = boost::get<double>(query->get(2));
					int auth_txn_id = static_cast<int>(boost::get<double>(query->get(3)));
					std::string last_4_digits = boost::get<std::string>(query->get(5));
					std::string auth_code = boost::get<std::string>(query->get(6));
					int swiped = static_cast<int>(boost::get<double>(query->get(7)));
					std::string card_type = boost::get<std::string>(query->get(8));
					std::string card_holder = boost::get<std::string>(query->get(9));
					if (txn_type_id == 5) // credit_auth
					{
						card_info_t card_info = card_info_t(last_4_digits, card_type, card_holder, (swiped == 1));
						credit_auth_t credit_auth = credit_auth_t(card_info, auth_txn_id, auth_code);
						prepayment_.reset(new credit_prepayment_t(credit_auth, amount));
					}
					else if (txn_type_id == 1) // add_to_batch
					{
						int txn_id = auth_txn_id;
						auth_txn_id = static_cast<int>(boost::get<double>(query->get(4)));
						int batch_id = static_cast<int>(boost::get<double>(query->get(10)));
						query->create_statement
						(
						"select auth_code \
						from ksi.order_cctrans \
						where transaction_id = :transaction_id \
						and order_id = :order_id \
						and cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end"
						);
						query->set("cafe_id", cafe_.cafe_id());
						query->set("order_id", order_id());
						query->set("transaction_id", auth_txn_id);
						query->execute_statement();
						auth_code = boost::get<std::string>(query->get(0));
						card_info_t card_info = card_info_t(last_4_digits, card_type, card_holder, (swiped == 1));
						credit_auth_t credit_auth = credit_auth_t(card_info, auth_txn_id, auth_code);
						credit_prepayment_t* prep = new credit_prepayment_t(credit_auth, amount);
						prepayment_.reset(prep);
						prep->credit_add_to_batch(txn_id, batch_id, prep->amount(), tips);
					}
					else
					{
						/* Not supported */
					}
				}
				else if (prepayment_type_id_ == 3) { // check
					query->create_statement
					(
					"select check_number \
								, amount \
								, manager_id \
					from ksi.order_check \
					where order_id = :order_id \
						and cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
					order by trans_id"
					);
					query->set("cafe_id", cafe_.cafe_id());
					query->set("order_id", order_id());
					query->execute_statement();

					int check_num = static_cast<int>(boost::get<double>(query->get(0)));
					double amount = boost::get<double>(query->get(1));
					int mng_id = static_cast<int>(boost::get<double>(query->get(2)));
					prepayment_.reset(new check_prepayment_t(mng_id, check_num, amount));
				}
				else {
					 /* Not supported */
				}
			}

		}
		else {
			/* Not supported */
		}
	}
	
	order_trans_.reset(trans.release());
}


order_t::order_t(const cafe_t& cafe_, int order_id_)
: cafe_(cafe_), order_id_(order_id_), prepayment_type_id_(0), payment_type_id_(0)
, status_id_(0), type_id_(1), delivery_id_(0), employee_id_(0), table_(), token_()
, guest_count_(0), creator_id_(0), flags_(0)
{
	custom_load(false, false);
}

order_t::order_t(const cafe_t& cafe_, int order_id_, bool for_update, bool nowait)
: cafe_(cafe_), order_id_(order_id_), prepayment_type_id_(0), payment_type_id_(0)
, status_id_(0), type_id_(1), delivery_id_(0), employee_id_(0), table_(), token_()
, guest_count_(0), creator_id_(0), flags_(0)
{
	custom_load(for_update, nowait);
}

order_t::order_t(const cafe_t& cafe_, const token_t& token_, int order_id_, bool for_update, bool nowait)
: cafe_(cafe_), order_id_(order_id_), prepayment_type_id_(0), payment_type_id_(0)
, status_id_(0), type_id_(1), delivery_id_(0), employee_id_(0), table_(), token_(token_)
, guest_count_(0), creator_id_(0), flags_(0)
{
	custom_load(for_update, nowait);
}

order_t::order_t(const cafe_t& cafe_, const table_t& table_, int order_id_, bool for_update, bool nowait)
: cafe_(cafe_), order_id_(order_id_), prepayment_type_id_(0), payment_type_id_(0)
, status_id_(0), type_id_(1), delivery_id_(0), employee_id_(0), table_(table_), token_()
, guest_count_(0), creator_id_(0), flags_(0)
{
	custom_load(for_update, nowait);
}

order_t::order_t(const cafe_t& cafe_, const table_t& table_, const token_t& token_, int order_id_, bool for_update, bool nowait)
: cafe_(cafe_), order_id_(order_id_), prepayment_type_id_(0), payment_type_id_(0)
, status_id_(0), type_id_(1), delivery_id_(0), employee_id_(0), table_(table_), token_(token_)
, guest_count_(0), creator_id_(0), flags_(0)
{
	custom_load(for_update, nowait);
}


void order_t::set_accept(int cafe_id2, bool logical_trans)
{
	if (!order_trans_.get()) order_trans_.reset(new ora_transaction_t(session.get()));

	std::auto_ptr<logical_transaction_t> log_trans;

	int save_order_id = order_id_;

	if (payment_credit_card() || prepayment_credit_card() || type_employee_meal()) {
		set_hide(false);
	}

	if (logical_trans)
    log_trans.reset(new logical_transaction_t(session.get(), 91));

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();


	if (order_id() == 0) {
		seq_t seq = seq_t(session.get(), "order_seq");
		int new_order_id = seq.nextval();

		if (delivery_info_.get() && (type_delivery() || type_pickup() || type_table_booking())) {
			seq_t seq = seq_t(session.get(), "order_delivery_seq");
			query->create_statement
			(
			"insert into ksi.order_delivery \
			( \
				cafe_id \
			, delivery_id \
			, firstname \
			, lastname \
			, state_id \
			, zip \
			, streetaddress \
			, phone_home \
			, delivery_price \
			, delivery_date \
			, city \
			) \
			values \
			( \
				case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
			, :delivery_id \
			, :firstname \
			, :lastname \
			, case when :state_id = 0 then null else :state_id end \
			, :zip \
			, :streetaddress \
			, :phone \
			, :price \
			, :delivery_date \
			, :city \
			)"
			);

			query->set("cafe_id", cafe_.cafe_id());
			query->set("delivery_id", seq.nextval());
			query->set("firstname", delivery_info_->firstname());
			query->set("lastname", delivery_info_->lastname());
			query->set("state_id", delivery_info_->state_id());
			query->set("zip", delivery_info_->zip());
			query->set("streetaddress", delivery_info_->streetaddress());
			query->set("phone", delivery_info_->phone());
			query->set("price", delivery_fee());
			query->set("delivery_date", delivery_info_->delivery_time());
			query->set("city", delivery_info_->city());

			query->execute_statement();

			delivery_id_ = seq.lastval();
		}

		query->create_statement
		(
	    "insert into ksi.orders \
	    ( \
			cafe_id \
			, cafe_id2 \
			, order_id \
			, order_id2 \
			, created \
			, creator_id \
			, status_id \
			, type_id \
			, subtotal \
			, sales_tax \
			, memo \
			, delivery_id \
			, pos_version \
			, table_group_id \
			, seance_id \
			, meal_person_id \
			, payment_type_id \
			, guest_count \
			, flags \
			, token \
			, tables \
			, gratuity \
	    ) \
	    values \
	    ( \
			case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
			, case when :cafe_id2 = 0 or :cafe_id = :cafe_id2 then null else :cafe_id2 end \
			, :order_id \
			, case when :cafe_id2 = 0 or :cafe_id = :cafe_id2 then null else :order_id end \
			, :created \
			, ksi.util.get_cur_user_id \
			, 1 \
			, :type_id \
			, :subtotal \
			, :sales_tax \
			, :memo \
			, case when :delivery_id = 0 then null else :delivery_id end \
			, :pos_version \
			, case when :table_group_id = 0 and :cafe_id2 != 0 \
			  then (select table_group_id from ksi.table_group where cafe_id = :cafe_id2 and deleted is null and is_default = 1) \
				else :table_group_id \
				end \
			, case when :cafe_id2 != 0 \
				then null \
				else ksi.util.get_cur_seance_id  \
				end \
			, case when :meal_person_id = 0 then null else :meal_person_id end \
			, case when :payment_type_id = 0 then null else :payment_type_id end \
			, case when :guest_count = 0 then null else :guest_count end \
			, :flags \
			, case when :token = '' then null else :token end \
			, case when :tables = '' then null else :tables end \
			, :gratuity \
	    )"
		);

		query->set("cafe_id2", cafe_id2);
		query->set("cafe_id", cafe_.cafe_id());
		query->set("order_id", seq.lastval());
		if (order_created_ == boost::posix_time::not_a_date_time)
			sync_order_created();
		query->set("created", order_created_);
		query->set("type_id", type_id_);
		query->set("subtotal", subtotal());
		query->set("sales_tax", tax());
		query->set("gratuity", gratuity());
		query->set("memo", memo_);
		if (type_delivery() || type_pickup() || type_table_booking()) query->set("delivery_id", delivery_id_);
		else query->set("delivery_id", 0);
		query->set("pos_version", pos_version);
		query->set("table_group_id", table_.table_group_id());
		query->set("meal_person_id", employee_id());
		query->set("payment_type_id", payment_type_id_);
		query->set("guest_count", guest_count_);
		query->set("flags", flags_);
		query->set("token", token_.name());
		query->set("tables", table_.name());
		query->execute_statement();
		
		order_id_ = new_order_id;
	}
	else if (order_id() != 0) {
		//int clean_delivery_id = 0;

		if (!delivery_id_ && delivery_info_.get() && (type_delivery() || type_pickup() || type_table_booking())) {
			seq_t seq = seq_t(session.get(), "order_delivery_seq");
			query->create_statement
			(
			"insert into ksi.order_delivery \
			( \
				cafe_id \
			, delivery_id \
			, firstname \
			, lastname \
			, state_id \
			, zip \
			, streetaddress \
			, phone_home \
			, delivery_price \
			, delivery_date \
			, city \
			) \
			values \
			( \
				case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
			, :delivery_id \
			, :firstname \
			, :lastname \
			, case when :state_id = 0 then null else :state_id end \
			, :zip \
			, :streetaddress \
			, :phone \
			, :price \
			, :delivery_date \
			, :city \
			)"
			);

			query->set("cafe_id", cafe_.cafe_id());
			query->set("delivery_id", seq.nextval());
			query->set("firstname", delivery_info_->firstname());
			query->set("lastname", delivery_info_->lastname());
			query->set("state_id", delivery_info_->state_id());
			query->set("zip", delivery_info_->zip());
			query->set("streetaddress", delivery_info_->streetaddress());
			query->set("phone", delivery_info_->phone());
			query->set("price", delivery_fee());
			query->set("delivery_date", delivery_info_->delivery_time());
			query->set("city", delivery_info_->city());

			query->execute_statement();

			delivery_id_ = seq.lastval();
		}
		else if (delivery_id_ && delivery_info_.get() && (type_delivery() || type_pickup() || type_table_booking())) {
			query->create_statement(
			"update ksi.order_delivery set "
			" firstname = :firstname, "
			" lastname = :lastname, "
			" state_id = case when :state_id = 0 then null else :state_id end, "
			" zip = :zip, "
			" streetaddress = :streetaddress, "
			" phone_home = :phone, "
			" delivery_price = :price, "
			" delivery_date = :delivery_date, "
			" city = :city "
			" where cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end and delivery_id = :delivery_id"
			);

			query->set("cafe_id", cafe_.cafe_id());
			query->set("delivery_id", delivery_id_);
			query->set("firstname", delivery_info_->firstname());
			query->set("lastname", delivery_info_->lastname());
			query->set("state_id", delivery_info_->state_id());
			query->set("zip", delivery_info_->zip());
			query->set("streetaddress", delivery_info_->streetaddress());
			query->set("phone", delivery_info_->phone());
			query->set("price", delivery_fee());
			query->set("delivery_date", delivery_info_->delivery_time());
			query->set("city", delivery_info_->city());

			query->execute_statement();
		}

		query->create_statement
		(
		"update ksi.orders set \
		type_id = :type_id \
		, payment_type_id = case when :payment_type_id = 0 then null else :payment_type_id end \
		, subtotal = :subtotal \
		, sales_tax = :sales_tax \
		, token = case when :token = '' then null else :token end \
		, seance_id = ksi.util.get_cur_seance_id \
		, meal_person_id = case when :meal_person_id = 0 then null else :meal_person_id end \
		, memo = case when :memo = '' then null else :memo end \
		, table_group_id = :table_group_id \
		, tables = case when :tables = '' then null else :tables end \
		, pos_version = :pos_version \
		, guest_count = :guest_count \
		, delivery_id = case when :delivery_id = 0 then null else :delivery_id end \
		, flags = :flags \
		, gratuity = :gratuity\
		, creator_id = case when :creator_id=0 then ksi.util.get_cur_user_id else :creator_id end\
		where cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
		and order_id = :order_id"
		);

		query->set("type_id", type_id_);
		query->set("payment_type_id", payment_type_id_);
		query->set("subtotal", subtotal());
		query->set("sales_tax", tax());
		query->set("gratuity", gratuity());
		query->set("token", token_.name());
		query->set("meal_person_id", employee_id());
		query->set("memo", memo_);
		query->set("table_group_id", table_.table_group_id());
		query->set("tables", table_.name());
		query->set("pos_version", pos_version);
		query->set("guest_count", guest_count_);
		query->set("delivery_id", delivery_id_);
		if (delivery_info_.get()) query->set("delivery_id", delivery_id_);
		else query->set("delivery_id", 0);
		//if (type_delivery() || type_pickup() || type_table_booking()) query->set("delivery_id", delivery_id_);
		//else query->set("delivery_id", 0);
		query->set("flags", flags_);
		query->set("order_id", order_id());
		query->set("cafe_id", cafe_.cafe_id());
		query->set("creator_id", creator_id_);
		query->execute_statement();

/*
		if (clean_delivery_id) {
			query->create_statement
			(
			"delete from ksi.order_delivery \
				where cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
					and delivery_id = :delivery_id"
			);
			query->set("cafe_id", cafe_.cafe_id());
			query->set("delivery_id", clean_delivery_id);
			query->execute_statement();
		}
*/
	}


	try {
		sync_order(false);
	}
	catch ( ... ) {
		order_id_ = save_order_id;
		throw;
	}

	set_status_accepted();
}

void order_t::set_close(bool logical_trans)
{
	/*if (order_id() == 0) set_accept(cafe_id2, logical_trans);*/
	payment_->payment(order_id(), logical_trans);

	std::auto_ptr<logical_transaction_t> log_trans;

	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 93));

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"update ksi.orders set \
		status_id = 3 \
		, paid = sysdate \
		, closed = sysdate \
		, closer_id = ksi.util.get_cur_user_id \
		where cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
			and order_id = :order_id\
		RETURNING closed INTO :close_date_time"
	);
	query->set("cafe_id", cafe_.cafe_id());
	query->set("order_id", order_id());
	boost::shared_ptr<boost::posix_time::ptime> close_date_time_ptr = boost::shared_ptr<boost::posix_time::ptime>(new boost::posix_time::ptime());
	query->set("close_date_time", close_date_time_ptr);
	query->execute_statement();
	order_closed_ = *close_date_time_ptr;
	set_status_closed();
}

void order_t::set_simply_close(bool logical_trans)
{
	/*if (order_id() == 0) set_accept(cafe_id2, logical_trans);*/
	payment_->payment(order_id(), logical_trans);

	std::auto_ptr<logical_transaction_t> log_trans;

	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 93));

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"update ksi.orders set \
		closed = sysdate \
		, status_id = 3 \
		where cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
		and order_id = :order_id \
		RETURNING closed INTO :close_date_time"
	);

	query->set("cafe_id", cafe_.cafe_id());
	query->set("order_id", order_id());
	boost::shared_ptr<boost::posix_time::ptime> close_date_time_ptr; close_date_time_ptr.reset(new boost::posix_time::ptime());
	query->set("close_date_time", close_date_time_ptr);
	query->execute_statement();
	order_closed_ = *close_date_time_ptr;
	set_status_closed();
}

void order_t::set_void(bool logical_trans)
{
	if (order_id() == 0) return;
	std::auto_ptr<logical_transaction_t> log_trans;
	if (logical_trans) log_trans.reset(new logical_transaction_t(session.get(), 105));

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(
  "update ksi.orders set status_id = 5 \
	where cafe_id = case when :cafe_id = 0 then ksi.util.get_cur_cafe_id else :cafe_id end \
		and order_id = :order_id"
	);

	query->execute_statement();

	this->set_status_void();
}

void order_t::uncompress() {
	std::vector<order_dish_t> tmp;
	for (size_t i=0; i<order_dish_.size(); ++i) {
		for (int j=0; j<order_dish_[i].count(); ++j) {
			order_dish_t dish(order_dish_[i]);
			dish.clear_count();
			dish.count_inc();
			tmp.push_back(dish);
		}
		for (int j=0; j<order_dish_[i].new_count(); ++j) {
			order_dish_t dish(order_dish_[i]);
			dish.clear_count();
			dish.new_count_inc();
			tmp.push_back(dish);
		}
		if (order_dish_[i].total_count() == 0)
			tmp.push_back(order_dish_[i]);
	}
	order_dish_ = tmp;	
}

void order_t::compress() {
  std::vector<order_dish_t> tmp = order_dish_;
  order_dish_.clear();
  for (size_t i=0; i<tmp.size(); ++i) {
    add_dish(tmp[i], false);
  }
}


};
