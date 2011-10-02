#ifndef COUPON_H_DERGACHEV_15_01_2008
#define COUPON_H_DERGACHEV_15_01_2008

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <map>
#include <ksi_include/ksi_exceptions.hpp>

namespace ksi_cafe {


class coupon_condition_t
{
public:
  coupon_condition_t(int coupon_id_, int dish_id_, int qty_)
		: coupon_id_(coupon_id_), dish_id_(dish_id_), qty_(qty_) {}

public:
  int coupon_id() const { return coupon_id_; }
  int dish_id() const { return dish_id_; }
  int qty() const { return qty_; }
private:
  int coupon_id_;
  int dish_id_;
  int qty_;
private:
  coupon_condition_t() {}
};

class coupon_application_t
{
public:
  coupon_application_t(int coupon_id_, int dish_id_, int qty_, double price_)
		: coupon_id_(coupon_id_), dish_id_(dish_id_), qty_(qty_), price_(price_) {}

public:
  int coupon_id() const { return coupon_id_; }
  int dish_id() const { return dish_id_; }
  int qty() const { return qty_; }
  double price() const { return price_; }
private:
  int coupon_id_;
  int dish_id_;
  int qty_;
  double price_;
private:
  coupon_application_t() {}
};


class order_dish_t;

class coupon_t
{
public:
	coupon_t(int coupon_id_);
public:
	int coupon_id() const { return coupon_id_; }
	const std::string& title() const { return title_; }
	double discount() const { return discount_; }
	const boost::posix_time::ptime& start_date() const { return start_date_; }
	const boost::posix_time::ptime& stop_date() const { return stop_date_; }
	int auth_req() const { return auth_req_; }
	int condition_rule() const { return condition_rule_; }
	int application_rule() const { return application_rule_; }
	int auto_scalable() const { return auto_scalable_; }
	int apply2condition() const { return apply2condition_; }
	int combinable() const { return combinable_; }
	double fixed_discount() const { return fixed_discount_; }
	double subtotal_condition() const { return subtotal_condition_; }
	int apply() const { return apply_; }
public:
	void set_apply(int count) { apply_ = count; }
private:
	int coupon_id_;
	std::string title_;
	double discount_;
	boost::posix_time::ptime start_date_;
	boost::posix_time::ptime stop_date_;
	int auth_req_;
	int condition_rule_;
	int application_rule_;
	int auto_scalable_;
	int apply2condition_;
	int combinable_;
	double fixed_discount_;
	double subtotal_condition_;
	std::string description_;
public:
	std::vector<coupon_condition_t> condition_;
	std::vector<coupon_application_t> application_;
private:
	int apply_;
private:
	coupon_t();
};


bool exist_condition_for_dish(const coupon_t& coupon, int dish_id);
bool exist_application_for_dish(const coupon_t& coupon, int dish_id);

int condition_qty_for_dish(const coupon_t& coupon, int dish_id);
int application_qty_for_dish(const coupon_t& coupon, int dish_id);

double subtotal(const std::vector<order_dish_t*>& dish_vec);

bool XXX_apply_coupon_XXX(const coupon_t& coupon, const std::vector<order_dish_t*>& dish_vec, bool dummy, int coupon_mng_id);

bool can_apply_coupon(const coupon_t& coupon, const std::vector<order_dish_t*>& dish_vec);
bool apply_coupon(const coupon_t& coupon, const std::vector<order_dish_t*>& dish_vec, int coupon_mng_id);


struct coupon_ptr_valid_pred : public std::unary_function<coupon_t*, bool>
{
  bool operator() (const coupon_t* coupon) const
  {
		const boost::posix_time::ptime null = boost::date_time::not_a_date_time;
		const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		const boost::posix_time::ptime& start = coupon->start_date();
		const boost::posix_time::ptime& stop = coupon->stop_date();
		if (!start.is_not_a_date_time() && !stop.is_not_a_date_time() && start <= now && stop >= now) return true;
    if (!start.is_not_a_date_time() && !stop.is_not_a_date_time() && stop > now) return true;
    if (!start.is_not_a_date_time() && stop.is_not_a_date_time() && start < now) return true;
		if (start.is_not_a_date_time() && stop.is_not_a_date_time()) return true;
    return false;
  }
};

struct coupon_valid_pred : public std::unary_function<coupon_t, bool>
{
  bool operator() (const coupon_t& coupon) const
  {
		const boost::posix_time::ptime null = boost::date_time::not_a_date_time;
		const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		const boost::posix_time::ptime& start = coupon.start_date();
		const boost::posix_time::ptime& stop = coupon.stop_date();
		if (!start.is_not_a_date_time() && !stop.is_not_a_date_time() && start <= now && stop >= now) return true;
    if (!start.is_not_a_date_time() && !stop.is_not_a_date_time() && stop > now) return true;
    if (!start.is_not_a_date_time() && stop.is_not_a_date_time() && start < now) return true;
		if (start.is_not_a_date_time() && stop.is_not_a_date_time()) return true;
    return false;
  }
};


class coupon_storage_t
{
public:
  coupon_storage_t();
public:
  std::vector<coupon_t*> coupon_for_cafe(int cafe_id) const;
private:
  std::vector<coupon_t> coupon_;
  std::multimap<int, coupon_t*> cafe_coupon_map_; // cafe_id -> coupon
private:
  coupon_storage_t& operator=(const coupon_storage_t&);
};

coupon_t& find_coupon(int coupon_id, const std::vector<coupon_t*>&);


class coupon_in_cafe_t
{
public:
  coupon_in_cafe_t() { THROW_LOCATION(10137, "coupon_in_cafe_t(coupon_in_cafe_t())"); }
  explicit coupon_in_cafe_t(std::vector<coupon_t*>& coupon_vec): coupon_(coupon_vec) {}
  const std::vector<coupon_t*> selected() const;
  const std::vector<coupon_t*> coupon() const;
  int coupon_count(coupon_t& c) const { std::map<coupon_t*, int> tmp = selected_; return tmp[&c]; }
  int coupon_count(coupon_t* c) const { std::map<coupon_t*, int> tmp = selected_; return tmp[c]; }
  int mng_id(coupon_t& c) const { std::map<coupon_t*, int> tmp = manager_; return tmp[&c]; }
  int mng_id(coupon_t* c) const { std::map<coupon_t*, int> tmp = manager_; return tmp[c]; }
  int not_combinable_exist() const;
  bool selected_empty() const { return selected_.empty(); }
  bool in_selected(coupon_t*) const;
  void select(int coupon_id, int count, int mng_id);
  void unselect(int coupon_id);
  void unselect(coupon_t*);
  void unselect_all() { selected_.clear(); manager_.clear(); }
private:
  std::vector<coupon_t*> coupon_;
  std::map<coupon_t*, int> selected_;
  std::map<coupon_t*, int> manager_;
private:
//  coupon_in_cafe_t();
};


};

#endif
