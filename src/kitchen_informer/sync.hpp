#ifndef DMITRY_DERGACHEV_27_03_2008_SYNC_H
#define DMITRY_DERGACHEV_27_03_2008_SYNC_H


#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <oracle_client/oracle_client.hpp>
#include <oracle_client/ksi_exception.hpp>
#include <ksi_include/monitor.hpp>

namespace ksi_kitchen {

class dish_sync_t {
public:
	explicit dish_sync_t(ksi_client::oracle_session_i* session_): session_(session_) { refresh(); }
public:
	const std::vector<int>& order_id_vec() const { return order_id_vec_; }
private:
	void refresh();
private:
	std::vector<int> order_id_vec_;
	ksi_client::oracle_session_i* session_;
private:
	dish_sync_t(dish_sync_t&);
};

class mod_sync_t {
public:
	explicit mod_sync_t(ksi_client::oracle_session_i* session_): session_(session_) { refresh(); }
public:
	const std::vector<int>& order_id_vec() const { return order_id_vec_; }
private:
	void refresh();
private:
	std::vector<int> order_id_vec_;
	ksi_client::oracle_session_i* session_;
private:
	mod_sync_t(mod_sync_t&);
};

class sync_queue_t {
	sync_queue_t(ksi_client::oracle_session_i* session_): session_(session_), dishComplete_(true) {
		dish_sync_t dish_sync(session_);
		std::vector<int> vec = dish_sync.order_id_vec();
		for (size_t i=0; i<vec.size(); ++i) queue_.push(vec[i]);
	}
	void nextStep() {
		if (dishComplete_) {
			mod_sync_t mod_sync(session_);
			std::vector<int> vec = mod_sync.order_id_vec();
			for (size_t i=0; i<vec.size(); ++i) queue_.push(vec[i]);
			dishComplete_ = false;
		}
		else {
			dish_sync_t dish_sync(session_);
			std::vector<int> vec = dish_sync.order_id_vec();
			for (size_t i=0; i<vec.size(); ++i) queue_.push(vec[i]);		
			dishComplete_ = true;
		}
	}
	int getOrderId() {
		while (!queue_.size()) nextStep();
		return queue_.popWait();
	}
private:
	ksi_client::oracle_session_i* session_;
	MonitorQueue<int> queue_;
	bool dishComplete_;
};

}

#endif

