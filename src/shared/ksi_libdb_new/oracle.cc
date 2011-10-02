/*
 * =====================================================================================
 *
 *       Filename:  oracle.cc
 *
 *    Description:  Oracle database client implementation
 *
 *        Version:  1.0
 *        Created:  28.09.2008 19:54:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel G. Koukoushkin (mn), k.pavel.g@gmail.com
 *        Company:  Phlint and K, Moscow, Russia
 *
 * =====================================================================================
 */

#include "oracle.hh"

using namespace database::oracle;
using namespace database::oracle::__oci_adapter;


void session::connect(const std::string& login,
    const std::string& password,
    const std::string& host,
    const std::string& database_name,
    int port)
{
  __adapter.connect(host, database_name, port);
  __adapter.login(login, password);
  __username = login;
  execute("ALTER SESSION SET NLS_DATE_FORMAT='DD-MM-YYYY HH24:MI:SS'");
}

void session::commit()
{
  __adapter.commit();
}

void session::rollback()
{
  __adapter.rollback();
}

unsigned int session::execute(const std::string& statement)
{
  statement_adapter st_adapter(__adapter.env(), __adapter.err(), __adapter.ctx()); 
  st_adapter.prepare(statement);
  return st_adapter.execute();
}


statement::statement(database::oracle::__oci_adapter::oci_adapter& session_adapter)
  : __adapter(session_adapter.env(), session_adapter.err(),
      session_adapter.ctx())
{
}

void statement::prepare(const std::string& statement_string)
{
  __adapter.clear();
  __statement = statement_string;
  __adapter.prepare(__statement);
  __adapter.describe();
}

void statement::set_null(unsigned int position)
{
  __adapter.set_null(position);
}

void statement::set_null(const std::string& alias)
{
  __adapter.set_null(alias);
}

unsigned int statement::execute()
{
  __adapter.clear();
  return __adapter.execute();
}

bool statement::next()
{
  return __adapter.next();
}

const boost::shared_array<char> statement::get_lob_data(unsigned int position,
    size_t& size)
{
}
