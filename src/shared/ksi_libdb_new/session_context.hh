/*
 * =====================================================================================
 *
 *       Filename:  session_context.hh
 *
 *    Description:  Context part of session. Contains handles
 *
 *        Version:  1.0
 *        Created:  09.12.2008 18:13:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel G. Koukoushkin (mn), k.pavel.g@gmail.com
 *        Company:  Phlint and K, Moscow, Russia
 *
 * =====================================================================================
 */

#include "oci_handles.hh"

namespace database { namespace oracle { namespace  __context {

  struct __session_context
  {
      database::oracle::__oci_handles::__oci_handle<OCIEnv> __env;
      database::oracle::__oci_handles::__oci_handle<OCIError> __err;
      database::oracle::__oci_handles::__oci_handle<OCISvcCtx> __ctx;
      database::oracle::__oci_handles::__oci_handle<OCIServer> __server;
      database::oracle::__oci_handles::__oci_handle<OCISession> __session;
  };

}; }; };
