/*
 * =====================================================================================
 *
 *       Filename:  oracle_lib.cc
 *
 *    Description:  Simple oracle interface
 *
 *        Version:  1.0
 *        Created:  01.10.2007 13:48:04 MSD
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel G. Koukoushkin (), koukoushkinpg@ksi.ru
 *        Company:  Phlint |COMPANY| K, Moscow, Russia
 *
 * =====================================================================================
 */

#define __RPCNDR_H__
#define __wtypes_h__
#define __unknwn_h__
#define __objidl_h__
#define _OBJBASE_H_
#define __oaidl_h__
#define _OLEAUTO_H_
#define __oleidl_h__
#define _OLE2_H_

#include <oracle_client.hpp>
#include <ksi_exception.hpp>
#include <ksi_common.hpp>

#include "oracle_driver.h"

using namespace std;
//using namespace ksi::oracle_interface;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::gregorian;

using namespace database::oracle;

#define DRIVER_CALL(OPERATION)                  \
  try                                           \
  {                                             \
    OPERATION;                                  \
  }                                             \
  catch (database::oracle::exception& e)        \
  {                                             \
    throw_db_exception(e.code(), e.message());  \
  }

void throw_db_exception(int ora_error, const string& internal_msg)
{
  if(ora_error == 1017 || ora_error == 1918)
    throw ksi_client::db_exception(DB_ERROR_INVALID_CREDENTIALS, internal_msg, ora_error);
  else if(ora_error == 1005)
    throw ksi_client::db_exception(DB_ERROR_EMPTY_PASSWORD, internal_msg, ora_error);
  else if(ora_error == 2292)
    throw ksi_client::db_exception(DB_ERROR_CHILD_RECORDS_EXISTS, internal_msg, ora_error);
  else if(ora_error == 1)
    throw ksi_client::db_exception(DB_ERROR_INVALID_HOSTNAME, internal_msg, ora_error);
  else if(ora_error == 3113 || ora_error == 1089 || ora_error == 3114 ||
      ora_error == 12541)
    throw ksi_client::connection_problem_exception(DB_ERROR_CONNECTION_PROBLEM, internal_msg, ora_error);
  else if (ora_error == 28000)
    throw ksi_client::db_exception(DB_ERROR_ACCOUNT_LOCKED, internal_msg, ora_error);
  else if (ora_error == 20600)
    throw ksi_client::db_exception(DB_ERROR_INTERNAL, internal_msg, ora_error);
  else if (ora_error == 54)
    throw ksi_client::db_exception(DB_ERROR_RESOURCE_BUSY, internal_msg, ora_error);
  else throw ksi_client::db_exception(DB_ERROR_INTERNAL, internal_msg, ora_error);
} 

namespace ksi_client
{
  class oracle_session_impl : public oracle_session_i
  {
  private:
    server __server;
    session __session;
    string __uname;
  public:
    oracle_session_impl()
      : __server()
      , __session(__server)
    {
    };
    virtual void connect(const std::string& login, const std::string& password, const std::string& tns_name, const std::string& sid);
    virtual void disconnect();
    virtual void ksi_transaction_begin(const long num);
    virtual void ksi_transaction_end();
    virtual void commit();
    virtual void rollback();
    virtual const string& username();
    virtual void direct_execute(const std::string& statement);
    virtual std::auto_ptr<oracle_query_i> create_query();
    virtual ~oracle_session_impl()
    {
      disconnect();
    }
  };

  class oracle_query_impl : public oracle_query_i
  {
    query __query;
    map<string, VARIANT> __params;
  public:
    oracle_query_impl(session& sess)
      : __query(sess)
    {
    }

    virtual VARIANT get(const long col_index);
    virtual void set(const std::string& col_name, const VARIANT& value);
    virtual long next();
    virtual void create_statement(const std::string& statement);
    virtual void execute_statement();
    virtual bool out_is_null(const std::string& col_name);
    virtual bool out_is_null(const int col_index);
    virtual ~oracle_query_impl()
    {};
  };
};

// struct async_attach
// {
//   OCIServer* __srvhp;
//   OCIError* errhp;
//   std::string& __tns_name;
//   async_attach(OCIServer* srvhp, OCIError* errhp, std::string& tns_name)
//     : __srvhp(srvhp)
//     , errhp(errhp)
//     , __tns_name(tns_name)
//   {
//   }
//   void operator ()()
//   {
//     __OCI_OP(OCIServerAttach (__srvhp, errhp, (text *)__tns_name.c_str(), __tns_name.length() + 1, OCI_DEFAULT), "");
//   }
// };

long ksi_client::initialize(int argc, char** argv)
{
}

std::auto_ptr<ksi_client::oracle_session_i> ksi_client::create_session(int sesstype)
{
  return std::auto_ptr<oracle_session_i>(dynamic_cast<oracle_session_i*>(new oracle_session_impl()));
}

std::auto_ptr<ksi_client::oracle_query_i> ksi_client::oracle_session_impl::create_query()
{
  return std::auto_ptr<oracle_query_i>(new oracle_query_impl(__session));
}

void ksi_client::oracle_session_impl::connect(const std::string& login, const std::string& passwd, const std::string& tns_name, const std::string& sid)
{
  __uname = login;
  DRIVER_CALL(__server.attach(tns_name, sid, 1521));
  DRIVER_CALL(__session.login(login, passwd));
}

void ksi_client::oracle_session_impl::disconnect()
{
  __session.logout();
  __server.detach();
}

const string& ksi_client::oracle_session_impl::username()
{
  return __uname;
}

long ksi_client::oracle_query_impl::next()
{
  bool ret_fetch;
  ret_fetch = __query.fetch();
  return ret_fetch ? 0 : 1;
}

VARIANT ksi_client::oracle_query_impl::get(const long index)
{
// ::null_t, double, boost::shared_ptr< double >, int, boost::shared_ptr< int >, std::string, boost::shared_ptr< std::string >, boost::posix_time::ptime, boost::shared_ptr< boost::posix_time::ptime >, std::vector< char >, std::vector< unsigned char> 
  datatypes::datatype dt = __query.column_datatype(index);
  switch (dt)
  {
  case datatypes::numeric:
    return __query.get_double(index);
  case datatypes::datetime:
    return __query.get_datetime(index);
  case datatypes::varchar:
    return string(__query.get_cstring(index));
  case datatypes::clob:
    return __query.get_lob(index);
  };
}

void ksi_client::oracle_query_impl::create_statement(const std::string& statement)
{
  __params.clear();
  DRIVER_CALL(__query.prepare(statement));
}

void ksi_client::oracle_query_impl::set(const string& name, const VARIANT& value)
{
  string myname = "";
  if (':' != name[0])
    myname += ":";
  myname += name;
  __params[myname] = value;
}

#define TMP_DATA_VARIANT variant<shared_ptr<int>, shared_ptr<double>, shared_array< char >, shared_ptr< string >, shared_ptr< OCIDate >, __clob_locator, __blob_locator >

void ksi_client::oracle_query_impl::execute_statement()
{
  map<string, VARIANT >::const_iterator iter = __params.begin();
// ::null_t, double, boost::shared_ptr< double >, int, boost::shared_ptr< int >, std::string, boost::shared_ptr< std::string >, boost::posix_time::ptime, boost::shared_ptr< boost::posix_time::ptime >, std::vector< char >, std::vector< unsigned char>
  for (; iter != __params.end(); iter++)
  {
    switch (iter->second.which())
    {
    case 0:
      DRIVER_CALL(__query.set_null(iter->first));
      break;
    case 1:
      DRIVER_CALL(__query.set(iter->first, boost::get<double>(iter->second)));
      break;
    case 2:
      DRIVER_CALL(__query.set(iter->first, boost::get<shared_ptr< double > > (iter->second).get()));
      break;
    case 3:
      DRIVER_CALL(__query.set(iter->first, boost::get<int>(iter->second)));
      break;
    case 4:
      DRIVER_CALL(__query.set(iter->first, boost::get<shared_ptr<int> >(iter->second).get()));
      break;
    case 5:
      DRIVER_CALL(__query.set(iter->first, boost::get<string>(iter->second)));
      break;
    case 7:
      DRIVER_CALL(__query.set(iter->first, boost::get<ptime>(iter->second)));
      break;
    case 8:
      DRIVER_CALL(__query.set(iter->first, boost::get<shared_ptr< ptime > >(iter->second).get()));
      break;
    case 9:
      DRIVER_CALL(__query.set_clob(iter->first
                                   , const_cast<char*>(&boost::get< vector< char > >(iter->second)[0])
                                   , boost::get< vector< char > >(iter->second).size()));
      break;
    case 10:
      DRIVER_CALL(__query.set_blob(iter->first
                                   , const_cast<unsigned char*>(&boost::get< vector< unsigned char > >(iter->second)[0])
                                   , boost::get< vector< unsigned char > >(iter->second).size()));
      break;
    }
  }
  DRIVER_CALL(__query.execute());
}

bool ksi_client::oracle_query_impl::out_is_null(const string& col_name)
{
  return __query.is_null(__query.column_index(col_name));
}

bool ksi_client::oracle_query_impl::out_is_null(const int col_index)
{
  return __query.is_null(col_index);
}

void ksi_client::oracle_session_impl::ksi_transaction_begin(long num)
{
}

void ksi_client::oracle_session_impl::ksi_transaction_end()
{
}

void ksi_client::oracle_session_impl::direct_execute(const std::string& stmt)
{
  query q(__session);
  DRIVER_CALL(q.prepare(stmt));
  DRIVER_CALL(q.execute());
}

void ksi_client::oracle_session_impl::commit()
{
  __session.commit();
}

void ksi_client::oracle_session_impl::rollback()
{
  __session.rollback();
}

