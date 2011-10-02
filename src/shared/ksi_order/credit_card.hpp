#ifndef CREDIT_CARD_H_DERGACHEV_15_01_2008
#define CREDIT_CARD_H_DERGACHEV_15_01_2008

#include <string>

namespace ksi_cafe {

class card_info_t
{
public:
	card_info_t(const std::string& num_, const std::string& type_, const std::string& holder_, bool swiped_)
		: num_(num_), type_(type_), holder_(holder_), swiped_(swiped_) {}

	card_info_t(const card_info_t& cc)
		: num_(cc.num_), type_(cc.type_), holder_(cc.holder_), swiped_(cc.swiped_) {}

	const std::string& num() const { return num_; }
	const std::string& type() const { return type_; }
	const std::string& holder() const { return holder_; }
	bool swiped() const { return swiped_; }
private:
	std::string num_;
	std::string type_;
	std::string holder_;
	bool swiped_;
};


class credit_return_t
{
public:
	credit_return_t(const card_info_t& cc_, int txn_, int batch_)
		: cc_(cc_), txn_(txn_), batch_(batch_) {}

	credit_return_t(const credit_return_t& ret)
		: cc_(ret.cc_), txn_(ret.txn_), batch_(ret.batch_) {}

	int txn() const { return txn_; }
	int batch() const { return batch_; }
	const std::string& num() const { return cc_.num(); }
	const std::string& type() const { return cc_.type(); }
	const std::string& holder() const { return cc_.holder(); }
	bool swiped() const { return cc_.swiped(); }
	const card_info_t& card_info() const { return cc_; }
private:
	card_info_t cc_;
	int txn_;
	int batch_;
};

class credit_auth_t
{
public:
	credit_auth_t(const card_info_t& cc_, int txn_, const std::string& auth_code_): cc_(cc_), txn_(txn_), auth_code_(auth_code_) {}
	credit_auth_t(const credit_auth_t& auth): cc_(auth.cc_), txn_(auth.txn_), auth_code_(auth.auth_code_) {}

	int txn() const { return txn_; }
	int batch() const { return 0; }
	int ref_txn() const { return 0; }
	const std::string& auth_code() const { return auth_code_; }
	const std::string& num() const { return cc_.num(); }
	const std::string& type() const { return cc_.type(); }
	const std::string& holder() const { return cc_.holder(); }
	bool swiped() const { return cc_.swiped(); }
	const card_info_t& card_info() const { return cc_; }
private:
	card_info_t cc_;
	int txn_;
	std::string auth_code_;
};

class credit_batch_t
{
public:
	credit_batch_t(const credit_auth_t& auth_, int txn_, int batch_): auth_(auth_), txn_(txn_), batch_(batch_) {}
	credit_batch_t(const credit_batch_t& cbatch): auth_(cbatch.auth_), txn_(cbatch.txn_), batch_(cbatch.batch_) {}
	int txn() const { return txn_; }
	int batch() const { return batch_; }
	int ref_txn() const { return auth_.txn(); }
	const std::string& num() const { return auth_.num(); }
	const std::string& type() const { return auth_.type(); }
	const std::string& holder() const { return auth_.holder(); }
	bool swiped() const { return auth_.swiped(); }
private:
	credit_auth_t auth_;
	int txn_;
	int batch_;
};

template <typename T> class credit_void_t
{
public:
  credit_void_t<T>(const T& cc_, int txn_): cc_(cc_), txn_(txn_) {}
  credit_void_t<T>(const credit_void_t<T>& cvoid): cc_(cvoid.cc_), txn_(cvoid.txn_) {}
  int txn() const { return txn_; }
  int batch() const { return cc_.batch(); }
  int ref_txn() const { return cc_.txn(); }
	const std::string& num() const { return cc_.num(); }
	const std::string& type() const { return cc_.type(); }
	const std::string& holder() const { return cc_.holder(); }
  bool swiped() const { return cc_.swiped(); }
private:
  T cc_;
  int txn_;
};

void credit_auth_processing(const credit_auth_t& cc_auth, int order_id, double amount);
void credit_batch_processing(const credit_batch_t& cc_batch, int order_id, double amount, double tip);
void credit_void_processing(const credit_void_t<credit_auth_t>& cc_void, int order_id, double amount);
void credit_void_processing(const credit_void_t<credit_batch_t>& cc_void, int order_id, double amount);
void credit_return_processing(const credit_return_t& cc_return, int order_id, double amount);

};

#endif
