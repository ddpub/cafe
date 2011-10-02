#ifndef DATATYPES_H
#define DATATYPES_H


#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdio>
//#include "oci_handles.hh"
#include <boost/shared_array.hpp>
#include "oci.h"
#include "oci_lob.hh"

struct OCIDate;

/* template<typename T>
class __array
{
  T* __ptr;
  unsigned int* __refcount;
public:
  __array(T* ptr)
    : __ptr(ptr)
    , __refcount(new unsigned int (1))
  {
  }
  __array()
    : __ptr(NULL)
    , __refcount(NULL)
  {
  }
  __array(const __array& array)
    : __ptr(array.__ptr)
    , __refcount(array.__refcount)
  {
    ++(*__refcount);
  }

  T& operator [](unsigned int pos)
  {
    return __ptr[pos];
  }
  
  T const& operator [](unsigned int pos) const
  {
    return __ptr[pos];
  }

  void release()
  {
    if (!__refcount)
      return;
    if (!(--(*__refcount)))
    {
      if (__ptr)
        delete[] __ptr;
      if (__refcount)
        delete __refcount;
    }
    __ptr = NULL;
    __refcount = NULL;
  }

  void reset(T* ptr)
  {
    release();
    __ptr = ptr;
    __refcount = new unsigned int(1);
  }

  T* get()
  {
    return __ptr;
  }
  
  const T* get() const
  {
    return __ptr;
  }

  ~__array()
  {
    release();
  }
};
*/

namespace database
{
  namespace oracle
  {
    namespace __datatypes
    {
      typedef database::oracle::__datatypes::oci_blob blob_locator_handle;
      typedef database::oracle::__datatypes::oci_clob clob_locator_handle;
      typedef std::vector<unsigned char> blob_container;
      typedef boost::shared_ptr< blob_container > blob_container_ptr;
      typedef std::vector<char> clob_container;
      typedef boost::shared_ptr< clob_container > clob_container_ptr;
      typedef std::string char_container_ptr;

      boost::posix_time::ptime __ocidate2ptime(OCIDate const& odate);
      OCIDate __ptime2ocidate(boost::posix_time::ptime const& odate);
      std::wstring __cp1251towstring(const std::string& src);


      struct __datatype_i
      {
        virtual void* ptr() = 0;
        virtual short* ind_ptr() = 0;
        virtual const short* ind_ptr() const = 0;
        virtual const void* ptr() const = 0;
        virtual const char* char_ptr() const = 0;
        virtual unsigned short typecode() const = 0;
        virtual size_t size() const = 0;
        virtual size_t resize(size_t len) = 0;
        virtual operator std::wstring() const = 0;
        virtual operator std::string() const = 0;
        virtual operator int() const = 0;
        virtual operator double() const = 0;
        virtual operator boost::posix_time::ptime() const = 0;
        virtual operator blob_container_ptr() const = 0;
        virtual operator clob_container_ptr() const = 0;
      }; 

      template<unsigned int TypeCode>
        struct __storage_type
        { 
          typedef char* type;
        };

      template<typename Type>
        struct __type_code
        {
          enum typecode  { TYPE_CODE = 0 };
        };

      template<typename Type>
        struct __ord_type_code
        { enum typecode  { TYPE_CODE = 0 }; };
      
      template<typename OraType>
        struct __ord_type
        {
          typedef void* type;
        };
      template<typename Type>
        struct __cpp_type
        {
          typedef void* type;
        };
#define CONSTRAINT_ORATYPE_CPPTYPE(ORATYPE, CPPTYPE, ORA_TYPECODE) \
      template<> \
        struct __storage_type<ORA_TYPECODE> \
        { typedef ORATYPE type; }; \
      template<> \
        struct __type_code<ORATYPE> \
        { enum typecode  { TYPE_CODE = ORA_TYPECODE }; }; \
      template<> \
        struct __ord_type_code<CPPTYPE> \
        { enum typecode { TYPE_CODE = ORA_TYPECODE }; }; \
      template<>\
        struct __ord_type<ORATYPE>\
        {\
          typedef CPPTYPE type;\
        };\
      template<>\
        struct __cpp_type<CPPTYPE>\
        {\
          typedef ORATYPE type;\
        };


CONSTRAINT_ORATYPE_CPPTYPE(char_container_ptr, std::string, 5); //SQLT_STR
CONSTRAINT_ORATYPE_CPPTYPE(blob_container, blob_container, 24); //SQLT_LBI
CONSTRAINT_ORATYPE_CPPTYPE(clob_container, clob_container, 8); //SQLT_LNG
CONSTRAINT_ORATYPE_CPPTYPE(blob_locator_handle, blob_container_ptr, 113); //SQLT_BLOB
CONSTRAINT_ORATYPE_CPPTYPE(clob_locator_handle, clob_container_ptr, 112); //SQLT_clob
CONSTRAINT_ORATYPE_CPPTYPE(long long int, long long int, 3); //SQLT_INT
CONSTRAINT_ORATYPE_CPPTYPE(double, double, 4); //SQLT_FLT
CONSTRAINT_ORATYPE_CPPTYPE(OCIDate, boost::posix_time::ptime, 156); //SQLT_ODT

/*      CONSTRAINT_TYPE_TYPECODE(__ora_blob, SQLT_BLOB);
      CONSTRAINT_TYPE_TYPECODE(__ora_clob, SQLT_CLOB);
      CONSTRAINT_TYPE_TYPECODE(OCIDate, SQLT_ODT);
      CONSTRAINT_TYPE_TYPECODE(int, SQLT_INT);
      CONSTRAINT_TYPE_TYPECODE(double, SQLT_FLT);
      CONSTRAINT_TYPE_TYPECODE(char*, SQLT_STR); */

      template<bool> 
      struct __cast_not_allowed__;
      template<>
        struct __cast_not_allowed__<true> {};

      template <class T, class U>
        T __cast(U value) { __cast_not_allowed__<false>(); };

      template<typename From>
        inline typename __ord_type<From>::type __to_ord_cast(From const& from)
        {
          return static_cast<typename __ord_type<From>::type>(from);
        }
      template<typename From>
        struct __to_cpp
        {
          static typename __cpp_type<From>::type cast(From const& from)
          {
            return static_cast<typename __cpp_type<From>::type>(from);
          }
        };
      template<>
        struct __to_cpp<boost::posix_time::ptime>
        {
          static __cpp_type<boost::posix_time::ptime>::type cast(boost::posix_time::ptime const& from)
          {
            return __ptime2ocidate(from);
          }
        };

/*      template<typename T>
      class shared_array
      {
        shared_array();
      };*/
      char_container_ptr __create_char(std::string const& from);

      template<>
        struct __to_cpp<std::string>
        {
          static __cpp_type<std::string>::type cast(std::string const& from)
          {
  /*        int i = 0;\
            for (; i < from.size(); i++)
              ret[i] = from[i];
            ret.get()[i] = '\0';*/
            return __create_char(from);
          }
        };
      template<>
        inline __ord_type<OCIDate>::type __to_ord_cast<OCIDate>(OCIDate const& from)
        {
          return __ocidate2ptime(from);
        }
      template<>
        inline __ord_type<blob_locator_handle>::type __to_ord_cast<blob_locator_handle>(blob_locator_handle const& from)
        {
          return from->extract_blob_data();
        }
      template<>
        inline __ord_type<clob_locator_handle>::type __to_ord_cast<clob_locator_handle>(clob_locator_handle const& from)
        {
          return from->extract_clob_data();
        }
      template<>
        inline __ord_type<char_container_ptr>::type __to_ord_cast<char_container_ptr>(char_container_ptr const& from)
        {
          return std::string(from.c_str());
        }

      template<class From>
        inline std::wstring cast_to_wstring(From const& from)
        {
          std::string data = boost::lexical_cast<std::string>(from);
          return __cp1251towstring(data);
        }

      template<>
        inline std::wstring cast_to_wstring<blob_container_ptr >(blob_container_ptr const& from)
        {
          return std::wstring();
        }
      template<>
        inline std::wstring cast_to_wstring<blob_container >(blob_container const& from)
        {
          return std::wstring();
        }
      template<>
        inline std::wstring cast_to_wstring<clob_container_ptr >(clob_container_ptr const& from)
        {
          return std::wstring();
        }
      template<>
        inline std::wstring cast_to_wstring<clob_container >(clob_container const& from)
        {
          return std::wstring();
        }

      template<class From>
        inline std::string cast_to_string(From const& from)
        {
          return boost::lexical_cast<std::string>(from);
        } 

      template<>
        inline std::string cast_to_string<blob_container_ptr>(blob_container_ptr const& from)
        {
          return std::string();
        }
      template<>
        inline std::string cast_to_string<blob_container>(blob_container const& from)
        {
          return std::string();
        }
      template<>
        inline std::string cast_to_string<clob_container_ptr>(clob_container_ptr const& from)
        {
          return std::string(from->data(), from->size());
        }
      template<>
        inline std::string cast_to_string<clob_container>(clob_container const& from)
        {
          return std::string(from.data(), from.size());
        }
      template<>
        inline std::string cast_to_string<char_container_ptr>(char_container_ptr const& from)
        {
          return std::string(from.c_str());
        }

      template<class From>
        inline int cast_to_int(From const& from)
        {
          return int();
        }

      template<>
        inline int cast_to_int(boost::posix_time::ptime const& from)
        {
          return int();
        }

      template<>
        inline int cast_to_int<std::string>(std::string const& from)
        {
          return static_cast<int>(boost::lexical_cast<double>(from) + .5);
        }

      template<>
        inline int cast_to_int<double>(double const& from)
        {
          return static_cast<int>(from + .5);
        }

      template<>
        inline int cast_to_int<blob_container_ptr >(blob_container_ptr const& from)
        {
          return int();
        }
      template<>
        inline int cast_to_int<blob_container >(blob_container const& from)
        {
          return int();
        }
      template<>
        inline int cast_to_int<clob_container_ptr >(clob_container_ptr const& from)
        {
          return int();
        }
      template<>
        inline int cast_to_int<clob_container >(clob_container const& from)
        {
          return int();
        }

      template<class From>
        inline double cast_to_double(From const& from)
        {
          return static_cast<double>(from);
        }

      template<>
        inline double cast_to_double<std::string>(std::string const& from)
        {
          return boost::lexical_cast<double>(from);
        }

      template<>
        inline double cast_to_double(boost::posix_time::ptime const& from)
        {
          return double();
        }

      template<>
        inline double cast_to_double<blob_container_ptr>(blob_container_ptr const& from)
        {
          return double();
        }
      template<>
        inline double cast_to_double<blob_container>(blob_container const& from)
        {
          return double();
        }
      template<>
        inline double cast_to_double<clob_container_ptr>(clob_container_ptr const& from)
        {
          return double();
        }
      template<>
        inline double cast_to_double<clob_container>(clob_container const& from)
        {
          return double();
        }

      template<class From>
        inline boost::posix_time::ptime cast_to_ptime(From const& from)
        {
          return boost::posix_time::ptime();
        }
      
      template<>
        inline boost::posix_time::ptime cast_to_ptime<boost::posix_time::ptime>(boost::posix_time::ptime const& from)
        {
          return from;
        }
     

      template<typename From>
        inline blob_container_ptr cast_to_blob_container_ptr(From const& from)
        {
          blob_container_ptr ptr (new blob_container(sizeof(from)));
          new (ptr.get()->data()) From(from);
        }
      template<>
        inline blob_container_ptr cast_to_blob_container_ptr(blob_locator_handle const& from)
        {
          return from->extract_blob_data();
        }
      template<>
        inline blob_container_ptr cast_to_blob_container_ptr(blob_container_ptr const& from)
        {
          return from;
        }
      template<typename From>
        inline clob_container_ptr cast_to_clob_container_ptr(From const& from)
        {
          clob_container_ptr ptr (new clob_container(sizeof(from)));
          new (ptr.get()->data()) From(from);
        }
      template<>
        inline clob_container_ptr cast_to_clob_container_ptr(clob_locator_handle const& from)
        {
          return from->extract_clob_data();
        }
      template<>
        inline clob_container_ptr cast_to_clob_container_ptr(clob_container_ptr const& from)
        {
          return from;
        }
      
      template<typename Type>
        struct __stub
        {
          static void resize(Type& data, size_t size)
          {
          }
          static void* ptr(Type& data)
          {
            return &data;
          }
          static void const * c_ptr(Type const & data)
          {
            return &data;
          }
          static size_t size(Type const & data)
          {
            return sizeof(data);
          }
        };
      
      template<>
        struct __stub<std::string>
        {
          static void resize(std::string& data, size_t size)
          {
            data.resize(size);
          }
          static void* ptr(std::string& data)
          {
            return const_cast<char*>(data.c_str());
          }
          static void const * c_ptr(std::string const & data)
          {
            return data.c_str();
          }
          static size_t size(std::string& data)
          {
            if (!data.length())
              return 0;
            if (data.length() > 4000)
              data.resize(4000);
            return data.length() + 1;
          }
        };

/*      template<>
        struct __stub<char_container_ptr>
        {
          static void resize(char_container_ptr& data, size_t size)
          {
            char* ch_data = new char[size];
            data.resize(size);
          }
          static void* ptr(char_container_ptr& data)
          {
            return data.data();
          }
          static void const * c_ptr(char_container_ptr const & data)
          {
            return data.data();
          }
          static size_t size(char_container_ptr& data)
          {
            return 0;
          }
        };*/

      template<>
        struct __stub<blob_locator_handle>
        {
          static void resize(blob_locator_handle& data, size_t size)
          {
          }
          static void* ptr(blob_locator_handle& data)
          {
            return reinterpret_cast<void*>(const_cast<void**>(data->handle_ptr()));
          }
          static void const * c_ptr(blob_locator_handle const & data)
          {
            return data->handle_ptr();
          }
          static size_t size(blob_locator_handle& data)
          {
            return sizeof(OCILobLocator*);
          }
        };
      template<>
        struct __stub<clob_locator_handle>
        {
          static void resize(clob_locator_handle& data, size_t size)
          {
          }
          static void* ptr(clob_locator_handle& data)
          {
            return reinterpret_cast<void*>(const_cast<void**>(data->handle_ptr()));
          }
          static void const * c_ptr(clob_locator_handle const & data)
          {
            return data->handle_ptr();
          }
          static size_t size(clob_locator_handle& data)
          {
            return sizeof(OCILobLocator*);
          }
        };

      template<>
        struct __stub<blob_container>
        {
          static void resize(blob_container& data, size_t size)
          {
            data.resize(size);
          }
          static void* ptr(blob_container& data)
          {
            return data.data();
          }
          static void const * c_ptr(blob_container const & data)
          {
            return data.data();
          }
          static size_t size(blob_container& data)
          {
            return data.size();
          }
        };
      template<>
        struct __stub<clob_container>
        {
          static void resize(clob_container& data, size_t size)
          {
            data.resize(size);
          }
          static void* ptr(clob_container& data)
          {
            return data.data();
          }
          static void const * c_ptr(clob_container const & data)
          {
            return data.data();
          }
          static size_t size(clob_container& data)
          {
            return data.size();
          }
        };

      template<unsigned int TypeCode> 
        struct __datatype : public __datatype_i
        { 
          typedef typename __storage_type<TypeCode>::type datatype;
          datatype __data;
          size_t __size;
          short ind;
          __datatype(datatype const& from, size_t size)
            : __data(from)
            , ind(0)
          {
            __size = __stub<datatype>::size(__data);
          }
          __datatype(size_t size)
            : ind(0)
          {
            __stub<datatype>::resize(__data, size);
            __size = __stub<datatype>::size(__data);
            if (!__size)
              __size = size;
          };
          __datatype()
          {
            ind = -1;
          }
          virtual void* ptr()
          {
            if (-1 == ind)
              return NULL;
            return __stub<datatype>::ptr(__data);
          }
          virtual short* ind_ptr() { return &ind; };
          virtual const short* ind_ptr() const { return &ind; };
          virtual const void* ptr() const
          { return __stub<datatype>::c_ptr(__data); }
          virtual const char* char_ptr() const
          { return reinterpret_cast<const char*>(__stub<datatype>::c_ptr(__data)); }
          virtual unsigned short typecode() const 
          {
            if (-1 == ind)
              return SQLT_CHR;
            return TypeCode; 
          }
          virtual size_t size() const
          {
            if (-1 == ind)
              return 0;
            return __size;
          }
          virtual size_t resize(size_t len)
          {
            __stub<datatype>::resize(__data, len);
            __size = __stub<datatype>::size(__data);
            return __size;
          }
          virtual ~__datatype() 
          {
          }
          virtual operator std::wstring() const
          {
            if (-1 == ind)
              return std::wstring();
            return cast_to_wstring(__to_ord_cast(__data));
          }
          virtual operator std::string() const
          {
            return cast_to_string(__to_ord_cast(__data));
          }
          virtual operator int() const 
          {
            if (-1 == ind)
              return int();
            return cast_to_int(__to_ord_cast(__data));
          }
          virtual operator double() const
          { 
            if (-1 == ind)
              return double();
            return cast_to_double(__to_ord_cast(__data));
          }  
          virtual operator blob_container_ptr() const
          {
            if (-1 == ind)
              return blob_container_ptr();
            typename __ord_type<typename __storage_type<TypeCode>::type >::type ptr = __to_ord_cast(__data);
            return cast_to_blob_container_ptr(ptr);
          }
          virtual operator clob_container_ptr() const
          {
            if (-1 == ind)
              return clob_container_ptr();
            typename __ord_type<typename __storage_type<TypeCode>::type >::type ptr = __to_ord_cast(__data);
            return cast_to_clob_container_ptr(ptr);
          }
          virtual operator boost::posix_time::ptime() const
          {
            if (-1 == ind)
              return boost::posix_time::ptime();
            return cast_to_ptime(__to_ord_cast(__data));
          }
        }; 

    };
  };
};



#endif //DATATYPES_H


