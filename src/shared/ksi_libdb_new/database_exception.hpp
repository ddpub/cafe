#ifndef __DATABASE_EXCEPTION_H__
#define __DATABASE_EXCEPTION_H__

#include "../oracle_client/ksi_exception.hpp"

enum __db_msgcodes
{
  DB_ERROR_LOGICAL = -1,
  DB_ERROR_INTERNAL = 0,
  DB_ERROR_INVALID_CREDENTIALS = 1001,
  DB_ERROR_EMPTY_PASSWORD,
  DB_ERROR_CHILD_RECORDS_EXISTS,
  DB_ERROR_INVALID_HOSTNAME,
  DB_ERROR_CONNECTION_PROBLEM,
  DB_ERROR_ACCOUNT_LOCKED,
  DB_ERROR_RESOURCE_BUSY
};
  
namespace database
{ 
  class exception : public ksi_client::ksi_exception
  {
  public:
    exception(int msg_id, const std::string& msg, long code = 0) : ksi_exception(msg_id, msg, code) {};
  };

  class connection_problem_exception : public exception
  {
  public:
    connection_problem_exception(int msg_id, const std::string& msg, long code = 0) : exception(msg_id, msg, code) {};
  };
};


#endif   /* ----- #ifndef ORACLE_CLIENT_LIB_INC  ----- */
