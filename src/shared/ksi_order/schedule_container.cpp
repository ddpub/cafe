
#include "schedule_container.hpp"
#include <shared_modules/connect.hpp>
#include <algorithm>
#include <functional>
#include <memory>
#include <boost/bind.hpp>
#include <ksi_include/ksi_exceptions.hpp>


namespace ksi_cafe {

cafe_schedule_t::cafe_schedule_t()
{
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement("select ksi.util.get_cur_cafe_id from dual");
	query->execute_statement();
	if (query->next()) THROW_LOCATION(0, "cafe_schedule_t::cafe_schedule_t(int)");
	
	int cafe_id_ = static_cast<int>(boost::get<double>(query->get(0)));
	
	query->create_statement
	(
	"select weekday_mask, open_time, close_time from ksi.cafe \
	where cafe_id = :cafe_id"
	);
	query->set("cafe_id", cafe_id_);
	query->execute_statement();
	
	if (query->next()) THROW_LOCATION(1, "cafe_schedule_t::cafe_schedule_t(int)");
	
	week_mask_ = static_cast<int>(boost::get<double>(query->get(0)));
	start_time_ = boost::get<boost::posix_time::ptime>(query->get(1));
	stop_time_ = boost::get<boost::posix_time::ptime>(query->get(2));
}

cafe_schedule_t::cafe_schedule_t(int cafe_id_): cafe_id_(cafe_id_)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select weekday_mask, open_time, close_time from ksi.cafe \
	where cafe_id = :cafe_id"
	);
	query->set("cafe_id", cafe_id_);
	query->execute_statement();
	
	if (query->next()) THROW_LOCATION(0, "cafe_schedule_t::cafe_schedule_t(int)");
	
	week_mask_ = static_cast<int>(boost::get<double>(query->get(0)));
	start_time_ = boost::get<boost::posix_time::ptime>(query->get(1));
	stop_time_ = boost::get<boost::posix_time::ptime>(query->get(2));
}

boost::posix_time::ptime cafe_schedule_t::start_time(int day_of_week) const
{
	int mask;
	if (day_of_week == 0) mask = 0x1;
	if (day_of_week == 1) mask = 0x2;
	if (day_of_week == 2) mask = 0x4;
	if (day_of_week == 3) mask = 0x8;
	if (day_of_week == 4) mask = 0x10;
	if (day_of_week == 5) mask = 0x20;
	if (day_of_week == 6) mask = 0x40;

	if (week_mask_ & mask) return start_time_;
	return boost::posix_time::not_a_date_time;
}

boost::posix_time::ptime cafe_schedule_t::stop_time(int day_of_week) const 
{
	int mask;
	if (day_of_week == 0) mask = 0x1;
	if (day_of_week == 1) mask = 0x2;
	if (day_of_week == 2) mask = 0x4;
	if (day_of_week == 3) mask = 0x8;
	if (day_of_week == 4) mask = 0x10;
	if (day_of_week == 5) mask = 0x20;
	if (day_of_week == 6) mask = 0x40;

	if (week_mask_ & mask) return stop_time_;
	return boost::posix_time::not_a_date_time;
}

void delivery_schedule_container_t::load() {
	std::vector<delivery_schedule_t> tmp;
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select zip, open_time, close_time, weekday_mask, cafe_id \
	from ksi.delivery_schedule \
	where active = 1"
	);
	
	query->execute_statement();
	while (!query->next()) {
		int zip = static_cast<int>(boost::get<double>(query->get(0)));
		boost::posix_time::ptime start_time = boost::get<boost::posix_time::ptime>(query->get(1));
		boost::posix_time::ptime stop_time = boost::get<boost::posix_time::ptime>(query->get(2));
		int week_mask = static_cast<int>(boost::get<double>(query->get(3)));
		int cafe_id = static_cast<int>(boost::get<double>(query->get(4)));
		tmp.push_back(delivery_schedule_t(cafe_id, zip, start_time, stop_time, week_mask));
	}
	std::swap(container_, tmp);
	loaded_ = true;
}

int delivery_schedule_container_t::zip_exist(int zip) const
{
	std::vector<delivery_schedule_t>::const_iterator It;
	It = std::find_if(container_.begin(), container_.end(), boost::bind(std::equal_to<int>(), zip, boost::bind(&delivery_schedule_t::zip, _1)));
	if (It == container_.end()) return 0;
	return It->cafe_id();
}

boost::posix_time::ptime delivery_schedule_container_t::start_time(int zip, int day_of_week) const
{
	int mask;
	if (day_of_week == 0) mask = 0x1;
	if (day_of_week == 1) mask = 0x2;
	if (day_of_week == 2) mask = 0x4;
	if (day_of_week == 3) mask = 0x8;
	if (day_of_week == 4) mask = 0x10;
	if (day_of_week == 5) mask = 0x20;
	if (day_of_week == 6) mask = 0x40;

	for (std::vector<delivery_schedule_t>::const_iterator i=container_.begin(); i!=container_.end(); ++i) {
		if ((*i).zip() == zip && ((*i).week_mask() & mask))
			return (*i).start_time();
		}
	return boost::posix_time::not_a_date_time;
}

boost::posix_time::ptime delivery_schedule_container_t::stop_time(int zip, int day_of_week) const
{
	int mask;
	if (day_of_week == 0) mask = 0x1;
	if (day_of_week == 1) mask = 0x2;
	if (day_of_week == 2) mask = 0x4;
	if (day_of_week == 3) mask = 0x8;
	if (day_of_week == 4) mask = 0x10;
	if (day_of_week == 5) mask = 0x20;
	if (day_of_week == 6) mask = 0x40;
	
	for (std::vector<delivery_schedule_t>::const_iterator i=container_.begin(); i!=container_.end(); ++i) {
		if ((*i).zip() == zip && ((*i).week_mask() & mask))
			return (*i).stop_time();
		}
	return boost::posix_time::not_a_date_time;
}

int delivery_schedule_container_t::week_mask(int zip) const
{
  int res = 0;
  for (std::vector<delivery_schedule_t>::const_iterator i=container_.begin(); i!=container_.end(); ++i) {
	if ((*i).zip() == zip) res = res | (*i).week_mask();
  }
  return res;
}

/*
void cafe_schedule_container_t::load()
{
	std::vector<cafe_schedule_t> tmp;
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select cafe_id \
	from ksi.cafe \
	where type_id = 2 and deleted is null \
	order by cafe_id"
	);
	query->execute_statement();
	
	while (!query->next()) {
		int cafe_id = static_cast<int>(boost::get<double>(query->get(0)));
		tmp.push_back(cafe_schedule_t(cafe_id));
	}
	std::swap(container_, tmp);
	loaded_ = true;
}

const cafe_schedule_t& cafe_schedule_container_t::schedule(int cafe_id) const
{
  std::vector<cafe_schedule_t>::const_iterator It;
  It = std::find_if(container_.begin(), container_.end(), boost::bind(std::equal_to<int>(), cafe_id, boost::bind(&cafe_schedule_t::cafe_id, _1)));
  if (It == container_.end()) THROW_MESSAGE(0, "cafe not found: cafe_id = " + boost::lexical_cast<std::string>(cafe_id), "const cafe_schedule_t& cafe_schedule_container_t::schedule(int) const");
  return *It;
}
*/

};
