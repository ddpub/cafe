#ifndef DMITRY_DERGACHEV_06_02_2010_CREDIT_UTILS_HPP
#define DMITRY_DERGACHEV_06_02_2010_CREDIT_UTILS_HPP

#include <string>
#include <vector>

class cctrans_record_t {
public:
	cctrans_record_t(): type_id_(0), amount_(0), tips_(0), batch_id_(0), ref_txn_id_(0), txn_id_(0), cc_num_(""), cc_type_("") {}
	cctrans_record_t(int type_id_, double amount_, double tips_, int batch_id_, int ref_txn_id_, int txn_id_, const std::string& cc_num_, const std::string& cc_type_)
	: type_id_(type_id_), amount_(amount_), tips_(tips_), batch_id_(batch_id_), ref_txn_id_(ref_txn_id_), txn_id_(txn_id_), cc_num_(cc_num_), cc_type_(cc_type_) {}
	int type_id() const { return type_id_; }
	double amount() const { return amount_; }
	double tips() const { return tips_; }
	int batch_id() const { return batch_id_; }
	int ref_txn_id() const { return ref_txn_id_; }
	int txn_id() const { return txn_id_; }
	const std::string& cc_num() const { return cc_num_; }
	const std::string& cc_type() const { return cc_type_; }
private:
	int type_id_;
	double amount_;
	double tips_;
	int batch_id_;
	int ref_txn_id_;
	int txn_id_;
	std::string cc_num_;	
	std::string cc_type_;
};

typedef std::vector<cctrans_record_t> cctrans_vec_t;

cctrans_vec_t fetch_cctrans_for_order(int order_id);
cctrans_vec_t valid_cctrans(const cctrans_vec_t& vec);

cctrans_record_t payment_auth(const cctrans_vec_t& vec, int payment_type, int prepayment_type);
cctrans_record_t payment_add_to_batch(const cctrans_vec_t& vec, int payment_type, int prepayment_type);
cctrans_record_t prepayment_auth(const cctrans_vec_t& vec, int payment_type, int prepayment_type);
cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t& vec, int payment_type, int prepayment_type);
void credit_void_fn(int order_id, int batch_id, const cctrans_record_t& add_to_batch_rec, const cctrans_record_t& auth_rec, const std::string& refund_note);

class wxWindow;
void credit_refund_fn(wxWindow* win, int order_id, int& current_batch_id, const cctrans_record_t &add_to_batch_rec, const std::string& refund_note);

#endif
