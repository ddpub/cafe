#ifndef POSTGRES_HH
#define POSTGRES_HH

#include <oracle_client.hpp>
#include <map>
#include <boost/variant.hpp>
//#include <libpq-fe.h>
#include <set>

namespace ksi_client
{
  class postgres_session_impl : public oracle_session_i
  {
    friend class postgres_query_impl;
  private:
    PGconn* __conn;
    PGresult *__res;
    int __err_code;
    std::string __err_str;
    std::string __login;

  public:
    postgres_session_impl() : __conn(NULL)
    {
      
    };
    virtual void connect(const std::string& login, const std::string& password, const std::string& tns_name); 
    virtual void disconnect();
    virtual void commit();
    virtual void rollback();
    virtual const std::string& username() {return __login; };
    virtual void direct_execute(const std::string& statement);
    virtual std::auto_ptr<oracle_query_i> create_query();
    virtual ~postgres_session_impl()
    {
    }
  };

  class postgres_query_impl : public oracle_query_i
  {
    std::map< int, VARIANT > __params;
    std::map< std::string, VARIANT > __out_params;
    std::map< int, std::string > __assoc_map;
    PGconn* __conn;
    std::string __statement;
    PGresult *__res;
    int __err_code;
    std::string __err_str;
    std::vector< std::vector<VARIANT > >::const_iterator __fetch_iter;
    bool __before_first;
    std::vector< std::vector<VARIANT > > __data;
    std::vector< std::set< int > > __nulls;
    std::vector< std::set< int > >::const_iterator __nulls_iter;
    std::set< std::string > __out_nulls;
    std::map< std::string, int > __ora_post_map;

    
  public:
    postgres_query_impl(PGconn* conn)
      : __conn(conn)
    {

    }

    postgres_query_impl(const postgres_session_impl& sess)
      : __conn(sess.__conn)
    {
    }

    virtual VARIANT get(const long col_index);
    virtual void set(const std::string& col_name, const VARIANT& value);
    virtual long next();
    virtual void create_statement(const std::string& statement);
    virtual void execute_statement();
    virtual bool out_is_null(const std::string& col_name);
    virtual bool out_is_null(const int col_index);
    virtual ~postgres_query_impl()
    {  };
  };
};
#endif
