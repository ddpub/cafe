/*
 * =====================================================================================
 * 
 *       Filename:  pos.hpp
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  24.12.2007 13:53:58 MSK
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Pavel G. Koukoushkin (), koukoushkinpg@ksi.ru
 *        Company:  Phlint and K, Moscow, Russia
 * 
 * =====================================================================================
 */

#ifndef  POS_INC
#define  POS_INC

#include <string>
#include <memory>
#include <oracle_client/ksi_exception.hpp>

namespace ksi_client
{
  class pos_exception : public ksi_exception
  {
  public:
    pos_exception(int msg_id, const std::string& msg, long code = 0) : ksi_exception(msg_id, msg, code) {};
    virtual pos_exception* clone()
      {
        return new pos_exception(__msg_id, text, __code);
      }
    virtual ~pos_exception() throw ()
      {
      }
  };

  class logical_pos_exception : public pos_exception
  {
  public:
    logical_pos_exception(int msg_id, const std::string& msg, long code = 0) : pos_exception(msg_id, msg, code) {};
    virtual logical_pos_exception* clone()
      {
        return new logical_pos_exception(__msg_id, text, __code);
      }
    virtual ~logical_pos_exception() throw ()
      {
      }
  };

  class fatal_pos_exception : public pos_exception
  {
  public:
    fatal_pos_exception(int msg_id, const std::string& msg, long code = 0) : pos_exception(msg_id, msg, code)
      {
      };
    virtual fatal_pos_exception* clone()
      {
        return new fatal_pos_exception(__msg_id, text, __code);
      }
    virtual ~fatal_pos_exception() throw ()
      {
      }
  };

  class user_pos_i
  {
  public:
    virtual int credit_auth(
        double amt,
        double gratuity_amount,
        const std::string& card_nbr,
        const int exp_month,
        const int exp_year,
        const std::string& cvv2 = "",
        const bool card_present = false,
        const bool reader_present = false) = 0;
    virtual int credit_auth(
        double amt,
        double gratuity_amount,
        const std::string& track_data) = 0;
    virtual int credit_add_to_batch(const int txn_id, double amt = 0, double amt_info = 0) = 0;
    virtual int credit_void(const int tnx_id) = 0;
    virtual int credit_return(double total, const std::string& track_data) = 0;
    virtual int credit_return(double total, const std::string& cc_num, int exp_month, int exp_year, const std::string& cvv2, bool card_present = true, bool reader_present = true) = 0;
    virtual int test_credentials() = 0;
    virtual std::string get_card_holder_first_name(int txn_id) = 0;
    virtual std::string get_card_holder_last_name(int txn_id) = 0;
    virtual std::string get_card_type(int txn_id) = 0;
    virtual const std::string& get_auth_code(int txn_id) = 0;
    virtual int get_current_batch_id() = 0;
    virtual bool get_do_report() = 0;
    virtual void set_do_report(bool do_report) = 0;
  };
  std::auto_ptr<user_pos_i> create_user_pos(const int license_id,
                                                    const int site_id, 
                                                    const int device_id,
                                                    const std::string& username,
                                                    const std::string& password);

};

#include <boost/function.hpp>

void cc_processing(int*, ksi_client::pos_exception**, boost::function<int ()>);


#endif   /* ----- #ifndef POS_INC  ----- */

