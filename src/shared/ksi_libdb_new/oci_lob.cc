/*
 * =====================================================================================
 *
 *       Filename:  oci_lob.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09.12.2008 17:29:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel G. Koukoushkin (mn), k.pavel.g@gmail.com
 *        Company:  Phlint and K, Moscow, Russia
 *
 * =====================================================================================
 */


#include "oci_lob.hh"
#include "oci_op.hh"
#include <boost/shared_ptr.hpp>
#include <vector>

using namespace database::oracle::__datatypes;
using namespace database::oracle::__oci_op;
using namespace boost;
using namespace std;

namespace database { namespace oracle { namespace __datatypes
{

  struct __oci_blob : public __oci_lob_i
  { 
    OCISvcCtx* __svc_ctx;
    OCIError* __err;
    OCILobLocator* __lob_locator;
    OCIEnv* __env;
    __oci_blob()
    {
    }
    void** handle_ptr()
    {
      return (void**)&__lob_locator;
    }
    void* handle()
    {
      return (void*)__lob_locator;
    }
    size_t size()
    {
      return sizeof(OCILobLocator*);
    }
    unsigned int get_length()
    {
      unsigned int len;
      __oci_lob_get_length(__svc_ctx, __err, __lob_locator, (&len));
      return len;
    }
    blob_data_ptr extract_blob_data()
    {
      unsigned int len = get_length();
      blob_data_ptr ret (new blob_data_container);
      ret->resize(len);
      unsigned long long ll_len = len;
      char* data_ptr = reinterpret_cast<char*>(ret->data());
      __oci_lob_read(__svc_ctx, __err, __lob_locator, &ll_len, NULL, data_ptr, len); 
      return ret;
    }
    clob_data_ptr extract_clob_data()
    {
      unsigned int len = get_length();
      clob_data_ptr ret (new clob_data_container);
      ret->resize(len);
      unsigned long long ll_len = len;
      char* data_ptr = ret->data();
      __oci_lob_read(__svc_ctx, __err, __lob_locator, &ll_len, &ll_len, data_ptr, len); 
      return ret;
    }
    virtual void initialize(void* oci_env_handle, void* oci_svcctx_handle, void* oci_error_handle)
    {
      __env =       reinterpret_cast<OCIEnv*>(oci_env_handle);
      __svc_ctx =   reinterpret_cast<OCISvcCtx*>(oci_svcctx_handle);
      __err =       reinterpret_cast<OCIError*>(oci_error_handle);
      __oci_handle_allocate(__env, reinterpret_cast<void**>(&__lob_locator), OCI_DTYPE_LOB, true);
    }
    virtual ~__oci_blob()
    {
      __oci_handle_free(__lob_locator, OCI_DTYPE_LOB);
    }
  }; 

}; }; };

using namespace database::oracle::__datatypes;

oci_blob::oci_blob()
  : lob(new __oci_blob())
{
}
oci_clob::oci_clob()
  : lob(new __oci_blob())
{
}
