#ifndef __OCI_OP_H__
#define __OCI_OP_H__

#include <oci.h>
#include <string>
#include <sstream>
#include <vector>
#include <boost/shared_ptr.hpp>



namespace database
{ 
  namespace oracle
  {
    namespace __oci_op
    { 
      
      void __oci_create_environment_default(OCIEnv** envhp, bool throws_exception = true);
      void __oci_handle_allocate(OCIEnv* envhp, void** handle, unsigned int stmt_type, bool throws_exception = true);
      void __oci_handle_free(void* handle, unsigned int stmt_type, bool throws_exception = true);
      short __oci_descriptor_allocate(const void *envhp, void **descpp, unsigned int type);
      void __oci_descriptor_free(void* handle, unsigned int stmt_type);
      
      void __oci_server_attach(OCIServer* srvhp, OCIError* errhp, const std::string& server_name,
          const std::string& database_name,
          int port = 1521, bool throws_exception = true);
      void __oci_session_set_login_information(OCISession* usrhp,
          const std::string& username,
          const std::string& password,
          OCIError* errhp, bool throws_exception = true);


      void __oci_attach_server_handle(OCISvcCtx* svchp, OCIServer* srvhp,
          OCIError* errhp, bool throws_exception = true);
      void __oci_attach_session_handle(OCISvcCtx* svchp, OCISession* usrhp,
          OCIError* errhp, bool throws_exception = true);
      void __oci_session_begin(OCISvcCtx* svchp, OCISession* usrhp,
          OCIError* errhp, bool throws_exception = true);

      void __oci_trans_commit(OCISvcCtx* svchp, OCIError* errhp, bool throws_exception = true);
      void __oci_trans_rollback(OCISvcCtx* svchp, OCIError* errhp, bool throws_exception = true);

      void __oci_stmt_prepare(OCIStmt* stmthp, OCIError* errhp, const std::string& stmt, bool throws_exception = true);
      void __oci_stmt_execute(OCIStmt* stmthp, OCIError* errhp, OCISvcCtx*
          svchp, unsigned int mode, bool throws_exception = true);

      void __oci_attr_get(void* hndl, unsigned int htype, void *attrp, unsigned int attrtype, OCIError * errhp, bool throws_exception = true);
      void __oci_param_get(void *handle, unsigned int type, OCIError*
          errhp, OCIParam** param, unsigned int pos, bool throws_exception);
      void __oci_define_by_pos(OCIStmt* stmthp, OCIDefine** defpp, OCIError* errhp, unsigned int
          position, void* value, unsigned int value_sz,
              unsigned short type, short* indp, bool dynamic, bool throw_exception);

      typedef int32_t ( *__define_callback_ptr )(void *octxp, OCIDefine* defnp,
          uint32_t iter, void **bufpp, 
          uint32_t **alenpp, uint8_t *piecep, 
          void **indpp, uint16_t** rcodepp);
      void __oci_define_dynamic(OCIDefine *defnp, OCIError *errhp, void *octxp,  __define_callback_ptr callback);
      bool __oci_stmt_fetch(OCIStmt* stmtp, OCIError *errhp, unsigned int
          nrows, bool& data_need, bool throws_exception);
      void __oci_stmt_get_piece_info(OCIStmt* stmthp, OCIError* errhp, void** hndlp, unsigned int *typep, unsigned char *in_outp,
          unsigned int *iterp, unsigned int *idxp, unsigned char* piecep);
      void __oci_stmt_set_piece_info(void *hndlp, unsigned int type,
          OCIError *errhp, const void *bufp, unsigned int *alenp,
          unsigned char piece, const void *indp, unsigned short *rcodep);

      void __oci_session_end(OCISvcCtx* svchp, OCISession* usrhp,
          OCIError* errhp, bool throws_exception = true);
      void __oci_server_detach(OCIServer* srvhp,
          OCIError* errhp, bool throws_exception = true);


      void __oci_lob_read(OCISvcCtx* svchp, OCIError* errhp,
          OCILobLocator* lob, unsigned long long* byte_ampt, unsigned long
          long* char_ampt, char* d_ptr, unsigned int count);

      void __oci_lob_get_length(OCISvcCtx* svchp, OCIError* errhp,
          OCILobLocator* lob, unsigned int* col_width);
    
      void __oci_bind_by_pos(OCIStmt* stmthp, OCIError* errhp, unsigned int
          position, void* data_ptr, int size, unsigned short type, void*
          indic_p);
      void __oci_bind_by_name(OCIStmt* stmthp, OCIError* errhp, const std::string& alias, void* data_ptr, int size, unsigned short type, void* indic_p);
    
    };

  };
};


#endif// __OCI_OP_H__
