#ifndef DMITRY_DERGACHEV_25_09_2008_SCHEDULE_CONTAINER_H
#define DMITRY_DERGACHEV_25_09_2008_SCHEDULE_CONTAINER_H

#include <string>
#include <vector>
#include <boost/date_time/posix_time/ptime.hpp>

namespace ksi_cafe {

class cafe_schedule_t {
public:
	explicit cafe_schedule_t();
	explicit cafe_schedule_t(int cafe_id_);
	boost::posix_time::ptime start_time(int day_of_week) const;
	boost::posix_time::ptime stop_time(int day_of_week) const;
	int cafe_id() const { return cafe_id_; }
private:
	int week_mask() const { return week_mask_; }
private:
	int cafe_id_;
	boost::posix_time::ptime start_time_;
	boost::posix_time::ptime stop_time_;
	int week_mask_;
};

class delivery_schedule_t {
public:
	delivery_schedule_t(int cafe_id_, int zip_, const boost::posix_time::ptime& start_time_, const boost::posix_time::ptime& stop_time_, int week_mask_)
	: cafe_id_(cafe_id_), zip_(zip_), start_time_(start_time_), stop_time_(stop_time_), week_mask_(week_mask_) {}
	int cafe_id() const { return cafe_id_; }
	int zip() const { return zip_; }
	const boost::posix_time::ptime& start_time() const { return start_time_; }
	const boost::posix_time::ptime& stop_time() const { return stop_time_; }
	int week_mask() const { return week_mask_; }
private:
	int cafe_id_;
	int zip_;
	boost::posix_time::ptime start_time_;
	boost::posix_time::ptime stop_time_;
	int week_mask_;
};

class delivery_schedule_container_t {
public:
	explicit delivery_schedule_container_t(): loaded_(false) {}
	int zip_exist(int zip) const;
	boost::posix_time::ptime start_time(int zip, int day_of_week) const;
	boost::posix_time::ptime stop_time(int zip, int day_of_week) const;
	void load();
	bool loaded() const { return loaded_; }
private:
	int week_mask(int zip) const;
private:
	std::vector<delivery_schedule_t> container_;
	bool loaded_;
};

/*
class cafe_schedule_container_t {
public:
	explicit cafe_schedule_container_t(): loaded_(false) {}
	const cafe_schedule_t& schedule(int cafe_id) const;  
	void load();
	bool loaded() const { return loaded_; }
private:
	std::vector<cafe_schedule_t> container_;
	bool loaded_;
};
*/

};


#endif
