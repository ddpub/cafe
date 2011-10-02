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
#define OTL_ORA10G
#undef OTL_UNICODE
#define OTL_STLPORT
#define OTL_STREAM_READ_ITERATOR_ON
#define OTL_STREAM_POOLING_ON
#define OTL_ORA_OCI_ENV_CREATE
//#define SLSHORTNAME
#include <oci.h>
#include <time.h>
#include <stdexcept>
#include <sstream>
#include <set>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
//#include <boost/python.hpp>
//#include "postgres.hh"

using namespace std;
//using namespace ksi::oracle_interface;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::gregorian;

bool ksi_client::inited = false;
#define SESSION sessions[session_id]
#define THROW(OP, MSG) \
err_str = boost::lexical_cast<std::string>("ERROR in ");\
err_str += boost::lexical_cast<std::string>(OP);\
err_str += boost::lexical_cast<std::string>(". At oracle.cc line ");\
err_str += boost::lexical_cast<std::string>(__LINE__);\
err_str += boost::lexical_cast<std::string>(". With code: ");\
err_str += boost::lexical_cast<std::string>(MSG);\
err_str += boost::lexical_cast<std::string>(". With message: ");\
checkerr(errhp, MSG);\
throw ksi_client::ksi_exception(err_str);

#define __OCI_OP(OP, MSG) retval = OP; \
  if (retval != OCI_SUCCESS) \
  {\
err_code = 0;\
err_str = boost::lexical_cast<std::string>("ERROR in ");\
err_str += boost::lexical_cast<std::string>(#OP);\
err_str += boost::lexical_cast<std::string>(". At oracle.cc line ");\
err_str += boost::lexical_cast<std::string>(__LINE__);\
err_str += boost::lexical_cast<std::string>(". With code: ");\
err_str += boost::lexical_cast<std::string>(retval);\
err_str += std::string(". With message: ");\
checkerr(errhp, retval);\
throw_db_exception(err_code, err_str); \
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
					ora_error == 12541 || ora_error == 12545 || ora_error == 24327 ||
					ora_error == 28 || ora_error == 3135 || ora_error == 24550 || ora_error == 12571 || ora_error == 1012)
    throw ksi_client::connection_problem_exception(DB_ERROR_CONNECTION_PROBLEM, internal_msg, ora_error);
  else if (ora_error == 28000)
    throw ksi_client::db_exception(DB_ERROR_ACCOUNT_LOCKED, internal_msg, ora_error);
  else if (ora_error == 20600)
    throw ksi_client::db_exception(DB_ERROR_INTERNAL, internal_msg, ora_error);
  else if (ora_error == 54)
    throw ksi_client::db_exception(DB_ERROR_RESOURCE_BUSY, internal_msg, ora_error);
  else throw ksi_client::db_exception(DB_ERROR_INTERNAL, internal_msg, ora_error);
} 


string session_id = "";
OCIEnv *oci_env;
string err_str;
long err_code;
stringstream err_ss(err_str);
sword retval;

std::auto_ptr<char> buf;

void checkerr(OCIError *errhp, sword status)
{
  text errbuf[512];
  sb4 errcode = 0;

  switch (status)
  {
  case OCI_SUCCESS:
    break;
  case OCI_SUCCESS_WITH_INFO:
    err_str += boost::lexical_cast<std::string>("Error - OCI_SUCCESS_WITH_INFO");
    break;
  case OCI_NEED_DATA:
    err_str += boost::lexical_cast<std::string>("Error - OCI_NEED_DATA\n");
    break;
  case OCI_NO_DATA:
    err_str += boost::lexical_cast<std::string>("Error - OCI_NODATA\n");
    break;
  case OCI_ERROR:
    (void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
                        errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    err_str += std::string("Error - ");
    err_str += boost::lexical_cast<string>(errcode);
    err_str += " ";
    err_str += std::string((const char*) errbuf);
    err_code = errcode;
    break;
  case OCI_INVALID_HANDLE:
    err_str += boost::lexical_cast<std::string>("Error - OCI_INVALID_HANDLE\n");
    break;
  case OCI_STILL_EXECUTING:
    err_str += boost::lexical_cast<std::string>("Error - OCI_STILL_EXECUTE\n");
    break;
  case OCI_CONTINUE:
    err_str += boost::lexical_cast<std::string>("Error - OCI_CONTINUE\n");
    break;
  default:
    break;
  }
}

namespace ksi_client
{
/*  struct __session_t
  {
    std::auto_ptr<otl_connect> connection;
    otl_connect* __valid_connection;
    std::auto_ptr<otl_stream_read_iterator<otl_stream, otl_exception, otl_lob_stream> > __iterator;
    std::string __login;
    std::string __passwd;
    std::string __message;
    time_t last_time;

    __session_t(const __session_t& session)
    {
    }
    
    __session_t() : __valid_connection(0), before_first(true) {};
  };*/
  

  struct __session_check_t
  {
    __session_check_t(const string& session_id, bool online = true)
    {
    };

    ~__session_check_t()
    {
    };
  };
  
  class oracle_session_impl : public oracle_session_i
  {
  private:
    OCIEnv     *envhp;    /* the environment handle */
    OCIServer  *srvhp;    /* the server handle */
    OCIError   *errhp;    /* the error handle */
    OCISession *usrhp;    /* user session handle */
    OCISvcCtx  *svchp;    /* the  service handle */
    string __uname;
  public:
    oracle_session_impl() : envhp(NULL), srvhp(NULL), errhp(NULL), usrhp(NULL), svchp(NULL)
    {
      
    };
    virtual void connect(const std::string& login, const std::string& password, const std::string& tns_name, const std::string& sid, unsigned short port);
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
    std::map<int, std::vector<VARIANT > >::const_iterator __fetch_iter;
    std::map<int, std::vector<VARIANT > > __data;
    std::set<string> __out_nulls;
    std::vector< std::set<int> > __nulls;
    bool before_first;
    std::map<std::string, VARIANT > __params;
    int __col_count;
    OCIEnv *__envhp;
    OCIError *errhp;
    OCIStmt *__stmthp;
    OCISvcCtx  *__svchp;    /* the  service handle */
    OCIServer *__srvhp;
  std::vector< std::set<int> >::const_iterator __nulls_iter;

  public:
    oracle_query_impl(OCIEnv* const envhp, OCISvcCtx * const svchp, OCIError* const errhp, OCIServer* srvhp)
      : __envhp(envhp)
      , errhp(errhp)
      , __svchp(svchp)
      , __srvhp(srvhp)
    {
      __OCI_OP(OCIHandleAlloc((dvoid*) __envhp,(dvoid **)&__stmthp, OCI_HTYPE_STMT, 0, NULL), "");
    }

    virtual VARIANT get(const long col_index);
    virtual void set(const std::string& col_name, const VARIANT& value);
    virtual long next();
    virtual void create_statement(const std::string& statement);
    virtual void execute_statement();
    virtual bool out_is_null(const std::string& col_name);
    virtual bool out_is_null(const int col_index);
    virtual ~oracle_query_impl()
    {  OCIHandleFree(__stmthp, OCI_HTYPE_STMT); };
  };
};

struct async_attach
{
  OCIServer* __srvhp;
  OCIError* errhp;
  std::string& __tns_name;
  async_attach(OCIServer* srvhp, OCIError* errhp, std::string& tns_name)
	  : __srvhp(srvhp)
	  , errhp(errhp)
	  , __tns_name(tns_name)
  {
  }
  void operator ()(string& result)
  {
	  try
	  {
		  __OCI_OP(OCIServerAttach (__srvhp, errhp, (text *)__tns_name.c_str(), __tns_name.length() + 1, OCI_DEFAULT), "");
	  }
	  catch (std::exception& e)
	  {
			result = e.what();
	  }
		catch (...)
		{
			result = "Attach failed";
		}
  }
};

long ksi_client::initialize(int argc, char** argv)
{
    if (!inited)
    {
     inited = true;
    }
    return 0;
}

std::auto_ptr<ksi_client::oracle_session_i> ksi_client::create_session(int sesstype)
{
  return std::auto_ptr<oracle_session_i>(dynamic_cast<oracle_session_i*>(new oracle_session_impl()));
}

std::auto_ptr<ksi_client::oracle_query_i> ksi_client::oracle_session_impl::create_query()
{
  return std::auto_ptr<oracle_query_i>(new oracle_query_impl(envhp, svchp, errhp, srvhp));
}

void ksi_client::oracle_session_impl::connect(const std::string& login, const std::string& passwd, const std::string& tns_name, const std::string& sid, unsigned short port)
{
  __uname = login;

  __OCI_OP(OCIEnvCreate((OCIEnv **)&envhp,
     (ub4)OCI_THREADED,
     (dvoid *)0, (dvoid * (*)(dvoid *, size_t))0,
     (dvoid * (*)(dvoid *, dvoid *, size_t))0,
     (void (*)(dvoid *, dvoid *))0,
     (size_t)0, (dvoid **)0), "Error during initializing OCI environment");
  
  /* allocate a server handle */
  __OCI_OP(OCIHandleAlloc ((dvoid *)envhp, (dvoid **)&srvhp, OCI_HTYPE_SERVER, 0, (dvoid **) 0), "");

  /* allocate an error handle */
  __OCI_OP(OCIHandleAlloc ((dvoid *)envhp, (dvoid **)&errhp, OCI_HTYPE_ERROR, 0, (dvoid **) 0), "");

  string tmp_tns_name = std::string("(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=")
    + tns_name
    + std::string(")(PORT=")
    + boost::lexical_cast<std::string>(port)
    + std::string(")))(CONNECT_DATA=(SERVICE_NAME=") + sid + std::string(")))");

  #ifndef _DONT_USE_THREADS_
  string connected;
  try
  {
	async_attach a_attach(srvhp, errhp, tmp_tns_name);
    boost::thread  attach_thread(boost::bind<void>(a_attach, connected));
    attach_thread.timed_join(boost::posix_time::seconds(10));
	attach_thread.interrupt();
  }
  catch (boost::thread_interrupted&)
  {
    throw
      ksi_client::connection_problem_exception(DB_ERROR_CONNECTION_PROBLEM,
          "Attach timed out", 0);
  }
  if (connected.length() > 0)
	  throw
		  ksi_client::connection_problem_exception(DB_ERROR_CONNECTION_PROBLEM, connected, 0);
  #else
  __OCI_OP(OCIServerAttach (srvhp, errhp, (text *)tmp_tns_name.c_str(), tmp_tns_name.length() + 1, OCI_DEFAULT), "");
  #endif



  /* create a server context */
//  __OCI_OP(OCIServerAttach (srvhp, errhp, (text *)tmp_tns_name.c_str(), tmp_tns_name.length() + 1, OCI_DEFAULT), "");

  /* allocate a service handle */
  __OCI_OP(OCIHandleAlloc ((dvoid *)envhp, (dvoid **)&svchp, OCI_HTYPE_SVCCTX, 0, (dvoid **) 0), "");

  /* set the server attribute in the service context handle*/
  __OCI_OP(OCIAttrSet ((dvoid *)svchp, OCI_HTYPE_SVCCTX, (dvoid *)srvhp, (ub4) 0, OCI_ATTR_SERVER, errhp), "");

  /* allocate a user session handle */
  __OCI_OP(OCIHandleAlloc ((dvoid *)envhp, (dvoid **)&usrhp, OCI_HTYPE_SESSION, 0, (dvoid **) 0), "");

  /* set user name attribute in user session handle */
  __OCI_OP(OCIAttrSet ((dvoid *)usrhp, OCI_HTYPE_SESSION, (dvoid *)login.c_str(), (ub4)strlen(login.c_str()), OCI_ATTR_USERNAME, errhp), "");

  /* set password attribute in user session handle */
  __OCI_OP(OCIAttrSet ((dvoid *)usrhp, OCI_HTYPE_SESSION, (dvoid *)passwd.c_str(), (ub4)strlen(passwd.c_str()), OCI_ATTR_PASSWORD, errhp), "");

  __OCI_OP(OCISessionBegin (svchp, errhp, usrhp, OCI_CRED_RDBMS, OCI_DEFAULT), "");

  /* set the user session attribute in the service context handle*/
  __OCI_OP(OCIAttrSet ((dvoid *)svchp, OCI_HTYPE_SVCCTX, (dvoid *)usrhp, (ub4) 0, OCI_ATTR_SESSION, errhp), "");
}

void ksi_client::oracle_session_impl::disconnect()
{
  OCITransRollback(svchp, errhp, OCI_DEFAULT);
  OCISessionEnd(svchp, errhp, usrhp, OCI_DEFAULT);
  OCIServerDetach(srvhp, errhp, OCI_DEFAULT);
  if (usrhp)
  {
    OCIHandleFree(usrhp, OCI_HTYPE_SESSION);
    usrhp = NULL;
  }
  if (svchp)
  {
    OCIHandleFree(svchp, OCI_HTYPE_SVCCTX);
    svchp = NULL;
  }
  if (errhp)
  {
    OCIHandleFree(errhp, OCI_HTYPE_ERROR);
    errhp = NULL;
  }
  if (srvhp)
  {
    OCIHandleFree(srvhp, OCI_HTYPE_SERVER);
    srvhp = NULL;
  }
  if (envhp)
  {
    OCIHandleFree(envhp, OCI_HTYPE_ENV);
    envhp = NULL;
  }
}

const string& ksi_client::oracle_session_impl::username()
{
/*  ub4 size;
  const char* data;
  __OCI_OP(OCIAttrGet(usrhp, OCI_HTYPE_SESSION, (void*) &data, &size, OCI_ATTR_USERNAME, errhp), "");
  string s(data, size);
  return s;*/
  return __uname;
}

long ksi_client::oracle_query_impl::next()
{
  if (before_first)
  {
    if (__fetch_iter == __data.end())
      return 1;
    if (__fetch_iter != __data.begin())
      throw ksi_client::db_exception(DB_ERROR_LOGICAL, "Execute statement before using next()", 0);
    before_first = false;
    return 0;
  }
  if (__fetch_iter == __data.end())
    throw ksi_client::db_exception(DB_ERROR_LOGICAL, "End of dataset reached", 0);
  __fetch_iter++;
  ++ __nulls_iter;
  if (__fetch_iter == __data.end())
    return 1;
  return 0;
}

VARIANT ksi_client::oracle_query_impl::get(const long index)
{
  if (long(__fetch_iter->second.size()) <= index)
    throw db_exception(DB_ERROR_LOGICAL, string("at oracle_query::get. Index is out of range"), 0);
  if (__fetch_iter == __data.end())
    throw db_exception(DB_ERROR_LOGICAL, string("at oracle_query::get."
          "You try to call \"get\" while next reaches "
          "end of dataset"), 0);
  
  return __fetch_iter->second[static_cast<int>(index)];
}

void ksi_client::oracle_query_impl::create_statement(const std::string& statement)
{
  __OCI_OP(OCIStmtPrepare(__stmthp, errhp, (text*) statement.c_str(), statement.length(), OCI_NTV_SYNTAX, OCI_DEFAULT), "");
  __params.clear();
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

struct __context_t
{
  sb4 ind;
  ub2 rcode;
  ub4 alen;
  int type;
  void* retval;
  const char* name;
};

static sb4 cbf_get_data(dvoid *ctxp, OCIBind *bindp, ub4 iter, ub4 index,
                         dvoid **bufpp, ub4 **alenp, ub1 *piecep,
                         dvoid **indpp, ub2 **rcodepp)
{
  __context_t* ctx = (__context_t*) ctxp;
  *bufpp = ctx->retval;
/*  try
  {
    if (0 == *bufpp)
      *bufpp = boost::get< shared_ptr< int > >(ctx->retval).get();
  }
  catch (bad_get&)
  {
  }*/
  
  *piecep = OCI_ONE_PIECE;

  /* provide address of the storage where the indicator will be returned */
  *indpp = (dvoid *) &ctx->ind;


  /* provide address of the storage where the return code  will be returned */
  *rcodepp = &ctx->rcode;

  /*
   * provide address of the storage where the actual length  will be
   * returned
   */
  *alenp = &ctx->alen;

  return OCI_CONTINUE;
}

sb4 cbk_read_lob(
  dvoid       *ctxp,
  CONST dvoid *bufxp,
  oraub8      len,
  ub1         piece,
  dvoid       **changed_bufpp,
  oraub8      *changed_lenp
)
{ 
    static ub4 piece_count = 0; 
    piece_count++; 

    switch (piece)
   {
      case OCI_LAST_PIECE:     /*--- buffer processing code goes here ---*/ 
        //(void) printf("callback read the %d th piece\n\n", piece_count);
          piece_count = 0;
          break;
      case OCI_FIRST_PIECE:   /*--- buffer processing code goes here ---*/ 
        //(void) printf("callback read the %d th piece\n", piece_count);
          /* --Optional code to set changed_bufpp and changed_lenp if the
              buffer needs to be changed dynamically --*/
          break;
      case OCI_NEXT_PIECE:   /*--- buffer processing code goes here ---*/
        //(void) printf("callback read the %d th piece\n", piece_count);
         /* --Optional code to set changed_bufpp and changed_lenp if the
             buffer needs to be changed dynamically --*/
          break;
      default:
        //(void) printf("callback read error: unkown piece = %d.\n", piece);
           return OCI_ERROR;
     } 
    return OCI_CONTINUE;
}


static sb4 cbf_no_data(dvoid *ctxp, OCIBind *bindp, ub4 iter, ub4 index,
                      dvoid **bufpp, ub4 *alenpp, ub1 *piecep, dvoid **indpp)
{
  /*
   * This is a dummy input callback function that provides input data
   * for the bind variables in the RETURNING clause.
   */
  *bufpp = (dvoid *) 0;
  *alenpp = 0;
  *indpp = (dvoid *) 0;
  *piecep = OCI_ONE_PIECE;

  return OCI_CONTINUE;
}

struct __clob_locator
{
  OCILobLocator* loc;
};

struct __blob_locator
{
  OCILobLocator* loc;
};

void ksi_client::oracle_query_impl::execute_statement()
{
  map<string, VARIANT >::const_iterator iter = __params.begin();
  std::vector<__context_t*> ctxs;
  vector< shared_ptr< sb2 > > inds;
  //OCINumber* nbr = (OCINumber*) ctx.retval;
  ub4 stmt_type = 0;
  __OCI_OP(OCIAttrGet(__stmthp, OCI_HTYPE_STMT, &stmt_type, 0, OCI_ATTR_STMT_TYPE, errhp), "");
  vector<TMP_DATA_VARIANT > tmp_data;
  vector< shared_ptr< OCIDate > > odt_binds;
  OCIBind* bindhp;
  __out_nulls.clear();

  OCILobLocator* param_lob;
  for (; iter != __params.end(); iter++)
  {
    try
    {
      boost::get< null_t >(iter->second);
      shared_ptr< sb2 > ind (new sb2(-1));
      inds.push_back(ind);
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), 
          iter->first.size(), (dvoid *) 0,
          (sword) 0, SQLT_CHR, (dvoid *) ind.get(),
          (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT), "");
      continue;
    }
    catch (bad_get&)
    {
    }

    try
    {
      const int& val = boost::get<int>(iter->second);
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), 
          iter->first.size(), (dvoid *) &val,
          (sword) sizeof(int), SQLT_INT, (dvoid *) 0,
          (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT), "");
      continue;
    }
    catch (bad_get&)
    {
    }
    try
    {
      shared_ptr<int> val = boost::get<shared_ptr<int> >(iter->second);
      ctxs.push_back(new __context_t());
      ctxs.back()->alen = sizeof(int);
      ctxs.back()->retval = new int;
      ctxs.back()->type = 0;
      ctxs.back()->name = iter->first.c_str();
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), 
          iter->first.size(), (dvoid *) 0,
          (sword) sizeof(int), SQLT_INT, (dvoid *) 0,
          (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DATA_AT_EXEC), "");
      __OCI_OP(OCIBindDynamic(bindhp, errhp, (dvoid*) NULL, cbf_no_data, (dvoid*) ctxs.back(), cbf_get_data), "");
      continue;
     }
    catch (bad_get&)
    {
    }

    try
    {
      const double& val = boost::get<double>(iter->second);
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), 
          iter->first.size(), (dvoid *) &val,
          (sword) sizeof(double), SQLT_FLT, (dvoid *) 0,
          (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT), "");
      continue;
    }
    catch (bad_get&)
    {
    }
    try
    {
      shared_ptr< double > val (boost::get< shared_ptr< double > >(iter->second));
      ctxs.push_back(new __context_t());
      ctxs.back()->alen = sizeof(double);
      ctxs.back()->retval = new double;
      ctxs.back()->type = 1;
      ctxs.back()->name = iter->first.c_str();
/*      ctxs1.retval = new double;
      ctxs1.alen = sizeof(double);*/
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), 
          iter->first.size(), (dvoid *) 0,
          (sword) sizeof(double), SQLT_FLT, (dvoid *) 0,
          (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DATA_AT_EXEC), "");
      __OCI_OP(OCIBindDynamic(bindhp, errhp, (dvoid*) NULL, cbf_no_data, (dvoid*) (ctxs.back()), cbf_get_data), "");
      continue;
    }
    catch (bad_get&)
    {
    }
   
    try
    {
      const vector< char >& val = boost::get< vector< char > >(iter->second);
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), iter->first.size(), (dvoid*) const_cast< char* >(&(val[0])), val.size(), SQLT_LNG, (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT), "");
      continue;
    }
    catch (bad_get&)
    {
    }
    
    try
    {
      const vector< unsigned char >& val = boost::get< vector< unsigned char > >(iter->second);
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), iter->first.size(), (dvoid*) const_cast< unsigned char* >(&(val[0])), val.size(), SQLT_LBI, (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT), "");
      continue;
    }
    catch (bad_get&)
    {
    }
    
    try
    {
      const string& val = boost::get<string>(iter->second);
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), 
          iter->first.size(), (dvoid *) val.c_str(),
          (sword) val.size() + 1, SQLT_STR, (dvoid *) 0,
          (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT), "");
      continue;
    }
    catch (bad_get&)
    {
    }
    try
    {
      shared_ptr< string > val = boost::get<shared_ptr< string > >(iter->second);
      ctxs.push_back(new __context_t());
      ctxs.back()->alen = 4000;
      ctxs.back()->retval = new char[4000];
      ctxs.back()->type = 2;
      ctxs.back()->name = iter->first.c_str();
/*      ctxs2.alen = 4000;
      ctxs2.retval = new char[4000];
      ctxs2.type = 1;*/
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), 
          iter->first.size(), (dvoid *) 0,
          (sword) 4000, SQLT_STR, (dvoid *) 0,
          (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DATA_AT_EXEC), "");
      __OCI_OP(OCIBindDynamic(bindhp, errhp, (dvoid*) NULL, cbf_no_data, (dvoid*) ctxs.back(), cbf_get_data), "");
      continue;
    }
    catch (bad_get&)
    {
    }
    
    try
    {
      const ptime& val = boost::get<ptime>(iter->second);
      shared_ptr< OCIDate > dt (new OCIDate);
      odt_binds.push_back(dt);
      OCIDateSetDate(dt.get(), val.date().year(), val.date().month(), val.date().day());
      OCIDateSetTime(dt.get(), val.time_of_day().hours(), val.time_of_day().minutes(), val.time_of_day().seconds());
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), 
          iter->first.size(), (dvoid *) dt.get(),
          (sword) sizeof(OCIDate), SQLT_ODT, (dvoid *) 0,
          (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT), "");
      continue;
    }
    catch (bad_get&)
    {
    }
    try
    {
      shared_ptr< ptime > val = boost::get<shared_ptr< ptime > >(iter->second);
      ctxs.push_back(new __context_t());
      ctxs.back()->alen = sizeof(OCIDate);
      ctxs.back()->retval = new OCIDate;
      ctxs.back()->type = 3;
      ctxs.back()->name = iter->first.c_str();
      __OCI_OP(OCIBindByName(__stmthp, &bindhp, errhp, (text *) iter->first.c_str(), 
          iter->first.size(), (dvoid *) 0,
          (sword) sizeof(OCIDate), SQLT_ODT, (dvoid *) 0,
          (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DATA_AT_EXEC), "");
      __OCI_OP(OCIBindDynamic(bindhp, errhp, (dvoid*) NULL, cbf_no_data, (dvoid*) ctxs.back(), cbf_get_data), "");
      continue;
    }
    catch (bad_get&)
    {
    }
  }


  __OCI_OP(OCIStmtExecute(__svchp, __stmthp, errhp, OCI_STMT_SELECT == stmt_type ? 0 : 1, 0, (OCISnapshot *) 0, (OCISnapshot *) 0, OCI_DEFAULT), "");

  std::vector<__context_t*>::iterator ctx_iter = ctxs.begin();
  for (; ctx_iter != ctxs.end(); ctx_iter++)
  {
    __context_t *ctx = *ctx_iter;
    if (65535 == ctx->ind)
    {
      __out_nulls.insert(string(ctx->name));
      continue;
    }
    switch (ctx->type)
    {
    case 0:
      *boost::get< shared_ptr< int > >(__params[string(ctx->name)]) = *(int*)(ctx->retval);
      delete (int*)ctx->retval;
      break;
    case 1:
      *boost::get< shared_ptr< double > >(__params[string(ctx->name)]) = *(double*)(ctx->retval);
      delete (double*)ctx->retval;
      break;
    case 2:
      *boost::get< shared_ptr< string > >(__params[string(ctx->name)]) = string((char*)(ctx->retval));
      delete[] (char*) ctx->retval;
      break;
    case 3:
      OCIDate *dt_val = ((OCIDate*)(ctx->retval));
      sb2 year;
      ub1 month;
      ub1 day;
      OCIDateGetDate(dt_val, &year, &month, &day);
      ub1 hour;
      ub1 min;
      ub1 sec;
      OCIDateGetTime(dt_val, &hour, &min, &sec);
      *boost::get< shared_ptr< ptime > >(__params[string(ctx->name)]) = ptime(date(year, month, day), time_duration(hour, min, sec));
      delete dt_val;
      break;
    }
    delete ctx;
  }

//  __params.clear();

  if (OCI_STMT_SELECT != stmt_type) return;
  OCIParam *mypard = NULL;
  ub4 counter = 1;
  sb4 parm_status = OCIParamGet(__stmthp, OCI_HTYPE_STMT, errhp, (dvoid**) &mypard, counter);
  if (parm_status != OCI_SUCCESS)
    return;
  __OCI_OP(parm_status, "");
  ub2 dtype;
  OCIDefine *defnp;
  vector< shared_ptr< sb2 > > indicators;

  while (parm_status == OCI_SUCCESS)
  {
    dtype = 0;
    __OCI_OP(OCIAttrGet((dvoid*) mypard, (ub4) OCI_DTYPE_PARAM, (dvoid*) &dtype,(ub4 *) 0, (ub4) OCI_ATTR_DATA_TYPE, (OCIError *) errhp), "");
    int char_semantics = 0;
    __OCI_OP(OCIAttrGet((dvoid*) mypard, (ub4) OCI_DTYPE_PARAM, (dvoid*) &char_semantics,(ub4 *) 0, (ub4) OCI_ATTR_CHAR_USED, (OCIError *) errhp  ), "");
    int col_width = 0;
    if (char_semantics)
    {
      __OCI_OP(OCIAttrGet((dvoid*) mypard, (ub4) OCI_DTYPE_PARAM, (dvoid*) &col_width, (ub4 *) 0, (ub4) OCI_ATTR_CHAR_SIZE, (OCIError *) errhp), "");
    }
    else
    {
      __OCI_OP(OCIAttrGet((dvoid*) mypard, (ub4) OCI_DTYPE_PARAM, (dvoid*) &col_width,(ub4 *) 0, (ub4) OCI_ATTR_DATA_SIZE, (OCIError *) errhp), "");
    }
    indicators.push_back(shared_ptr<sb2>(new sb2(0)));
    shared_ptr< OCIDate > date_ptr;
    shared_ptr< double > double_ptr;
    shared_array< char > char_ptr;
    OCILobLocator *lob;
    __clob_locator clob;
    __blob_locator blob;
    switch (dtype)
    {
    case 112:
      __OCI_OP(OCIDescriptorAlloc((dvoid*) __envhp, (dvoid **)&(clob.loc), (ub4)OCI_DTYPE_LOB, (size_t)0, (dvoid**)0), "");
      tmp_data.push_back(clob);
      __OCI_OP(OCIDefineByPos(__stmthp, &defnp, errhp, counter, (dvoid *) &(boost::get<__clob_locator>(tmp_data.back()).loc), 0, SQLT_CLOB, (dvoid *) indicators[counter - 1].get(), (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT), "");
      break;
    case 113:
      __OCI_OP(OCIDescriptorAlloc((dvoid*) __envhp, (dvoid **)&(blob.loc), (ub4)OCI_DTYPE_LOB, (size_t)0, (dvoid**)0), "");
      tmp_data.push_back(blob);
      __OCI_OP(OCIDefineByPos(__stmthp, &defnp, errhp, counter, (dvoid *) &(boost::get<__blob_locator>(tmp_data.back()).loc), 0, SQLT_BLOB, (dvoid *) indicators[counter - 1].get(), (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT), "");
      break;
    case 1:
      char_ptr.reset(new char[col_width + 1]);
      tmp_data.push_back(char_ptr);
      __OCI_OP(OCIDefineByPos(__stmthp, &defnp, errhp, counter, (dvoid *) char_ptr.get(), col_width + 1, SQLT_STR, (dvoid *) indicators[counter - 1].get(), (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT), "");
      break;
    case 12:
      date_ptr.reset(new OCIDate);
      tmp_data.push_back(date_ptr);
      __OCI_OP(OCIDefineByPos(__stmthp, &defnp, errhp, counter, (dvoid *) date_ptr.get(), sizeof(OCIDate), SQLT_ODT, (dvoid *) indicators[counter - 1].get(), (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT), "");
      break;
    case 2:
      double_ptr.reset(new double);
      tmp_data.push_back(double_ptr);
      __OCI_OP(OCIDefineByPos(__stmthp, &defnp, errhp, counter, (dvoid *) double_ptr.get(), sizeof(double), SQLT_FLT, (dvoid *) indicators[counter - 1].get(), (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT), "");
      break;
    default:
      char_ptr.reset(new char[0]);
      tmp_data.push_back(char_ptr);
      break;
    }
    counter++;
    parm_status = OCIParamGet(__stmthp, OCI_HTYPE_STMT, errhp, (dvoid**) &mypard, counter);
  }
  
  __data.clear();
  __nulls.clear();
  OCILobLocator* surr_lob;
  for (int ind = 0;;ind++)
  {
    int __result = OCIStmtFetch(__stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT);
    if (__result == OCI_NO_DATA) break;
    __OCI_OP(__result, "");
    __nulls.push_back(std::set< int >());
    for (unsigned int i = 0; i < tmp_data.size(); i++)
    {
      bool is_null = (*(indicators[i]) == -1);
      if (is_null)
        __nulls.back().insert(i);
      try
      {
        ub4 col_width = 0;
        __clob_locator lob = boost::get< __clob_locator >(tmp_data[i]);
        surr_lob = lob.loc;
        __OCI_OP(OCILobGetLength(__svchp, errhp, lob.loc, (ub4*) &col_width), "");
        vector< char > buf;
        buf.resize(col_width);
        oraub8 byte_ampt = col_width;
        oraub8 char_ampt = 0;
        oraub8 offset = 0;
        if (col_width)
          __OCI_OP(OCILobRead2(__svchp, errhp, lob.loc, &byte_ampt, &char_ampt, 1, &(buf[0]), col_width, OCI_ONE_PIECE, NULL, NULL, 0, SQLCS_IMPLICIT), "");
        __data[ind].push_back(buf);
        continue;
      }
      catch (boost::bad_get&) {}
      try
      {
        ub4 col_width = 0;
        __clob_locator lob = boost::get< __clob_locator >(tmp_data[i]);
        surr_lob = lob.loc;
        __OCI_OP(OCILobGetLength(__svchp, errhp, lob.loc, (ub4*) &col_width), "");
        vector< unsigned char > buf;
        buf.resize(col_width);
        oraub8 byte_ampt = col_width;
        oraub8 char_ampt = 0;
        oraub8 offset = 0;
        if (col_width)
          __OCI_OP(OCILobRead2(__svchp, errhp, lob.loc, &byte_ampt, &char_ampt, 1, &(buf[0]), col_width, OCI_ONE_PIECE, NULL, NULL, 0, SQLCS_IMPLICIT), "");
        __data[ind].push_back(buf);
        continue;
      }
      catch (boost::bad_get&) {}
      try
      {
        double *double_val = boost::get<shared_ptr<double> >(tmp_data[i]).get();
        if (is_null)
        {
          __data[ind].push_back(double(0));
          continue;
        }
        __data[ind].push_back(*double_val);
        continue;
      }
      catch (boost::bad_get&) {}
      try
      {
        string str_val = string(boost::get<shared_array< char > >(tmp_data[i]).get());
        if (is_null)
        {
          __data[ind].push_back(string());
          continue;
        }
        __data[ind].push_back(str_val);
        continue;
      }
      catch (boost::bad_get&) {}
      try
      {
        OCILobLocator* val = *(boost::get<shared_ptr<OCILobLocator*> >(tmp_data[i]).get());
        oraub8   offset = 1;
        oraub8   loblen = 0; 
        oraub8   byte_amt = 0;
        oraub8   char_amt = 0;
        ub1      bufp[1024];

        sword retval; 

        if (retval = OCILobRead2(__svchp, errhp, val, &byte_amt, &char_amt, offset,
          (dvoid *) bufp, (oraub8) 1024,  OCI_FIRST_PIECE, 0, 
          cbk_read_lob, (ub2) 0, (ub1) SQLCS_IMPLICIT))
        {
          //(void) printf("ERROR: OCILobRead2() LOB.\n");
        }

        continue;
      }
      catch (boost::bad_get&) {}
      try
      {
        OCIDate* val = boost::get<shared_ptr<OCIDate> >(tmp_data[i]).get();
        if (is_null)
        {
          __data[ind].push_back(ptime());
          continue;
        }
        val->OCIDateYYYY = (val->OCIDateYYYY < 1400 || val->OCIDateYYYY > 10000) ? 1400 : val->OCIDateYYYY;
        ptime ptime_val(date(val->OCIDateYYYY, val->OCIDateMM, val->OCIDateDD),
            time_duration(val->OCIDateTime.OCITimeHH, val->OCIDateTime.OCITimeMI, val->OCIDateTime.OCITimeSS)); 
        __data[ind].push_back(ptime_val);
        continue;
      }
      catch (boost::bad_get&) {}
    }
  }
//  __OCI_OP(OCIDescriptorFree((dvoid*)&surr_lob, OCI_DTYPE_LOB), "");
  __fetch_iter = __data.begin();
  __nulls_iter = __nulls.begin();
  before_first = true;
}

bool ksi_client::oracle_query_impl::out_is_null(const string& col_name)
{
  string myname = "";
  if (':' != col_name[0])
    myname += string(":");
  myname += col_name;
  return __out_nulls.find(myname) != __out_nulls.end();
}

bool ksi_client::oracle_query_impl::out_is_null(const int col_index)
{
  return __nulls_iter->find(col_index) != __nulls_iter->end();
}

void ksi_client::oracle_session_impl::ksi_transaction_begin(long num)
{
}

void ksi_client::oracle_session_impl::ksi_transaction_end()
{
}

void ksi_client::oracle_session_impl::direct_execute(const std::string& stmt)
{
  OCIStmt *__stmthp;
  __OCI_OP(OCIHandleAlloc((dvoid*) envhp,(dvoid **)&__stmthp, OCI_HTYPE_STMT, 0, NULL), "");
  __OCI_OP(OCIStmtPrepare(__stmthp, errhp, (text*) stmt.c_str(), stmt.length(), OCI_NTV_SYNTAX, OCI_DEFAULT), "");
  int durect_execute_retval = 0;
  __OCI_OP(((retval = OCIStmtExecute(svchp, __stmthp, errhp, 1, 0, (OCISnapshot *) 0, (OCISnapshot *) 0, OCI_DEFAULT)) == OCI_SUCCESS_WITH_INFO ? OCI_SUCCESS : retval), "");
  __OCI_OP(OCIHandleFree(__stmthp, OCI_HTYPE_STMT), "");
}

void ksi_client::oracle_session_impl::commit()
{
  __OCI_OP(OCITransCommit(svchp, errhp, OCI_DEFAULT), "")
}

void ksi_client::oracle_session_impl::rollback()
{
  __OCI_OP(OCITransRollback(svchp, errhp, OCI_DEFAULT), "")
}

