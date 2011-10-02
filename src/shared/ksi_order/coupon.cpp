
#include "coupon.hpp"
#include <boost/bind.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include <ksi_include/algo_functor.hpp>
#include "order_interface.hpp"
#include <map>
#include <boost/function.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <shared_modules/connect.hpp>


namespace ksi_cafe {

coupon_t::coupon_t(int coupon_id_)
	: coupon_id_(coupon_id_)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select title, start_date, stop_date, auth_required, condition_rule,  \
	application_rule, discount, description, nvl(autoscalable,0) as autoscalable, \
	nvl(fixed_discount, 0) as fixed_discount, apply2condition, combinable, \
	subtotal_condition \
	from ksi.coupon \
	where coupon_id = :coupon_id"
	);
	query->set("coupon_id", coupon_id_);
	query->execute_statement();

	title_ = boost::get<std::string>(query->get(0));

	start_date_ = boost::get<boost::posix_time::ptime>(query->get(1));
	stop_date_ = boost::get<boost::posix_time::ptime>(query->get(2));
	auth_req_ = static_cast<int>(boost::get<double>(query->get(3)));
	condition_rule_ = static_cast<int>(boost::get<double>(query->get(4)));
	application_rule_ = static_cast<int>(boost::get<double>(query->get(5)));
	discount_ = boost::get<double>(query->get(6));
	description_ = boost::get<std::string>(query->get(7));
	auto_scalable_ = static_cast<int>(boost::get<double>(query->get(8)));
	fixed_discount_ = boost::get<double>(query->get(9));
	apply2condition_ = static_cast<int>(boost::get<double>(query->get(10)));
	combinable_ = static_cast<int>(boost::get<double>(query->get(11)));
	subtotal_condition_ = boost::get<double>(query->get(12));

	query->create_statement
	(
	"select dish_id, quantity as qty \
	from ksi.coupon_condition \
	where coupon_id = :coupon_id and quantity != 0"
	);
	query->set("coupon_id", coupon_id_);
	query->execute_statement();

	while (!query->next()) {
		int dish_id = static_cast<int>(boost::get<double>(query->get(0)));
		int qty = static_cast<int>(boost::get<double>(query->get(1)));
		condition_.push_back(coupon_condition_t(coupon_id_, dish_id, qty));
	}

	if (!fixed_discount()) {
		query->create_statement
		(
		"select dish_id, quantity qty, price \
		from ksi.coupon_application \
		where coupon_id = :coupon_id"
		);
		query->set("coupon_id", coupon_id_);
		query->execute_statement();

		while (!query->next()) {
			int dish_id = static_cast<int>(boost::get<double>(query->get(0)));
			int qty = static_cast<int>(boost::get<double>(query->get(1)));
			double price = boost::get<double>(query->get(2));
			application_.push_back(coupon_application_t(coupon_id_, dish_id, qty, price));
		}
	}
}


bool exist_condition_for_dish(const coupon_t& coupon, int dish_id)
{
	std::vector<coupon_condition_t>::const_iterator It;
	It = std::find_if
	(
			coupon.condition_.begin()
		, coupon.condition_.end()
		, boost::bind(std::equal_to<int>(), boost::bind(&coupon_condition_t::dish_id, _1), dish_id)
	);

	if (It == coupon.condition_.end()) return false;

	return true;
}


bool exist_application_for_dish(const coupon_t& coupon, int dish_id)
{
	std::vector<coupon_application_t>::const_iterator It;
	It = std::find_if
	(
			coupon.application_.begin()
		, coupon.application_.end()
		, boost::bind(std::equal_to<int>(), boost::bind(&coupon_application_t::dish_id, _1), dish_id)
	);

	if (It == coupon.application_.end()) return false;

	return true;
}

int condition_qty_for_dish(const coupon_t& coupon, int dish_id)
{
	std::vector<coupon_condition_t>::const_iterator It;
	It = std::find_if
	(
			coupon.condition_.begin()
		, coupon.condition_.end()
		, boost::bind(std::equal_to<int>(), boost::bind(&coupon_condition_t::dish_id, _1), dish_id)
	);

	if (It == coupon.condition_.end()) return 0;

	return It->qty();
}

int application_qty_for_dish(const coupon_t& coupon, int dish_id)
{
	std::vector<coupon_application_t>::const_iterator It;
	It = std::find_if
	(
			coupon.application_.begin()
		, coupon.application_.end()
		, boost::bind(std::equal_to<int>(), boost::bind(&coupon_application_t::dish_id, _1), dish_id)
	);

	if (It == coupon.application_.end()) return 0;

	return It->qty();
}
/*
int free_dish_count(const std::vector<order_dish_t*>& dish_vec)
{
	int qty = accumulate_proc_if
		        (
								dish_vec.begin()
							, dish_vec.end()
							, 0
							,
								boost::bind
								(
										std::plus<int>()
									, boost::bind(&order_dish_t::count_without_coupon, _1)
									, 0
								)
							,
								boost::bind
								(
										std::equal_to<int>()
									, boost::bind(&order_dish_t::coupon_id, _1)
									, 0
								)
						);
	return qty;
}

int free_dish_count(const std::vector<order_dish_t*>& dish_vec, int dish_id)
{
	int qty = accumulate_proc_if
						(
								dish_vec.begin()
							, dish_vec.end()
							, 0
							,
								boost::bind
								(
										std::plus<int>()
									, boost::bind(&order_dish_t::count_without_coupon, _1)
									, 0
								)
							,
								boost::bind
								(
										std::logical_and<bool>()
										, boost::bind(std::equal_to<int>(), boost::bind(&order_dish_t::coupon_id, _1), 0)
										, boost::bind(std::equal_to<int>(), boost::bind(&order_dish_t::dish_id, _1), dish_id)
								)
						);
	return qty;
}
*/

double subtotal(const std::vector<order_dish_t*>& dish_vec)
{
	double sub = accumulate_proc
							 (
									 dish_vec.begin()
								 , dish_vec.end()
								 , 0.0
								 ,
									 boost::bind
								   (
											 std::plus<double>()
										 , boost::bind(&order_dish_t::amount, _1)
										 , 0
									 )
							 );
	return sub;
}



coupon_storage_t::coupon_storage_t() {
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select coupon_id, coupon_cafe.cafe_id \
	from ksi.coupon_cafe \
	left join ksi.cafe on cafe.cafe_id = coupon_cafe.cafe_id \
	where cafe.deleted is null \
	order by coupon_id"
	);

	query->execute_statement();

	std::multimap<int, int> tmp_map;// cafe_id -> coupon_id
	while (!query->next()) {
		int coupon_id = static_cast<int>(boost::get<double>(query->get(0)));
		int cafe_id = static_cast<int>(boost::get<double>(query->get(1)));

		std::vector<coupon_t>::iterator It =
			std::find_if
				(
					coupon_.begin()
				, coupon_.end()
				, boost::bind
					(
							std::equal_to<int>()
						,	coupon_id
						, boost::bind(&coupon_t::coupon_id, _1)
					)
				);

		if (It == coupon_.end()) {
			coupon_.push_back(coupon_t(coupon_id));
		}

		tmp_map.insert(std::pair<int, int>(cafe_id, coupon_id));
	}

	std::multimap<int, int>::iterator end = tmp_map.end();
	std::vector<coupon_t>::iterator It;
	std::vector<coupon_t>::iterator coupon_end = coupon_.end();

	for (std::multimap<int, int>::iterator i=tmp_map.begin(); i!=end; ++i) {
		It = std::find_if
			(
					coupon_.begin()
				, coupon_.end()
				, boost::bind
					(
							std::equal_to<int>()
						,	i->second
						, boost::bind(&coupon_t::coupon_id, _1)
					)
			);

		if (It == coupon_end) THROW_LOCATION(10138, "coupon_storage_t::coupon_storage_t()");
		cafe_coupon_map_.insert(std::pair<int, coupon_t*>(i->first, &(*It)));
	}
}

std::vector<coupon_t*> coupon_storage_t::coupon_for_cafe(int cafe_id) const {
	std::vector<coupon_t*> coupon_vec;
	//coupon_vec.reserve(cafe_coupon_map_.count(cafe_id));
	std::multimap<int, coupon_t*>::const_iterator lb = cafe_coupon_map_.lower_bound(cafe_id);
	std::multimap<int, coupon_t*>::const_iterator ub = cafe_coupon_map_.upper_bound(cafe_id);

	std::transform(lb,
                 ub,
                 std::back_inserter(coupon_vec),
                 select2nd<std::pair<int, coupon_t*> >());
	return coupon_vec;
}

coupon_t& find_coupon(int coupon_id, const std::vector<coupon_t*>& s) {
	std::vector<coupon_t*>::const_iterator It =
	std::find_if
		(
				s.begin()
			,	s.end()
			,	boost::bind
				(
						std::equal_to<int>()
					,	coupon_id
					, boost::bind(&coupon_t::coupon_id, _1)
				)
		);

	if (It == s.end()) THROW_LOCATION(10136, "find_coupon("+boost::lexical_cast<std::string>(coupon_id)+", const std::vector<coupon_t*>&)");

	return **It;
}

const std::vector<coupon_t*> coupon_in_cafe_t::selected() const {
	std::vector<coupon_t*> selected;
	std::transform(selected_.begin(),
                 selected_.end(),
                 std::back_inserter(selected),
                 select1st<std::pair<coupon_t*, int> >());
	return selected;
}

bool coupon_in_cafe_t::in_selected(coupon_t* coupon_ptr) const {
	std::map<coupon_t*, int>::const_iterator map_end = selected_.end();
	return selected_.find(coupon_ptr) != map_end;
}

int coupon_in_cafe_t::not_combinable_exist() const {
	int res = 0;
	std::vector<coupon_t*>::const_iterator end = coupon_.end();

	for (std::vector<coupon_t*>::const_iterator i=coupon_.begin(); i!=end; ++i) {
		if ((*i)->combinable() && in_selected(*i))
			res = (*i)->coupon_id();
	}

	return res;
/*

  int not_combinable_exist = 0;
  for (int i=0; i<cafe_storage->coupon_size(); i++)
  {
    const coupon_t& cur_coupon = cafe_storage->get_coupon(i);
    if (!cur_coupon.combinable() && cafe_storage->in_selected(cur_coupon.coupon_id()))
    {
      not_combinable_exist = cur_coupon.coupon_id();
    }
  }
*/
}

const std::vector<coupon_t*> coupon_in_cafe_t::coupon() const {
	std::vector<coupon_t*> coupon_vec;
	copy_if(coupon_.begin(),
               coupon_.end(),
               std::back_inserter(coupon_vec),
               coupon_ptr_valid_pred());
	return coupon_vec;
}

void coupon_in_cafe_t::select(int coupon_id, int count, int mng_id) {
	coupon_t& coupon = find_coupon(coupon_id, coupon_);
	selected_[&coupon] = count;
	manager_[&coupon] = mng_id;
}

void coupon_in_cafe_t::unselect(coupon_t* coupon_ptr) {
	selected_.erase(coupon_ptr);
	manager_.erase(coupon_ptr);
}
void coupon_in_cafe_t::unselect(int coupon_id)
{
	selected_.erase(&find_coupon(coupon_id, coupon_));
	manager_.erase(&find_coupon(coupon_id, coupon_));
}

bool XXX_apply_coupon_XXX(const coupon_t& coupon, const std::vector<order_dish_t*>& dish_vec, bool dummy, int coupon_mng_id) {
	if (subtotal(dish_vec) < coupon.subtotal_condition()) return false;
	
	std::map<order_dish_t*, int> dish_application_checked_;
	std::map<order_dish_modifier_t*, int> modifier_application_checked_;
	std::map<order_dish_t*, double> new_dish_price_;
	std::map<order_dish_modifier_t*, double> new_modifier_price_;
	std::map<order_dish_t*, int> dish_condition_checked_;
	std::map<order_dish_modifier_t*, int> modifier_condition_checked_;	
	
	std::map<order_dish_t*, const coupon_application_t*> dish_application_;
	std::map<order_dish_modifier_t*, const coupon_application_t*> modifier_application_;
	std::map<const coupon_application_t*, order_dish_t*> application_dish_;
	std::map<const coupon_application_t*, order_dish_modifier_t*> application_modifier_;
	
	std::map<order_dish_t*, const coupon_condition_t*> dish_condition_;
	std::map<order_dish_modifier_t*, const coupon_condition_t*> modifier_condition_;
	std::map<const coupon_condition_t*, order_dish_t*> condition_dish_;
	std::map<const coupon_condition_t*, order_dish_modifier_t*> condition_modifier_;
	
	typedef std::map<const coupon_application_t*, order_dish_t*>::iterator AppDishIter;
	typedef std::map<const coupon_application_t*, order_dish_modifier_t*>::iterator AppModIter;
	typedef std::map<order_dish_t*, const coupon_application_t*>::iterator DishAppIter;
	typedef std::map<order_dish_modifier_t*, const coupon_application_t*>::iterator ModAppIter;
	
	typedef std::map<const coupon_condition_t*, order_dish_t*>::iterator ConDishIter;
	typedef std::map<const coupon_condition_t*, order_dish_modifier_t*>::iterator ConModIter;
	typedef std::map<order_dish_t*, const coupon_condition_t*>::iterator DishConIter;
	typedef std::map<order_dish_modifier_t*, const coupon_condition_t*>::iterator ModConIter;
	
	typedef std::map<order_dish_t*, int>::iterator DishIter;
	typedef std::map<order_dish_modifier_t*, int>::iterator ModIter;
	
	if (coupon.fixed_discount()) {
		if (coupon.condition_rule()) {
			bool apply_all_condition = true;
			for (size_t ii=0; ii<coupon.condition_.size(); ++ii) {
				int dish_id = coupon.condition_[ii].dish_id();
				int erase_qty = coupon.condition_[ii].qty();
				bool all = erase_qty == 0 ? true : false;
				bool apply_condition = false;
				for (size_t i=0; i<dish_vec.size(); ++i) {
					if (dish_vec[i]->total_count() == 0) continue;
					for (size_t j=0; j<dish_vec[i]->modifier_vec().size(); ++j) {
						order_dish_modifier_t& modifier = dish_vec[i]->modifier_vec()[j];
						if (modifier.count() == 0) continue;
						if (modifier.coupon_id() == 0 && modifier.mod_dish_id() == dish_id) {
							if (!modifier_condition_checked_[&modifier] && (all || modifier.count() <= erase_qty)) {
								if (!all) erase_qty -= modifier.count();
								modifier_condition_checked_[&modifier] = 1;
								new_modifier_price_[&modifier] = modifier.price();
								apply_condition = true;
							}
						}
					}
				}
				for (size_t i=0; i<dish_vec.size(); ++i) {
					if (dish_vec[i]->total_count() == 0) continue;
					if (dish_vec[i]->coupon_id() == 0 && dish_vec[i]->dish_id() == dish_id) {
						if (!dish_condition_checked_[dish_vec[i]] && (all || dish_vec[i]->total_count() <= erase_qty)) {
							if (!all) erase_qty -= dish_vec[i]->total_count();
							dish_condition_checked_[dish_vec[i]] = 1;
							new_dish_price_[dish_vec[i]] = dish_vec[i]->price();
							apply_condition = true;
						}
					}
				}
				if (!apply_condition) {
					apply_all_condition = false;
					break;
				}
			}
			if (apply_all_condition) {
				/* Применить */
				if (!dummy) {
					for (DishIter i=dish_condition_checked_.begin(); i!=dish_condition_checked_.end(); ++i) {
						if (i->second == 1) {
							i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
							i->first->set_price(new_dish_price_[i->first]);
						}
					}
					for (ModIter i=modifier_condition_checked_.begin(); i!=modifier_condition_checked_.end(); ++i) {
						if (i->second == 1) {
							i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
							i->first->set_price(new_modifier_price_[i->first]);
						}
					}
				}
				return true;
			}
			else {
				return false;
			}

		}
		else if (!coupon.condition_rule()) {
			for (size_t ii=0; ii<coupon.condition_.size(); ++ii) {
				int dish_id = coupon.condition_[ii].dish_id();
				int erase_qty = coupon.condition_[ii].qty();
				bool all = erase_qty == 0 ? true : false;
				for (size_t i=0; i<dish_vec.size(); ++i) {
					if (dish_vec[i]->total_count() == 0) continue;
					for (size_t j=0; j<dish_vec[i]->modifier_vec().size(); ++j) {
						order_dish_modifier_t& modifier = dish_vec[i]->modifier_vec()[j];
						if (modifier.count() == 0) continue;
						if (modifier.coupon_id() == 0 && modifier.mod_dish_id() == dish_id) {
							if (!modifier_condition_checked_[&modifier] && (all || modifier.count() <= erase_qty)) {
								if (!all) erase_qty -= modifier.count();
								modifier_condition_checked_[&modifier] = 1;
								new_modifier_price_[&modifier] = modifier.price();
								/* Применить */
								if (!dummy) {
									for (DishIter i=dish_condition_checked_.begin(); i!=dish_condition_checked_.end(); ++i) {
										if (i->second == 1) {
											i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
											i->first->set_price(new_dish_price_[i->first]);
										}
									}
									for (ModIter i=modifier_condition_checked_.begin(); i!=modifier_condition_checked_.end(); ++i) {
										if (i->second == 1) {
											i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
											i->first->set_price(new_modifier_price_[i->first]);
										}
									}
								}								
								return true;
							}
						}
					}
					for (size_t i=0; i<dish_vec.size(); ++i) {
						if (dish_vec[i]->total_count() == 0) continue;
						if (dish_vec[i]->coupon_id() == 0 && dish_vec[i]->dish_id() == dish_id) {
							if (!dish_condition_checked_[dish_vec[i]] && (all || dish_vec[i]->total_count() <= erase_qty)) {
								if (!all) erase_qty -= dish_vec[i]->total_count();
								dish_condition_checked_[dish_vec[i]] = 1;
								new_dish_price_[dish_vec[i]] = dish_vec[i]->price();
								/* Применить */
								if (!dummy) {
									for (DishIter i=dish_condition_checked_.begin(); i!=dish_condition_checked_.end(); ++i) {
										if (i->second == 1) {
											i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
											i->first->set_price(new_dish_price_[i->first]);
										}
									}
									for (ModIter i=modifier_condition_checked_.begin(); i!=modifier_condition_checked_.end(); ++i) {
										if (i->second == 1) {
											i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
											i->first->set_price(new_modifier_price_[i->first]);
										}
									}
								}
								return true;
							}
						}
					}
				}
			}
		}
		else if (!coupon.condition_.size()) {
			/* Применить */
			if (!dummy) {
				for (DishIter i=dish_condition_checked_.begin(); i!=dish_condition_checked_.end(); ++i) {
					if (i->second == 1) {
						i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
						i->first->set_price(new_dish_price_[i->first]);
					}
				}
				for (ModIter i=modifier_condition_checked_.begin(); i!=modifier_condition_checked_.end(); ++i) {
					if (i->second == 1) {
						i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
						i->first->set_price(new_modifier_price_[i->first]);
					}
				}
			}
			return true;
		}
		return false;
	}
	
	bool apply_all_condition = true;
	int coupon_id = coupon.coupon_id();
	if (coupon.condition_rule() && coupon.condition_.size()) {
		for (size_t ii=0; ii<coupon.condition_.size(); ++ii) {
			int dish_id = coupon.condition_[ii].dish_id();
			int erase_qty = coupon.condition_[ii].qty();
			bool all = erase_qty == 0 ? true : false;
			bool apply_condition = false;
			for (size_t i=0; i<dish_vec.size(); ++i) {
				if (dish_vec[i]->total_count() == 0) continue;
				for (size_t j=0; j<dish_vec[i]->modifier_vec().size(); ++j) {
					order_dish_modifier_t& modifier = dish_vec[i]->modifier_vec()[j];
					if (modifier.count() == 0) continue;
					if (modifier.coupon_id() == 0 && modifier.mod_dish_id() == dish_id) {
						if (!modifier_condition_checked_[&modifier] && (all || modifier.count() <= erase_qty)) {
							if (!all) erase_qty -= modifier.count();
							modifier_condition_checked_[&modifier] = 1;
							/** new **/
							modifier_condition_[&modifier] = &coupon.condition_[ii];
							condition_modifier_[&coupon.condition_[ii]] = &modifier;
							/** new **/
							if (coupon.apply2condition()) {
								if (coupon.discount()) new_modifier_price_[&modifier] = modifier.price()*(1-coupon.discount());
								else new_modifier_price_[&modifier] = -1;
							}	
							else {
								if (coupon.discount()) new_modifier_price_[&modifier] = modifier.price();
								else new_modifier_price_[&modifier] = -1;
							}
							apply_condition = true;
						}
					}
				}
			}
			for (size_t i=0; i<dish_vec.size(); ++i) {
				if (dish_vec[i]->total_count() == 0) continue;
				if (dish_vec[i]->coupon_id() == 0 && dish_vec[i]->dish_id() == dish_id) {
					if (!dish_condition_checked_[dish_vec[i]] && (all || dish_vec[i]->total_count() <= erase_qty)) {
						if (!all) erase_qty -= dish_vec[i]->total_count();
						dish_condition_checked_[dish_vec[i]] = 1;
						/** new **/
						dish_condition_[dish_vec[i]] = &coupon.condition_[ii];
						condition_dish_[&coupon.condition_[ii]] = dish_vec[i];
						/** new **/
						if (coupon.apply2condition()) {
							if (coupon.discount()) new_dish_price_[dish_vec[i]] = dish_vec[i]->price()*(1-coupon.discount());
							else new_dish_price_[dish_vec[i]] = -1;
						}
						else {
							if (coupon.discount()) new_dish_price_[dish_vec[i]] = dish_vec[i]->price();
							else new_dish_price_[dish_vec[i]] = -1;
						}
						apply_condition = true;
					}
				}
			}
			if (!apply_condition || erase_qty != 0) {
				apply_all_condition = false;
				break;
			}
		}
		if (!apply_all_condition) {
			return false;
		}			
	}
	else if (!coupon.condition_rule() && coupon.condition_.size()) {
		bool apply_condition = false;
		for (size_t ii=0; ii<coupon.condition_.size(); ++ii) {
			int dish_id = coupon.condition_[ii].dish_id();
			int erase_qty = coupon.condition_[ii].qty();
			bool all = erase_qty == 0 ? true : false;
			for (size_t i=0; i<dish_vec.size(); ++i) {
				if (dish_vec[i]->total_count() == 0) continue;
				for (size_t j=0; j<dish_vec[i]->modifier_vec().size(); ++j) {
					order_dish_modifier_t& modifier = dish_vec[i]->modifier_vec()[j];
					if (modifier.count() == 0) continue;
					if (modifier.coupon_id() == 0 && modifier.mod_dish_id() == dish_id) {
						if (!modifier_condition_checked_[&modifier] && (all || modifier.count() <= erase_qty)) {
							if (!all) erase_qty -= modifier.count();
							modifier_condition_checked_[&modifier] = 1;
							/** new **/
							modifier_condition_[&modifier] = &coupon.condition_[ii];
							condition_modifier_[&coupon.condition_[ii]] = &modifier;
							/** new **/
							if (coupon.apply2condition()) {
								if (coupon.discount()) new_modifier_price_[&modifier] = modifier.price()*(1-coupon.discount());
								else new_modifier_price_[&modifier] = -1;
							}
							else {
								if (coupon.discount()) new_modifier_price_[&modifier] = modifier.price();
								else new_modifier_price_[&modifier] = -1;
							}
							apply_condition = true;
						}
					}
				}
			}
			for (size_t i=0; i<dish_vec.size(); ++i) {
				if (dish_vec[i]->total_count() == 0) continue;
				if (dish_vec[i]->coupon_id() == 0 && dish_vec[i]->dish_id() == dish_id) {
					if (!dish_condition_checked_[dish_vec[i]] && (all || dish_vec[i]->total_count() <= erase_qty)) {
						if (!all) erase_qty -= dish_vec[i]->total_count();
						dish_condition_checked_[dish_vec[i]] = 1;
						/** new **/
						dish_condition_[dish_vec[i]] = &coupon.condition_[ii];
						condition_dish_[&coupon.condition_[ii]] = dish_vec[i];
						/** new **/
						if (coupon.apply2condition()) {
							if (coupon.discount()) new_dish_price_[dish_vec[i]] = dish_vec[i]->price()*(1-coupon.discount());
							else new_dish_price_[dish_vec[i]] = -1;
						}
						else {
							if (coupon.discount()) new_dish_price_[dish_vec[i]] = dish_vec[i]->price();
							else new_dish_price_[dish_vec[i]] = -1;
						}
						apply_condition = true;
					}
				}
			}
			if (apply_condition && erase_qty == 0) {
				break;
			}
			else {
				apply_condition = false;
			}
		}
		if (!apply_condition) {
			return false;
		}
	}
	
	if (coupon.application_.size()) {
		bool apply_any_application = false;
		for (size_t ii=0; ii<coupon.application_.size(); ++ii) {
			int dish_id = coupon.application_[ii].dish_id();
			int erase_qty = coupon.application_[ii].qty();
			bool all = erase_qty == 0 ? true : false;
			for (size_t i=0; i<dish_vec.size(); ++i) {
				if (dish_vec[i]->total_count() == 0) continue;
				for (size_t j=0; j<dish_vec[i]->modifier_vec().size(); ++j) {
					order_dish_modifier_t& modifier = dish_vec[i]->modifier_vec()[j];
					if (modifier.count() == 0) continue;
					if (modifier.coupon_id() == 0 && modifier.mod_dish_id() == dish_id) {
						if (coupon.apply2condition()) {
							if (!modifier_application_checked_[&modifier] && (all || modifier.count() <= erase_qty)) {
								if (!all) erase_qty -= modifier.count();
								modifier_application_checked_[&modifier] = 1;
								if (coupon.discount()) new_modifier_price_[&modifier] = modifier.price()*(1-coupon.discount());
								else new_modifier_price_[&modifier] = coupon.application_[ii].price();
								application_modifier_[&coupon.application_[ii]] = &modifier;
								modifier_application_[&modifier] = &coupon.application_[ii];
								if (erase_qty == 0) {
									apply_any_application = true;
								}
							}
						}
						else if (!coupon.apply2condition()) {
							if (!modifier_condition_checked_[&modifier] && !modifier_application_checked_[&modifier] && (all || modifier.count() <= erase_qty)) {
								if (!all) erase_qty -= modifier.count();
								modifier_application_checked_[&modifier] = 1;
								if (coupon.discount()) new_modifier_price_[&modifier] = modifier.price()*(1-coupon.discount());
								else new_modifier_price_[&modifier] = coupon.application_[ii].price();
								application_modifier_[&coupon.application_[ii]] = &modifier;
								modifier_application_[&modifier] = &coupon.application_[ii];
								if (erase_qty == 0) {
									apply_any_application = true;
								}
							}								
						}
					}
				}
			}
			for (size_t i=0; i<dish_vec.size(); ++i) {
				if (dish_vec[i]->total_count() == 0) continue;
				if (dish_vec[i]->coupon_id() == 0 && dish_vec[i]->dish_id() == dish_id) {
					if (coupon.apply2condition()) {
						if (!dish_application_checked_[dish_vec[i]] && (all || dish_vec[i]->total_count() <= erase_qty)) {
							if (!all) erase_qty -= dish_vec[i]->total_count();
							dish_application_checked_[dish_vec[i]] = 1;
							if (coupon.discount()) new_dish_price_[dish_vec[i]] = dish_vec[i]->price()*(1-coupon.discount());
							else new_dish_price_[dish_vec[i]] = coupon.application_[ii].price();
							application_dish_[&coupon.application_[ii]] = dish_vec[i];
							dish_application_[dish_vec[i]] = &coupon.application_[ii];
							if (erase_qty == 0) {
								apply_any_application = true;
							}
						}
					}
					else if (!coupon.apply2condition()) {
						if (!dish_condition_checked_[dish_vec[i]] && !dish_application_checked_[dish_vec[i]] && (all || dish_vec[i]->total_count() <= erase_qty)) {
							if (!all) erase_qty -= dish_vec[i]->total_count();
							dish_application_checked_[dish_vec[i]] = 1;
							if (coupon.discount()) new_dish_price_[dish_vec[i]] = dish_vec[i]->price()*(1-coupon.discount());
							else new_dish_price_[dish_vec[i]] = coupon.application_[ii].price();
							application_dish_[&coupon.application_[ii]] = dish_vec[i];
							dish_application_[dish_vec[i]] = &coupon.application_[ii];
							if (erase_qty == 0) {
								apply_any_application = true;
							}
						}
					}
				}
			}
		}
		if (!apply_any_application) {
			return false;
		}
		if (!dummy && coupon.application_rule()) {
			for (ModIter i=modifier_application_checked_.begin(); i!=modifier_application_checked_.end(); ++i) {
				if (i->second == 1) {
					i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
					i->first->set_price(new_modifier_price_[i->first]);
				}
			}
			for (DishIter i=dish_application_checked_.begin(); i!=dish_application_checked_.end(); ++i) {
				if (i->second == 1) {
					i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
					i->first->set_price(new_dish_price_[i->first]);
				}
				if (coupon.discount()) {
					for (size_t j=0; j<i->first->modifier_vec().size(); ++j) {
						order_dish_modifier_t& modifier = i->first->modifier_vec()[j];
						if (modifier.coupon_id() == 0) {
							modifier.set_coupon_id(coupon.coupon_id(), coupon_mng_id);
							modifier.set_price(modifier.price()*(1-coupon.discount()));
						}
					}
				}
			}
		}
		else if (!dummy && !coupon.application_rule()) {
			bool applied = false;
			for (AppDishIter i=application_dish_.begin(); i!= application_dish_.end(); ++i) {
				if (applied) continue;
				for (DishAppIter j=dish_application_.begin(); j!=dish_application_.end(); ++j) {
					if (i->first == j->second) {
						j->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
						j->first->set_price(new_dish_price_[j->first]);
						applied = true;
					}
				}
				for (ModAppIter j=modifier_application_.begin(); j!=modifier_application_.end(); ++j) {
					if (i->first == j->second) {
						j->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
						j->first->set_price(new_modifier_price_[j->first]);
						applied = true;
					}
				}
			}
			for (AppModIter i=application_modifier_.begin(); i!=application_modifier_.end(); ++i) {
				if (applied) continue;
				for (DishAppIter j=dish_application_.begin(); j!=dish_application_.end(); ++j) {
					if (i->first == j->second) {
						j->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
						j->first->set_price(new_dish_price_[j->first]);
						applied = true;
					}
				}
				for (ModAppIter j=modifier_application_.begin(); j!=modifier_application_.end(); ++j) {
					if (i->first == j->second) {
						j->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
						j->first->set_price(new_modifier_price_[j->first]);
						applied = true;
					}
				}					
			}
			/*
			for (DishIter i=dish_application_checked_.begin(); i!=dish_application_checked_.end(); ++i) {
				if (applied) continue;
				if (i->second == 1) {						
					i->first->set_coupon_id(coupon_id(), title());
					i->first->set_price(new_dish_price_[i->first]);
				}
				if (applied && discount()) {
					for (int j=0; j<i->first->mod_count(); ++j) {
						order_dish_mod_t& modifier = i->first->modifier(j);
						if (modifier.coupon_id() == 0) {
							modifier.set_coupon_id(coupon_id(), title());
							modifier.set_price(modifier.price()*(1-discount()));
						}
					}
				}
			}
			if (!applied) {
				for (ModIter i=modifier_application_checked_.begin(); i!=modifier_application_checked_.end(); ++i) {
					if (i->second == 1) {
						if (discount()) {
							i->first->set_coupon_id(coupon_id(), title());
							i->first->set_price(new_modifier_price_[i->first]);
							applied = true;
						}
						else if (!discount() && !applied) {
							i->first->set_coupon_id(coupon_id(), title());
							i->first->set_price(new_modifier_price_[i->first]);
							applied = true;
						}
					}
				}
			}
			*/
		}
		if (!dummy) {
			if (coupon.apply2condition()) {
				for (ModIter i=modifier_application_checked_.begin(); i!=modifier_application_checked_.end(); ++i) {
					int dish_id = i->first->mod_dish_id();
					int count = i->first->count();
					for (ModIter j=modifier_condition_checked_.begin(); j!=modifier_condition_checked_.end(); ++j) {
						if (j->first->count() == 0) continue;
						if (j->second == 0) continue;
						if (j->first->mod_dish_id() == dish_id && j->first->count() <= count) {
							count -= j->first->count();
							modifier_condition_checked_[j->first] = 0;
						}
					}
					for (DishIter j=dish_condition_checked_.begin(); j!=dish_condition_checked_.end(); ++j) {
						if (j->first->total_count() == 0) continue;
						if (j->second == 0) continue;
						if (j->first->dish_id() == dish_id && j->first->total_count() <= count) {
							count -= j->first->total_count();
							dish_condition_checked_[j->first] = 0;
						}
					}
				}
				for (DishIter i=dish_application_checked_.begin(); i!=dish_application_checked_.end(); ++i) {
					int dish_id = i->first->dish_id();
					int count = i->first->total_count();
					for (ModIter j=modifier_condition_checked_.begin(); j!=modifier_condition_checked_.end(); ++j) {
						if (j->first->count() == 0) continue;
						if (j->second == 0) continue;
						if (j->first->mod_dish_id() == dish_id && j->first->count() <= count) {
							count -= j->first->count();
							modifier_condition_checked_[j->first] = 0;
						}
					}
					for (DishIter j=dish_condition_checked_.begin(); j!=dish_condition_checked_.end(); ++j) {
						if (j->first->total_count() == 0) continue;
						if (j->second == 0) continue;
						if (j->first->dish_id() == dish_id && j->first->total_count() <= count) {
							count -= j->first->total_count();
							dish_condition_checked_[j->first] = 0;
						}
					}
				}
			}
			for (DishIter i=dish_condition_checked_.begin(); i!=dish_condition_checked_.end(); ++i) {
				if (i->second == 0) continue;
				if (i->first->total_count() == 0) continue;
				if (i->first->coupon_id() == 0) {
					i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
				}
			}
			for (ModIter i=modifier_condition_checked_.begin(); i!=modifier_condition_checked_.end(); ++i) {
				if (i->second == 0) continue;
				if (i->first->count() == 0) continue;
				if (i->first->coupon_id() == 0) {
					i->first->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
				}
			}
		}
		return true;
	}
	else if (coupon.application_.size() == 0 && coupon.discount()) {
		bool applied = false;
		if (coupon.apply2condition()) {
			for (size_t i=0; i<dish_vec.size(); ++i) {
				if (dish_vec[i]->total_count() == 0) continue;
				if (dish_vec[i]->coupon_id() == 0) {
					if (!dummy) {
						dish_vec[i]->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
						dish_vec[i]->set_price(dish_vec[i]->price()*(1-coupon.discount()));
					}
					applied = true;						
				}
				for (size_t j=0; j<dish_vec[i]->modifier_vec().size(); ++j) {
					order_dish_modifier_t& modifier = dish_vec[i]->modifier_vec()[j];
					if (modifier.count() == 0) continue;
					if (modifier.coupon_id() == 0) {
						if (!dummy) {
							modifier.set_coupon_id(coupon.coupon_id(), coupon_mng_id);
							modifier.set_price(modifier.price()*(1-coupon.discount()));
						}
						applied = true;
					}
				}
			}
		}
		else if (!coupon.apply2condition()) {
			for (size_t i=0; i<dish_vec.size(); ++i) {
				if (dish_vec[i]->total_count() == 0) continue;
				if (dish_condition_checked_[dish_vec[i]] == 1) {
					for (size_t j=0; j<dish_vec[i]->modifier_vec().size(); ++j) {
						order_dish_modifier_t& modifier = dish_vec[i]->modifier_vec()[j];
						if (modifier.count() == 0) continue;
						if (modifier.coupon_id() == 0 && modifier_condition_checked_[&modifier] == 0) {
							if (!dummy) {
								modifier.set_coupon_id(coupon.coupon_id(), coupon_mng_id);
								modifier.set_price(modifier.price()*(1-coupon.discount()));
							}
							applied = true;
						}
					}
				}
				else if (dish_condition_checked_[dish_vec[i]] == 0) {
					if (!dummy && dish_vec[i]->coupon_id() == 0) {
						dish_vec[i]->set_coupon_id(coupon.coupon_id(), coupon_mng_id);
						dish_vec[i]->set_price(dish_vec[i]->price()*(1-coupon.discount()));
					}
					if (dish_vec[i]->coupon_id() == 0) applied = true;
					for (size_t j=0; j<dish_vec[i]->modifier_vec().size(); ++j) {
						order_dish_modifier_t& modifier = dish_vec[i]->modifier_vec()[j];
						if (modifier.count() == 0) continue;
						if (modifier.coupon_id() == 0 && modifier_condition_checked_[&modifier] == 0) {
							if (!dummy) {
								modifier.set_coupon_id(coupon.coupon_id(), coupon_mng_id);
								modifier.set_price(modifier.price()*(1-coupon.discount()));
							}
							applied = true;
						}
					}
				}
			}
		}
		return applied;
	}
	
	return false;
}


bool can_apply_coupon(const coupon_t& coupon, const std::vector<order_dish_t*>& dish_vec) {
	return XXX_apply_coupon_XXX(coupon, dish_vec, true, 0);
}

bool apply_coupon(const coupon_t& coupon, const std::vector<order_dish_t*>& dish_vec, int coupon_mng_id) {
	return XXX_apply_coupon_XXX(coupon, dish_vec, false, coupon_mng_id);
}

};
