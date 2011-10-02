#ifndef DMITRY_DERGACHEV_WX_TREE_NODE_H_20_05_2008
#define DMITRY_DERGACHEV_WX_TREE_NODE_H_20_05_2008

#include <wx/treectrl.h>
#include <boost/date_time/posix_time/posix_time.hpp>


namespace node_type
{
  enum type_t {empty, error, modifier_group, menu_modifier, order_dish, order_modifier, dish_comment, discount, modifier_discount, other, expense };
};


template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class wx_node_t: public wxTreeItemData {
public:
	wx_node_t(node_type::type_t type_, T1 col1_, T2 col2_, T3 col3_, T4 col4_, T5 col5_, T6 col6_, T7 col7_, T8 col8_, T9 col9_, T10 col10_): type_(type_), col1_(col1_), col2_(col2_), col3_(col3_), col4_(col4_), col5_(col5_), col6_(col6_), col7_(col7_), col8_(col8_), col9_(col9_), col10_(col10_) {}
	wx_node_t(node_type::type_t type_, T1 col1_, T2 col2_, T3 col3_, T4 col4_, T5 col5_, T6 col6_, T7 col7_, T8 col8_, T9 col9_): type_(type_), col1_(col1_), col2_(col2_), col3_(col3_), col4_(col4_), col5_(col5_), col6_(col6_), col7_(col7_), col8_(col8_), col9_(col9_) {}
	wx_node_t(node_type::type_t type_, T1 col1_, T2 col2_, T3 col3_, T4 col4_, T5 col5_, T6 col6_, T7 col7_, T8 col8_): type_(type_), col1_(col1_), col2_(col2_), col3_(col3_), col4_(col4_), col5_(col5_), col6_(col6_), col7_(col7_), col8_(col8_) {}
	wx_node_t(node_type::type_t type_, T1 col1_, T2 col2_, T3 col3_, T4 col4_, T5 col5_, T6 col6_, T7 col7_): type_(type_), col1_(col1_), col2_(col2_), col3_(col3_), col4_(col4_), col5_(col5_), col6_(col6_), col7_(col7_) {}
	wx_node_t(node_type::type_t type_, T1 col1_, T2 col2_, T3 col3_, T4 col4_, T5 col5_, T6 col6_): type_(type_), col1_(col1_), col2_(col2_), col3_(col3_), col4_(col4_), col5_(col5_), col6_(col6_) {}
	wx_node_t(node_type::type_t type_, T1 col1_, T2 col2_, T3 col3_, T4 col4_, T5 col5_): type_(type_), col1_(col1_), col2_(col2_), col3_(col3_), col4_(col4_), col5_(col5_) {}
	wx_node_t(node_type::type_t type_, T1 col1_, T2 col2_, T3 col3_, T4 col4_): type_(type_), col1_(col1_), col2_(col2_), col3_(col3_), col4_(col4_) {}
	wx_node_t(node_type::type_t type_, T1 col1_, T2 col2_, T3 col3_): type_(type_), col1_(col1_), col2_(col2_), col3_(col3_) {}
	wx_node_t(node_type::type_t type_, T1 col1_, T2 col2_): type_(type_), col1_(col1_), col2_(col2_) {}
	wx_node_t(node_type::type_t type_, T1 col1_): type_(type_), col1_(col1_) {}
	node_type::type_t type() const { return type_; }
	T1 col1() const { return col1_; }
	T2 col2() const { return col2_; }
	T3 col3() const { return col3_; }
	T4 col4() const { return col4_; }
	T5 col5() const { return col5_; }
	T6 col6() const { return col6_; }
	T7 col7() const { return col7_; }
	T8 col8() const { return col8_; }
	T9 col9() const { return col9_; }
	void col1(const T1& val) { col1_ = val; }
	void col2(const T2& val) { col2_ = val; }
	void col3(const T3& val) { col3_ = val; }
	void col4(const T4& val) { col4_ = val; }
	void col5(const T5& val) { col5_ = val; }
	void col6(const T6& val) { col6_ = val; }
	void col7(const T7& val) { col7_ = val; }
	void col8(const T8& val) { col8_ = val; }
	void col9(const T9& val) { col9_ = val; }
	void col10(const T10& val) { col10_ = val; }
private:
	node_type::type_t type_;
	T1 col1_;
	T2 col2_;
	T3 col3_;
	T4 col4_;
	T5 col5_;
	T6 col6_;
	T7 col7_;
	T8 col8_;
	T9 col9_;
	T10 col10_;
};

typedef wx_node_t<const void*, double, std::string, int, int, int, int, int, int, int> order_node_t;
typedef wx_node_t<const void*, int, int, int, int, int, int, int, int, int> mod_node_t;

/*
col1  -  order_id
col2  -  created
col3  -  subtotal
col4  -  payment_type_id
col5  -  payment_type_str
col6  -  room
col7  -  terminal
col8  -  closer_id
col9  -  prepayment_type_id
col10 -  prepayment_type_str
*/
typedef wx_node_t<int, boost::posix_time::ptime, double, int, std::string, std::string, std::string, int, int, std::string> close_order_t;


typedef wx_node_t<const void*, int, int, int, int, int, int, int, int, int> expense_node_t;


#endif
