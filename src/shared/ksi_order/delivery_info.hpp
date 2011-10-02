#ifndef DELIVERY_INFO_H_DERGACHEV_16_06_2009
#define ORDER_INTERFACE_H_DERGACHEV_16_06_2009

#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace ksi_cafe {

	class delivery_info_t {
	public:
		delivery_info_t(const std::string& firstname_, const std::string& lastname_, const std::string& phone_, const boost::posix_time::ptime& delivery_time_) : firstname_(firstname_), lastname_(lastname_), phone_(phone_), delivery_time_(delivery_time_) {}

		delivery_info_t(const std::string& firstname_, const std::string& lastname_, int state_id_, const std::string state_, int zip_, const std::string& streetaddress_, const std::string& phone_, const boost::posix_time::ptime& delivery_time_, const std::string& city_): firstname_(firstname_), lastname_(lastname_), state_id_(state_id_), state_(state_), zip_(zip_), streetaddress_(streetaddress_), phone_(phone_), city_(city_), delivery_time_(delivery_time_)  {}

		const std::string& firstname() const { return firstname_; }
		const std::string& lastname() const { return lastname_; }
		const std::string fio() const { return firstname_ + " " + lastname_; }
		int zip() const { return zip_; }
		int state_id() const { return state_id_; }
		const std::string& state() const { return state_; }
		const std::string& streetaddress() const { return streetaddress_; }
		const std::string& phone() const { return phone_; }
		const boost::posix_time::ptime& delivery_time() const { return delivery_time_; }
		const std::string& city() const { return city_; }
		const std::string full_delivery_address() const {
			return streetaddress() + " " + city() + ", " + state() + " " + boost::lexical_cast<std::string>(zip());
		}
		delivery_info_t* clone() const {
			delivery_info_t* tmp = new delivery_info_t(*this);
			return tmp;
		}
	private:
		std::string firstname_;
		std::string lastname_;
		int state_id_;
		std::string state_;
		int zip_;
		std::string streetaddress_;
		std::string phone_;
		std::string city_;
		boost::posix_time::ptime delivery_time_;
	};

	std::auto_ptr<delivery_info_t> make_delivery_info(int delivery_id, int cafe_id = 0);

};

#endif
