#ifndef CASH_ADVANCE_H_DERGACHEV_10_09_2008
#define CASH_ADVANCE_H_DERGACHEV_10_09_2008

#include <ksi_include/ksi_exceptions.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <vector>
#include <ksi_order/cafe_container.hpp>

namespace ksi_cafe {

class expense_type_t {
public:
	expense_type_t(int type_id_, const std::string& name_): type_id_(type_id_), name_(name_) {}
	int type_id() const { return type_id_; }
	const std::string name() const { return name_; }
private:
	int type_id_;
	std::string name_;
private:
	expense_type_t();
};

class expense_t {
public:
	explicit expense_t(int expense_id);
	expense_t(int cafe_id_, int seance_id_, int opener_id_, int type_id_, int manager_id_, double amount_, const std::string description_);
	explicit expense_t();
public:
	bool open() const { return closed_.is_not_a_date_time(); }
	void close(double change_);
	int expense_id() const { return expense_id_; }
	const boost::posix_time::ptime& created() const { return created_; }
	const boost::posix_time::ptime& closed() const { return closed_; }
	double amount() const { return amount_; }
	double change() const { return change_; }
	int opener_id() const { return opener_id_; }
	const std::string& description() const { return description_; }
	int type_id() const { return type_id_; }
	int manager_id() const { return manager_id_; }
private:
	int expense_id_;
	boost::posix_time::ptime created_;
	boost::posix_time::ptime closed_;
	double amount_;
	double change_;
	int opener_id_;
	std::string description_;
	int type_id_;
	int manager_id_;
};

class expense_list_t {
public:
	expense_list_t(const cafe_t& cafe_, int seance_id=0);
public:
	int cafe_id() const { return cafe_id_; }
	int seance_id() const { return seance_id_; }
	double max() const { return max_; }
	std::vector<expense_type_t> type_vec() const { return type_vec_; }
	std::vector<expense_t> expense_vec() const { return expense_vec_; }
	std::vector<const expense_t*> open_expense_vec();
	std::string type_name(int type_id) const;
	void open_expense(int opener_id, int type_id, double amount, const std::string& descr, int manager_id) {
		expense_vec_.push_back(expense_t(cafe_id_, seance_id_, opener_id, type_id, manager_id, amount, descr));
	}
	void close_expense(int expense_id, double change);
	size_t open_expense_size() const; 
private:
	int cafe_id_;
	int seance_id_;
	std::vector<expense_type_t> type_vec_;
	std::vector<expense_t> expense_vec_;
	double max_;
};

};

#endif
