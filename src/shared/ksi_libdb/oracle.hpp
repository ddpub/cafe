/*
 * =====================================================================================
 * 
 *       Filename:  oracle_lib.hpp
 * 
 *    Description:  Simple oracle interface
 * 
 *        Version:  1.0
 *        Created:  01.10.2007 13:50:27 MSD
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Pavel G. Koukoushkin (), koukoushkinpg@ksi.ru
 *        Company:  Phlint |COMPANY| K, Moscow, Russia
 * 
 * =====================================================================================
 */

#ifndef  ORACLE_LIB_INC
#define  ORACLE_LIB_INC

#include <vector>
#include <string>
#include <iostream>
#include <map>
//#include <memory>
#define OTL_ORA10G
#undef OTL_UNICODE
#define OTL_STLPORT
#define OTL_STREAM_READ_ITERATOR_ON
#define OTL_STREAM_POOLING_ON
#define OTL_ORA_OCI_ENV_CREATE
#include <otlv4.h>
#include <time.h>
#include <boost/variant.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

struct null_t {
};

#define VARIANT boost::variant<null_t, int, std::string, double, boost::posix_time::ptime>
#define SESSION sessions[session_id]

/*namespace ksi
{
namespace oracle_interface
{*/

struct __session_t
{
  int __col_count;
  std::auto_ptr<otl_connect> connection;
  otl_connect* __valid_connection;
  std::auto_ptr<otl_stream> __stream;
  std::auto_ptr<otl_stream_read_iterator<otl_stream, otl_exception, otl_lob_stream> > __iterator;
  std::map<std::string, VARIANT > __params;
  std::map<int, std::vector<VARIANT > > __data;
  std::map<int, std::vector<VARIANT > >::const_iterator __fetch_iter;
  std::string __login;
  std::string __passwd;
  std::string __message;
  time_t last_time;
  bool before_first;

  __session_t(const __session_t& session)
  {
  }
  
  __session_t() : __valid_connection(0), before_first(true) {};
};


std::string connect(const std::string& login, const std::string& passwd, const std::string& tns_name);
std::string get_last_error(const std::string& session_id);
long disconnect(const std::string& session_id);
long next(const std::string& session_id);
long get_param_type(const std::string& session_id, const std::string& name, int& type);
long get(const std::string& session_id, const long index, VARIANT& value);
long set(const std::string& session_id, const std::string& name, const VARIANT& value);
long create_statement(const std::string& session_id, const std::string& statement);
//long set_any(const std::string& session_id, const CORBA::Any& value);
long execute_statement(const std::string& session_id);
long ksi_transaction_begin(const std::string& session_id, const int num);
long ksi_transaction_end(const std::string& session_id);
long direct_execute(const std::string& session_id, const std::string& statement);
long commit(const std::string& session_id);
long rollback(const std::string& session_id);



/*};
};*/

#endif   /* ----- #ifndef ORACLE_LIB_INC  ----- */

