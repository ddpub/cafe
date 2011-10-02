#ifndef __OCI_HANDLES_H__
#define __OCI_HANDLES_H__
#include "oci_op.hh"


namespace database
{
  namespace oracle
  {
    namespace __oci_handles
    {
      template <class T>
        unsigned int __get_htype()
        { return 0; }

      template<>
        inline unsigned int __get_htype<OCIError>() { return OCI_HTYPE_ERROR; }
      template<>
        inline unsigned int __get_htype<OCISvcCtx>() { return OCI_HTYPE_SVCCTX; }
      template<>
        inline unsigned int __get_htype<OCISession>() { return OCI_HTYPE_SESSION; }
      template<>
        inline unsigned int __get_htype<OCIServer>() { return OCI_HTYPE_SERVER; }
      template<>
        inline unsigned int __get_htype<OCIStmt>() { return OCI_HTYPE_STMT; }
      template<>
        inline unsigned int __get_htype<OCIParam>() { return OCI_DTYPE_PARAM; }
      template<>
        inline unsigned int __get_htype<OCILobLocator>() { return OCI_DTYPE_LOB; }
      
      template<class T>
        struct __oci_handle
        {
          __oci_handle(OCIEnv* env_handle);
          __oci_handle();
          OCIEnv* __env_handle;
          OCISvcCtx* __svcctx_handle;
          OCIError* __err_handle;
          __oci_handle(OCIEnv* env_handle, T* handle, OCISvcCtx* svcctx_handle = NULL, OCIError* err_handle = NULL)
            : __env_handle(env_handle)
            , __handle(handle)
            , __svcctx_handle(svcctx_handle)
            , __err_handle(err_handle)
          {};
          T* __handle;
          ~__oci_handle();
        };       
      
      extern __oci_handle<OCIEnv> environment;

      template <class T>
        __oci_handle<T>::__oci_handle(OCIEnv* env_handle) :
          __env_handle(env_handle)
        {
          database::oracle::__oci_op::__oci_handle_allocate(__env_handle, (void**)&__handle, __get_htype<T>());
        }

        
      template<class T>
        __oci_handle<T>::__oci_handle() :
          __env_handle(environment.__handle)
        {
          database::oracle::__oci_op::__oci_handle_allocate(__env_handle, (void**)&__handle, __get_htype<T>());
        }


      
      template <class T>
        __oci_handle<T>::~__oci_handle()
        {
          database::oracle::__oci_op::__oci_handle_free((void*)__handle, __get_htype<T>());
        }

    };
  };
};

#endif // __OCI_HANDLES_H__

