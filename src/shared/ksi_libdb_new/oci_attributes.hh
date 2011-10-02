#ifndef __OCI_ATTRIBUTES_H__
#define __OCI_ATTRIBUTES_H__
#include "oci_handles.hh"

namespace database
{
  namespace oracle
  {
    namespace __oci_attributes
    {
      template<unsigned int AttrTypecode>
        struct __attr_type
        { 
          typedef void* __type;
        };
     
      template<>
        struct __attr_type<OCI_ATTR_ROW_COUNT>
        {
          typedef unsigned int __type;
        };

      template<>
        struct __attr_type<OCI_ATTR_PARAM_COUNT>
        {
          typedef unsigned int __type;
        };
      
      template<>
        struct __attr_type<OCI_ATTR_STMT_TYPE>
        {
          typedef unsigned int __type;
        };
      
      template<>
        struct __attr_type<OCI_ATTR_DATA_TYPE>
        {
          typedef unsigned short __type;
        };
      
      template<>
        struct __attr_type<OCI_ATTR_DATA_SIZE>
        {
          typedef unsigned int __type;
        };
      
      template<>
        struct __attr_type<OCI_ATTR_CHAR_SIZE>
        {
          typedef unsigned short __type;
        };
      
      template<>
        struct __attr_type<OCI_ATTR_CHAR_USED>
        {
          typedef unsigned short __type;
        };

      template<>
        struct __attr_type<OCI_ATTR_NAME>
        {
          typedef std::string __type;
        };
      
      template<unsigned int AttrTypecode>
        struct __attr_getter
        {
          typedef typename __attr_type<AttrTypecode>::__type __a_type;
          template<class HandleType>
            static __a_type get(HandleType* hndl, OCIError* errhp)
            { 
              __a_type val;
              __oci_attr_get(hndl, database::oracle::__oci_handles::__get_htype<HandleType>(), &val, AttrTypecode, errhp, true);
              return val;
            }
        };
      
      template<>
        struct __attr_getter<OCI_ATTR_NAME>
        {
          typedef __attr_type<OCI_ATTR_NAME>::__type __a_type;
          template<class HandleType>
            static __a_type get(HandleType* hndl, OCIError* errhp)
            { 
              __a_type val;
              boost::shared_array<char> data (new char[1024]);
              __oci_attr_get(hndl, database::oracle::__oci_handles::__get_htype<HandleType>(), data.get(), OCI_ATTR_NAME, errhp, true);
              val = __a_type(data.get());
              return val;
            }
        };
  
      template<class HandleType>
        OCIParam* __get_param(HandleType* hndl, OCIError* errhp, unsigned int pos)
        {
          OCIParam* param;
          __oci_param_get(hndl, database::oracle::__oci_handles::__get_htype<HandleType>(), errhp, &param, pos,
              true);
          return param;
        }


      template<class HandleType, typename ParamType>
        void get_attribute(HandleType* hndlp, ParamType* value, unsigned int
            attr_type, OCIError* errhp)
        {
          __oci_attr_get(hndlp, database::oracle::__oci_handles::__get_htype<HandleType>(), value, attr_type, errhp, true);
        }
    };
  };
};

#endif //__OCI_ATTRIBUTES_H__
