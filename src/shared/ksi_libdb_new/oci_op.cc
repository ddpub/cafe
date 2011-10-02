#include "oci_op.hh"

#include "database_exception.hpp"
#include <iostream>

using namespace database;
using namespace database::oracle;
using namespace std;

void throw_db_exception(int ora_error, const std::string& internal_msg)
{
  if(ora_error == 1017 || ora_error == 1918)
    throw database::exception(DB_ERROR_INVALID_CREDENTIALS, internal_msg, ora_error);
  else if(ora_error == 1005)
    throw database::exception(DB_ERROR_EMPTY_PASSWORD, internal_msg, ora_error);
  else if(ora_error == 2292)
    throw database::exception(DB_ERROR_CHILD_RECORDS_EXISTS, internal_msg, ora_error);
  else if(ora_error == 1)
    throw database::exception(DB_ERROR_INVALID_HOSTNAME, internal_msg, ora_error);
  else if(ora_error == 3113 || ora_error == 1089 || ora_error == 3114)
    throw database::connection_problem_exception(DB_ERROR_CONNECTION_PROBLEM, internal_msg, ora_error);
  else if (ora_error == 28000)
    throw database::exception(DB_ERROR_ACCOUNT_LOCKED, internal_msg, ora_error);
  else if (ora_error == 20600)
    throw database::exception(DB_ERROR_INTERNAL, internal_msg, ora_error);
  else if (ora_error == 54)
    throw database::exception(DB_ERROR_RESOURCE_BUSY, internal_msg, ora_error);
  else throw database::exception(DB_ERROR_INTERNAL, internal_msg, ora_error);
} 


void __op(OCIError *errhp, sword status, const std::string& details,
    bool throws_exception)
{
//  cout << "LOG: " << details << endl;
  if (status == OCI_SUCCESS)
    return;
  text errbuf[512];
  sb4 errcode = 0;
  stringstream ss;
  ss << "ERROR: ";

  switch (status)
  {
  case OCI_SUCCESS:
    break;
  case OCI_SUCCESS_WITH_INFO:
    ss << "Error - OCI_SUCCESS_WITH_INFO";
    break;
  case OCI_NEED_DATA:
    ss << "Error - OCI_NEED_DATA";
    break;
  case OCI_NO_DATA:
    ss <<"Error - OCI_NODATA";
    break;
  case OCI_ERROR:
    if (!errhp)
    {
      ss << "Unexpected error while errhp is NULL";
      break;
    }
    (void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
                        errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    ss << "Error - " << errcode << " " << (const char*) errbuf;
    break;
  case OCI_INVALID_HANDLE:
    ss << "Error - OCI_INVALID_HANDLE";
    break;
  case OCI_STILL_EXECUTING:
    ss << "Error - OCI_STILL_EXECUTE";
    break;
  case OCI_CONTINUE:
    ss << "Error - OCI_CONTINUE";
    break;
  default:
    break;
  } 
  if (details.size())
    ss << " DETAILS: " << details;
  if (throws_exception)
    throw_db_exception(errcode, ss.str());
//  else
//    cerr << "Warning " << ss.str() << endl;
}


void __oci_op::__oci_create_environment_default(OCIEnv** envhp, bool throws_exception)
{ 
  __op(NULL, OCIEnvCreate((OCIEnv **)envhp,
     (ub4)OCI_DEFAULT,
     (dvoid *)0, (dvoid * (*)(dvoid *, size_t))0,
     (dvoid * (*)(dvoid *, dvoid *, size_t))0,
     (void (*)(dvoid *, dvoid *))0,
    (size_t)0, (dvoid **)0), "Create Oracle environment", throws_exception);
}

void __oci_op::__oci_handle_allocate(OCIEnv* envhp, void** handle, unsigned int stmt_type, bool throws_exception)
{
  if (OCI_DTYPE_LOB == stmt_type)
    __op(NULL, OCIDescriptorAlloc ((dvoid *)envhp, (dvoid **)handle,
          stmt_type, 0, (dvoid **) 0), "Allocate descriptor", throws_exception);
  else
    __op(NULL, OCIHandleAlloc ((dvoid *)envhp, (dvoid **)handle,
          stmt_type, 0, (dvoid **) 0), "Allocate handle", throws_exception);
}

void __oci_op::__oci_handle_free(void* handle, unsigned int stmt_type, bool throws_exception)
{
  if (OCI_DTYPE_LOB == stmt_type)
    __op(NULL, OCIDescriptorFree(handle, stmt_type), "Freeing descriptor", throws_exception);
  else
    __op(NULL, OCIHandleFree(handle, stmt_type), "Freeing handle", throws_exception);
}

short __oci_op::__oci_descriptor_allocate(const void *envhp, void **descpp, unsigned int type)
{
  return OCIDescriptorAlloc(envhp, descpp,  type, 0, NULL);
}

void __oci_op::__oci_descriptor_free(void* descp, unsigned int type)
{
  OCIDescriptorFree(descp, type);
}

void __oci_op::__oci_server_attach(OCIServer* srvhp, OCIError* errhp, const std::string& server_name,
    const std::string& database_name,
    int port, bool throws_exception)
{
  stringstream ss;
  ss << "(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST="
    << server_name << ")(PORT=" << port << ")))(CONNECT_DATA=(SERVICE_NAME=" << database_name
    << ")))"; 
  __op(errhp, OCIServerAttach (srvhp, errhp, (text *)ss.str().c_str(),
        ss.str().length() + 1, OCI_DEFAULT), "Server attach", throws_exception);
}

void __oci_op::__oci_session_set_login_information(OCISession* usrhp,
    const std::string& username,
    const std::string& password,
    OCIError* errhp, bool throws_exception)
{
  __op(errhp, OCIAttrSet ((dvoid *)usrhp, OCI_HTYPE_SESSION, (dvoid*) username.c_str(), username.length(), OCI_ATTR_USERNAME,
        errhp), "Appending Username", throws_exception);

  __op(errhp, OCIAttrSet ((dvoid *)usrhp, OCI_HTYPE_SESSION, (dvoid*) password.c_str(), password.length(), OCI_ATTR_PASSWORD,
        errhp), "Appending Password", throws_exception);
}


void __oci_op::__oci_attach_server_handle(OCISvcCtx* svchp, OCIServer* srvhp,
    OCIError* errhp, bool throws_exception)
{
  __op(errhp, OCIAttrSet ((dvoid *)svchp, OCI_HTYPE_SVCCTX, (dvoid *)srvhp,
        (ub4) 0, OCI_ATTR_SERVER, errhp), "Attach server handle", throws_exception);
}

void __oci_op::__oci_attach_session_handle(OCISvcCtx* svchp, OCISession* usrhp,
    OCIError* errhp, bool throws_exception)
{
  __op(errhp, OCIAttrSet ((dvoid *)svchp, OCI_HTYPE_SVCCTX, (dvoid *)usrhp,
        (ub4) 0, OCI_ATTR_SESSION, errhp), "Attach session handle", throws_exception);
}

void __oci_op::__oci_session_begin(OCISvcCtx* svchp, OCISession* usrhp,
    OCIError* errhp, bool throws_exception)
{
  __op(errhp, OCISessionBegin (svchp, errhp, usrhp, OCI_CRED_RDBMS,
        OCI_DEFAULT), "Begin Session", throws_exception);
}

void __oci_op::__oci_param_get(void *handle, unsigned int type, OCIError*
    errhp, OCIParam** param, unsigned int pos, bool throws_exception)
{
  __op(errhp, OCIParamGet(handle, type, errhp, (void**) param, pos), "Get param", throws_exception);
}

void __oci_op::__oci_define_dynamic(OCIDefine *defnp, OCIError *errhp, void *octxp,  __define_callback_ptr callback)
{
  __op(errhp, OCIDefineDynamic(defnp, errhp, octxp, callback), "Define dynamic", true);
}

void __oci_op::__oci_define_by_pos(OCIStmt* stmthp, OCIDefine** defpp, OCIError* errhp, unsigned int
    position, void* value, unsigned int value_sz,
        unsigned short type, short* indp, bool dynamic, bool throw_exception)
{
  __op(errhp, OCIDefineByPos(stmthp, defpp, errhp, position, value, value_sz,
        type, indp, NULL, NULL, dynamic ? OCI_DYNAMIC_FETCH : OCI_DEFAULT), "Define field", throw_exception);
}

void __oci_op::__oci_trans_commit(OCISvcCtx* svchp, OCIError* errhp, bool throws_exception)
{
  __op(errhp, OCITransCommit(svchp, errhp, OCI_DEFAULT), "Commit Transaction", throws_exception);
}

void __oci_op::__oci_trans_rollback(OCISvcCtx* svchp, OCIError* errhp, bool throws_exception)
{ 
  __op(errhp, OCITransRollback(svchp, errhp, OCI_DEFAULT), "Rollback transaction", throws_exception);
}

void __oci_op::__oci_stmt_prepare(OCIStmt* stmthp, OCIError* errhp, const string& stmt, bool throws_exception)
{
  stringstream ss;
  ss << "Preparing statement: " << stmt;
  __op(errhp, OCIStmtPrepare(stmthp, errhp, (text*) stmt.c_str(), stmt.length(),
        OCI_NTV_SYNTAX, OCI_DEFAULT), ss.str(), throws_exception);
}

void __oci_op::__oci_stmt_execute(OCIStmt* stmthp, OCIError* errhp, OCISvcCtx* svchp, unsigned int mode, bool throws_exception)
{
  unsigned short type = 0;
  string operation = "Getting statement type before executing";
  __op(errhp, OCIAttrGet(stmthp, OCI_HTYPE_STMT, &type, NULL,
        OCI_ATTR_STMT_TYPE, errhp), operation, throws_exception);
  unsigned int iters = 1;
  if ((OCI_DESCRIBE_ONLY == mode) ||  (OCI_STMT_SELECT == type))
    iters = 0;
  __op(errhp, OCIStmtExecute(svchp, stmthp, errhp, iters, 0, (OCISnapshot *) 0,
        (OCISnapshot *) 0, mode), "Executing statement",
      throws_exception);
}

void __oci_op::__oci_attr_get(void* hndl, unsigned int htype, void *attrp, unsigned int attrtype, OCIError * errhp, bool throws_exception)
{ 
  int ret = OCIAttrGet(hndl, htype, attrp, NULL, attrtype, errhp);
  __op(errhp, ret, "Getting attribute value",
      throws_exception);
}

bool __oci_op::__oci_stmt_fetch(OCIStmt* stmtp, OCIError *errhp, unsigned int
    nrows, bool& data_need, bool throws_exception)
{
  int retval;
  retval = OCIStmtFetch(stmtp, errhp, nrows, OCI_FETCH_NEXT, OCI_DEFAULT);
  data_need = (OCI_NEED_DATA == retval);
  if (data_need)
    retval = OCI_SUCCESS;
  __op(errhp, retval, "Fetching row", OCI_NO_DATA != retval ? throws_exception : false);
  return OCI_NO_DATA != retval;
}

void __oci_op::__oci_stmt_get_piece_info(OCIStmt* stmthp, OCIError* errhp, void** hndlp, unsigned int *typep, unsigned char *in_outp,
    unsigned int *iterp, unsigned int *idxp, unsigned char* piecep)
{
  __op(errhp, OCIStmtGetPieceInfo(stmthp, errhp, hndlp, typep, in_outp, iterp, idxp, piecep), "Get piece info", true); 
}

void __oci_op::__oci_stmt_set_piece_info(void *hndlp, unsigned int type,
          OCIError *errhp, const void *bufp, unsigned int *alenp,
          unsigned char piece, const void *indp, unsigned short *rcodep)
{
  __op(errhp, OCIStmtSetPieceInfo(hndlp, type, errhp, bufp, alenp, piece, indp, rcodep), "Set piece info", true);
}

void __oci_op::__oci_session_end(OCISvcCtx* svchp, OCISession* usrhp,
    OCIError* errhp, bool throws_exception)
{ 
  __op(errhp, OCISessionEnd(svchp, errhp, usrhp, OCI_DEFAULT), "Session end", throws_exception);
}

void __oci_op::__oci_server_detach(OCIServer* srvhp,
    OCIError* errhp, bool throws_exception)
{ 
  __op(errhp, OCIServerDetach(srvhp, errhp, OCI_DEFAULT), "Server detach",
      throws_exception);
}

void __oci_op::__oci_lob_read(OCISvcCtx* svchp, OCIError* errhp,
    OCILobLocator* lob, unsigned long long* byte_ampt, unsigned long long* char_ampt, char* d_ptr, unsigned int count)
{
  __op(errhp,  OCILobRead2(svchp, errhp, lob,
        reinterpret_cast<oraub8*>(byte_ampt), reinterpret_cast<oraub8*>(char_ampt), 1, d_ptr,
        count, OCI_ONE_PIECE, NULL, NULL, 0, SQLCS_IMPLICIT), "Reading lob", true);
}


void __oci_op::__oci_lob_get_length(OCISvcCtx* svchp, OCIError* errhp, OCILobLocator*
    lob, unsigned int* col_width)
{
  __op(errhp, OCILobGetLength(svchp, errhp, lob,
        col_width), "Get lob length", true);
}

void __oci_op::__oci_bind_by_pos(OCIStmt* stmthp, OCIError* errhp, unsigned int
    position, void* data_ptr, int size, unsigned short type, void* indic_p)
{
  OCIBind* bindpp;
  __op(errhp, OCIBindByPos(stmthp, &bindpp, errhp, position, data_ptr, size, type, indic_p,
      NULL, NULL, 0, NULL, OCI_DEFAULT), "Bind parameter", true);
}

void __oci_op::__oci_bind_by_name(OCIStmt* stmthp, OCIError* errhp, const std::string& alias, void* data_ptr, int size, unsigned short type, void* indic_p)
{
  OCIBind* bindpp;
  string details("Bind parameter: ");
  details += alias;
  __op(errhp, OCIBindByName(stmthp, &bindpp, errhp, reinterpret_cast<const OraText*>(alias.c_str()), alias.length(), data_ptr, size, type, indic_p,
      NULL, NULL, 0, NULL, OCI_DEFAULT), details, true);
}


