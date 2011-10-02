
#include "cash_advance.hpp"
#include <shared_modules/connect.hpp>
#include <ksi_util/util.hpp>
#include <ksi_include/algo_functor.hpp>
#include <boost/bind.hpp>
#include <ksi_util/log.hpp>

extern std::auto_ptr<file_log_t> flog;

namespace ksi_cafe {

expense_t::expense_t(int expense_id) {
	expense_id_ = expense_id;
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select \
	created \
	, closed \
	, amount \
	, change \
	, opener_id \
	, description \
	, type_id \
	, manager_id \
	from ksi.expense ex \
	where expense_id = :expense_id"
	);
	query->set("expense_id", expense_id);
	query->execute_statement();
	
	if (query->next()) THROW_LOCATION(0, "expense_t::expense_t(int)");
	
	created_ = boost::get<boost::posix_time::ptime>(query->get(0));
	closed_ = boost::get<boost::posix_time::ptime>(query->get(1));
	amount_ = round(boost::get<double>(query->get(2)));
	change_ = round(boost::get<double>(query->get(3)));
	opener_id_ = static_cast<int>(boost::get<double>(query->get(4)));
	description_ = boost::get<std::string>(query->get(5));
	type_id_ = static_cast<int>(boost::get<double>(query->get(6)));
	manager_id_ = static_cast<int>(boost::get<double>(query->get(7)));
}

expense_t::expense_t(int cafe_id_, int seance_id_, int opener_id_, int type_id_, int manager_id_, double amount_, const std::string description_) 
: opener_id_(opener_id_), type_id_(type_id_), manager_id_(manager_id_), amount_(amount_), description_(description_) {
	ora_transaction_t trans(session.get());
	logical_transaction_t logical_trans(session.get(), 109);
	seq_t seq(session.get(), "expense_seq");
	int expense_id_ = seq.nextval();
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"insert into ksi.expense (seance_id, expense_id, opener_id, type_id, amount, cafe_id, description, created) \
    values(:seance_id, :expense_id, :opener_id, :type_id, :amount, :cafe_id, :descr, sysdate)"
	);
	query->set("seance_id", seance_id_);
	query->set("expense_id", expense_id_);
	query->set("opener_id", opener_id_);
	query->set("type_id", type_id_);
	query->set("amount", amount_);
	query->set("cafe_id", cafe_id_);
	query->set("descr", description_);
	query->execute_statement();
	logical_trans.logical_end();
	trans.commit();
}

void expense_t::close(double change) {
	ora_transaction_t trans(session.get());
	logical_transaction_t logical_trans(session.get(), 110);
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"update ksi.expense set change=:change, closed=sysdate where expense_id=:expense_id"
	);
	query->set("change", change);
	query->set("expense_id", expense_id_);
	query->execute_statement();
	closed_ = boost::posix_time::second_clock::local_time();
	logical_trans.logical_end();
	trans.commit();
}

std::string expense_list_t::type_name(int type_id) const {
	std::vector<expense_type_t>::const_iterator It =
	std::find_if(type_vec_.begin(), type_vec_.end(), boost::bind(std::equal_to<int>(), type_id, boost::bind(&expense_type_t::type_id, _1)));
	if (It == type_vec_.end()) THROW_LOCATION(0, "std::string expense_list_t::type_name(int) const");
	return It->name();
}

size_t expense_list_t::open_expense_size() const {
	return count_if(expense_vec_.begin(), expense_vec_.end(), boost::bind(&expense_t::open, _1));
}

void expense_list_t::close_expense(int expense_id, double change) {
	std::vector<expense_t>::iterator It = 
	std::find_if(expense_vec_.begin(), expense_vec_.end(), boost::bind(std::equal_to<int>(), expense_id, boost::bind(&expense_t::expense_id, _1)));
	if (It == expense_vec_.end()) THROW_LOCATION(0, "void expense_list_t::close_expense(int, double)");
	It->close(change);
}

std::vector<const expense_t*> expense_list_t::open_expense_vec() {
	std::vector<const expense_t*> res_vec;
	for (size_t i=0; i<expense_vec_.size(); ++i) {
		if (expense_vec_[i].open()) res_vec.push_back(&expense_vec_[i]);
	}
	return res_vec;
}

expense_list_t::expense_list_t(const cafe_t& cafe_, int seance_id): seance_id_(seance_id) {
	cafe_id_ = cafe_.cafe_id();
	max_ = cafe_.expense_auth_limit();
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select type_id, name from ksi.expense_type"
	);
	query->execute_statement();
	while (!query->next()) {
		int type_id = static_cast<int>(boost::get<double>(query->get(0)));
		std::string name = boost::get<std::string>(query->get(1));
		type_vec_.push_back(expense_type_t(type_id, name));
	}
	if (seance_id_ == 0) {
		query->create_statement("select ksi.util.get_cur_seance_id from dual");
		query->execute_statement();
		seance_id_ = static_cast<int>(boost::get<double>(query->get(0)));
	}
	query->create_statement
	(
	"select expense_id \
	from ksi.expense \
	where seance_id = case when :seance_id = 0 then ksi.util.get_cur_seance_id else :seance_id end and cafe_id = :cafe_id and closed is null"
	);
	
	query->set("seance_id", seance_id_);
	query->set("cafe_id", cafe_id_);
	query->execute_statement();
	while (!query->next()) {
		int expense_id = static_cast<int>(boost::get<double>(query->get(0)));
		expense_vec_.push_back(expense_t(expense_id));
	}
}

};
