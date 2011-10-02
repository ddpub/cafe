#ifndef __delayed_order_f__
#define __delayed_order_f__

/**
@file
Subclass of base_delayed_order_f, which is generated by wxFormBuilder.
*/

#include "base_delayed_order_f.h"
#include <vector>
#include <ksi_order/cafe_container.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/shared_ptr.hpp>

namespace ksi_cafe {
	class order_t;
	class table_t;
}

/** Implementing base_delayed_order_f */
class delayed_order_f : public base_delayed_order_f
{
protected:
	// Handlers for base_delayed_order_f events.
	void fname_edClick( wxMouseEvent& event );
	void lname_edClick( wxMouseEvent& event );
	void address_edClick( wxMouseEvent& event );
	void city_edClick( wxMouseEvent& event );
	void zip_edClick( wxMouseEvent& event );
	void state_cbChoice( wxCommandEvent& event );
	void phone_edClick( wxMouseEvent& event );
	void set_date_btnClick( wxCommandEvent& event );
	void set_time_btnClick( wxCommandEvent& event );
	void prepayment_btnClick( wxCommandEvent& event );
	void cancel_prepayment_btnClick( wxCommandEvent& event );
	void auth_cc_btnClick( wxCommandEvent& event );
	void cancel_auth_btnClick( wxCommandEvent& event );
	void print_sleep_btnClick( wxCommandEvent& event );
	void cancel_order_btnClick( wxCommandEvent& event );
	void order_notes_edClick( wxMouseEvent& event );
	void save_btnClick( wxCommandEvent& event );
	void save_and_hide_btnClick( wxCommandEvent& event );
	void cancel_btnClick( wxCommandEvent& event );
	void save_and_print_btnClick( wxCommandEvent& event );
	void select_table_btnClick( wxCommandEvent& event );
public:
	/** Constructor */
	delayed_order_f( wxWindow* parent, const wxString& title, int type_id_, bool hide_exist/*=false*/, ksi_cafe::order_t* order);
	ksi_cafe::order_t* release() { return order; }
private:
	ksi_cafe::order_t* order;
	std::auto_ptr<ksi_cafe::table_t> table_;
private:
	void change_item();
	void set_date(const boost::gregorian::date& date);
	void set_time(const boost::posix_time::time_duration& time);
	void set_delivery_pickup_info();
	std::vector<ksi_cafe::state_t> state_vec_;
	int type_id_;
	boost::posix_time::time_duration selected_time_;
	boost::gregorian::date selected_date_;
	bool dummy_;
	bool usMode_;
};

#endif // __delayed_order_f__