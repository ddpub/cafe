#ifndef CAFE_CONTAINER_H_DERGACHEV_14_01_2008
#define CAFE_CONTAINER_H_DERGACHEV_14_01_2008

#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <memory>
#include <ksi_util/util.hpp>


namespace ksi_cafe {

class state_t
{
public:
	state_t(int state_id_, const std::string& name_, const std::string& shortname_): state_id_(state_id_), name_(name_), shortname_(shortname_) {}
	int state_id() const { return state_id_; }
	const std::string& name() const { return name_; }
	const std::string& shortname() const { return shortname_; }
private:
	int state_id_;
	std::string name_;
	std::string shortname_;
};

class address_t
{
public:
	address_t(const std::string& streetaddress_, const std::string& city_, const state_t& state_, int zip_): streetaddress_(streetaddress_), city_(city_), state_(state_), zip_(zip_) {}
	const std::string& streetaddress() const { return streetaddress_; }
	const std::string& city() const { return city_; }
	const state_t& state() const { return state_; }
	int zip() const { return zip_; }
	std::string full_address() const { return streetaddress_ + " " + city_ + ", " + state_.shortname() + " " + boost::lexical_cast<std::string>(zip_); }
private:
	std::string streetaddress_;
	std::string city_;
	state_t state_;
	int zip_;
};


class cafe_t
{
public:
	cafe_t(int cafe_id_, const std::string& chain_, const std::string& name_, const address_t& address_, const std::string& phone_, int delivery_time_, int prepare_time_, double sales_tax_, double delivery_fee_, double delivery_free_subtotal_, double delivery_min_subtotal_, double meal_free_limit_, const std::string& delivery_notes_, const std::string& receipt_header_, const std::string& receipt_footer_, double expense_auth_limit_, int default_table_group_id_, const std::string& default_table_group_name_, double gratuity_)
	: cafe_id_(cafe_id_), chain_(chain_), name_(name_), address_(address_), phone_(phone_), delivery_time_(delivery_time_), prepare_time_(prepare_time_), sales_tax_(sales_tax_), delivery_fee_(delivery_fee_), delivery_free_subtotal_(delivery_free_subtotal_), delivery_min_subtotal_(delivery_min_subtotal_), meal_free_limit_(meal_free_limit_), delivery_notes_(delivery_notes_), receipt_header_(receipt_header_), receipt_footer_(receipt_footer_), expense_auth_limit_(expense_auth_limit_), default_table_group_id_(default_table_group_id_), default_table_group_name_(default_table_group_name_), gratuity_(gratuity_)  {}
	int cafe_id() const { return cafe_id_; }
	const std::string& chain() const { return chain_; }
	const std::string& name() const { return name_; }
	const address_t& address() const { return address_; }
	const std::string& phone() const { return phone_; }
	std::string full_name() const { return name() + " " + address_.full_address(); }
	double sales_tax() const { return sales_tax_; }
	double delivery_fee() const { return round(delivery_fee_); }
	double delivery_free_subtotal() const { return round(delivery_free_subtotal_); }
	double delivery_min_subtotal() const { return round(delivery_min_subtotal_); }
	double meal_free_limit() const { return round(meal_free_limit_); }
	const std::string& delivery_notes() const { return delivery_notes_; }
	const std::string& receipt_header() const { return receipt_header_; }
	const std::string& receipt_footer() const { return receipt_footer_; }
	int setup_time(int type_id) const { if (type_id == 3) return delivery_time_+prepare_time_; return prepare_time_; }
	double expense_auth_limit() const { return expense_auth_limit_; }
	int default_table_group_id() const { return default_table_group_id_; }
	const std::string& default_table_group_name() const { return default_table_group_name_; }
	double gratuity() const { return gratuity_; }
private:
	int cafe_id_;
	std::string chain_;
	std::string name_;
	address_t address_;
	std::string phone_;
	int delivery_time_;
	int prepare_time_;
	double sales_tax_;
	double delivery_fee_;
	double delivery_free_subtotal_;
	double delivery_min_subtotal_;
	double meal_free_limit_;
	std::string delivery_notes_;
	std::string receipt_header_;
	std::string receipt_footer_;
	double expense_auth_limit_;
	int default_table_group_id_;
	std::string default_table_group_name_;
	double gratuity_;
	cafe_t();
};

std::vector<state_t> fetch_state();

class cafe_container_t
{
public:
	explicit cafe_container_t(bool load_for_all_cafe = false);
	const std::vector<cafe_t>& all_cafe() const { return container_; }
	std::vector<int> all_cafe_id() const;
	int position_cafe(int cafe_id) const;
	const cafe_t* current_cafe() const { return current_cafe_ptr_.get(); }
private:
	std::vector<cafe_t> container_;
	std::auto_ptr<cafe_t> current_cafe_ptr_;
};


};


#endif
