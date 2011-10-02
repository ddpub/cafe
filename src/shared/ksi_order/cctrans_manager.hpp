#ifndef CCTRANS_MANAGER_H_DERGACHEV_01_02_2008
#define CCTRANS_MANAGER_H_DERGACHEV_01_02_2008

#include <string>
#include <vector>


namespace ksi_cafe {

class cctrans_record_t
{
public:
	cctrans_record_t( int type_id_, double amount_, double tips_
									, int txn_id_, const std::string& l4d_, int batch_id_
									, int ref_txn_id_, const std::string& card_type_, int trans_id_ )
									: type_id_(type_id_), amount_(amount_), tips_(tips_)
									, txn_id_(txn_id_), l4d_(l4d_), batch_id_(batch_id_)
									, ref_txn_id_(ref_txn_id_), card_type_(card_type_), trans_id_(trans_id_) {}
public:
	int type_id() const { return type_id_; }
	double amount() const { return amount_; }
	double tips() const { return tips_; }
	int txn_id() const { return txn_id_; }
	const std::string& last_4_digits() const { return l4d_; }
	int batch_id() const { return batch_id_; }
	int ref_txn_id() const { return ref_txn_id_; }
	const std::string& card_type() const { return card_type_; }
	int trans_id() const { return trans_id_; }
private:
	int type_id_;
	double amount_;
	double tips_;
	int txn_id_;
	std::string l4d_;
	int batch_id_;
	int ref_txn_id_;
	std::string card_type_;
	int trans_id_;
private:
	cctrans_record_t();	
};

typedef std::vector<cctrans_record_t> cctrans_vec_t;

cctrans_vec_t fetch_cctrans_for_order(int order_id);
cctrans_vec_t valid_cctrans(const cctrans_vec_t& vec);
cctrans_record_t prepayment_auth(const cctrans_vec_t& vec, int payment_type_id, int prepayment_type_id);
cctrans_record_t payment_auth(const cctrans_vec_t& vec, int payment_type_id, int prepayment_type_id);
cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t& vec, int payment_type_id, int prepayment_type_id);
cctrans_record_t payment_add_to_batch(const cctrans_vec_t& vec, int payment_type_id, int prepayment_type_id);

};


#endif
