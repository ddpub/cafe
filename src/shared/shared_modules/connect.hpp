#ifndef CONNECT_H_DERGACHEV_24_12_2007
#define CONNECT_H_DERGACHEV_24_12_2007

#include <oracle_client/oracle_client.hpp>
#include <oracle_client/ksi_exception.hpp>
#include <boost/lexical_cast.hpp>
#include <memory>

namespace ksi_cafe {

extern std::auto_ptr<ksi_client::oracle_session_i> session;

class seq_t
{
public:
	seq_t(ksi_client::oracle_session_i* session_, const std::string& seq_name_)
		: seq_name_(seq_name_), session_(session_), last_val_(0) {}
	int lastval() const { return last_val_; }
	int nextval()
	{
		std::auto_ptr<ksi_client::oracle_query_i> query = session_->create_query();
		query->create_statement("select ksi." + seq_name_ + ".nextval as val from dual");
		query->execute_statement();
		last_val_ = static_cast<int>(boost::get<double>(query->get(0)));
		return last_val_;
	}
private:
	std::string seq_name_;
	ksi_client::oracle_session_i* session_;
	int last_val_;
};

class ora_transaction_t {
public:
	explicit ora_transaction_t(ksi_client::oracle_session_i* session_)
		: owner(!in_trans), session_(session_) {
		if (owner) {
			//if (session_->connected())
				//session_->start_transaction();
			in_trans = true;
		}
	}
	~ora_transaction_t() {
		if (owner) {
			//if (session_->connected())
				try {
					session_->rollback();
				}
				catch (...) {
				}
			in_trans = false;
		}
	}
	bool commit(bool force=false) {
		if (owner) {
			//if (session_->connected())
				session_->commit();
			in_trans = false;
			owner = false;
			return true;
		}
		else if (force) {
			session_->commit();
			in_trans = false;
			owner = false;
			return false;
		}
		return true;
	}
private:
	static bool in_trans;
	bool owner;
	ksi_client::oracle_session_i* session_;
private:
	ora_transaction_t();
	ora_transaction_t(const ora_transaction_t&);
	ora_transaction_t& operator=(const ora_transaction_t&);
};


class logical_transaction_t
{
public:
	logical_transaction_t(ksi_client::oracle_session_i* session_, long num)
		: session_(session_), logical_end_(false) {
		std::auto_ptr<ksi_client::oracle_query_i> query = session_->create_query();
		query->create_statement("begin ksi.transaction_begin("+boost::lexical_cast<std::string>(num)+"); end;");
		query->execute_statement();
	}
	void logical_end() {
		std::auto_ptr<ksi_client::oracle_query_i> query = session_->create_query();
		query->create_statement("begin ksi.transaction_end; end;");
		query->execute_statement();
		logical_end_ = true;
	}
	~logical_transaction_t() {
		if (!logical_end_) {
			try {
				logical_end();
			}
			catch (...) {
			}
		}
	}
private:
	ksi_client::oracle_session_i* session_;
	bool logical_end_;
};

bool session_exist();

};

#endif
