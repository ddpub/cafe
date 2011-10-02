#ifndef __ORACLE_DRIVER_H__
#define __ORACLE_DRIVER_H__

#ifndef MAX_FETCH_BUFFER_SIZE
#define MAX_FETCH_BUFFER_SIZE 50000
#endif

#include <string>
#include <list>
#include <map>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/variant.hpp>
#include <boost/shared_array.hpp>
#include <oci.h>

namespace database
{
  namespace oracle
  {
    namespace datatypes
    {
      enum datatype
      {
        incorrect = 0,
        numeric,
        varchar,
        datetime,
        blob,
        clob
      };
    };

    inline size_t buffer_extend_policy(size_t size)
    {
      if (!size)
        return 200;
      size *= 2;
      if (size > MAX_FETCH_BUFFER_SIZE)
        return MAX_FETCH_BUFFER_SIZE;
      return size;
    }

    template <typename HType>
    inline unsigned short get_htype() { return 0; };
#define HANDLE(TYPE, VALUE)                                     \
    template <>                                                 \
    inline unsigned short get_htype<TYPE>() { return VALUE; }; 

    HANDLE(OCIServer, OCI_HTYPE_SERVER)
    HANDLE(OCIError, OCI_HTYPE_ERROR)
    HANDLE(OCIStmt, OCI_HTYPE_STMT)
    HANDLE(OCISvcCtx, OCI_HTYPE_SVCCTX)
    HANDLE(OCISession, OCI_HTYPE_SESSION)
    HANDLE(OCIDateTime, OCI_DTYPE_TIMESTAMP)

    template<typename HType>
    struct __handle_allocation_manager
    {
      static void alloc(HType** hndl, OCIEnv** env)
        {
          OCIHandleAlloc(*env, reinterpret_cast<void**>(hndl), get_htype<HType>(), 0, NULL);
        }
      static void free(HType* hndl)
        {
          OCIHandleFree(hndl, get_htype<HType>());
        }
    };
    template<>
    struct __handle_allocation_manager<OCIEnv>
    {
      static void alloc(OCIEnv** hndl, OCIEnv** env)
        {
          OCIEnvInit(env, OCI_DEFAULT, 0, NULL);
          *hndl = *env;
        }
      static void free(OCIEnv* hndl)
        {
        }
    };
    template<>
    struct __handle_allocation_manager<OCIDateTime>
    {
      static void alloc(OCIDateTime** hndl, OCIEnv** env)
        {
          OCIDescriptorAlloc(*env, reinterpret_cast<void**>(hndl)
                             , get_htype<OCIDateTime>(), 0, NULL);
        }
      static void free(OCIDateTime* hndl)
        {
          OCIDescriptorFree(hndl, get_htype<OCIDateTime>());
        }
    };

    template <typename HType>
    struct __handle
    {
      HType* hndl;
      OCIEnv* __env;
      __handle()
        {
          __handle_allocation_manager<HType>::alloc(&hndl, &__env);
        }
      __handle(OCIEnv* env)
        : __env(env)
        {
          __handle_allocation_manager<HType>::alloc(&hndl, &__env);
        }
      void reset()
        {
          __handle_allocation_manager<HType>::free(hndl);
          __handle_allocation_manager<HType>::alloc(&hndl, &__env);
        }
      ~__handle()
        {
          __handle_allocation_manager<HType>::free(hndl);
        }
    private:
      __handle(const __handle& hndl)
        {
        }
    };
    //******

    typedef std::vector<char> clob_container;
    typedef std::vector<unsigned char> blob_container;
    typedef boost::posix_time::ptime date_time;
    typedef boost::gregorian::date date;

    class exception : public std::exception
    {
      std::string __statement;
      unsigned int __code;
      std::string __message;
      std::string __complete_message;
      void __init_complete_message();
    public:
      exception(int oci_return_value, OCIError* errhp);
      exception(const std::string& statement,
                unsigned int code,
                const std::string& message);
      exception(const std::string& message);
      unsigned int code() const
        {
          return __code;
        }
      const std::string& message()
        {
          return __message;
        }
      virtual const char* what() const throw()
        {
          return const_cast<char*>(__complete_message.c_str());
        }
      virtual ~exception() throw()
        {
        }
    };

    class server
    {
      __handle<OCIEnv> __env;
      __handle<OCIServer> __srv;
      __handle<OCIError> __err;
    public:
      server()
        : __env()
        , __srv(__env.hndl)
        , __err(__env.hndl)
        {
        }
    public:
      OCIEnv* envhp()
        {
          return __env.hndl;
        }
      OCIServer* srvhp()
        {
          return __srv.hndl;
        }
      OCIError* errhp()
        {
          return __err.hndl;
        }
      void attach(const std::string& host, const std::string& database
                         , unsigned short port = 1521);
      void detach();
      bool check_for_error(int retval, bool ignore_no_data = false);
    };

    class session
    {
      server& __server;
      __handle<OCISession> __sess;
      __handle<OCISvcCtx> __ctx;
    public:
      session(server& srv)
        : __server(srv)
        , __sess(__server.envhp())
        , __ctx(__server.envhp())
        {
        }
    public:
      void login(const std::string& username, const std::string& password);
      void logout();
      void commit();
      void rollback();
    public:
      bool check_for_error(int retval, bool ignore_no_data = false)
        {
          return __server.check_for_error(retval, ignore_no_data);
        }
      OCIEnv* envhp()
        {
          return __server.envhp();
        }
      OCIError* errhp()
        {
          return __server.errhp();
        }
      OCISvcCtx* ctxhp()
        {
          return __ctx.hndl;
        }
    };

    struct __base_datatype
    {
      boost::shared_array<OCIIOV> __iov;
      boost::shared_array<short> __indicator;
      __base_datatype();
      virtual void* get_ptr_for_define();
      void* get_ptr(size_t row);
      virtual bool is_null(size_t row);
      virtual short* get_indicator();

      virtual size_t extend() = 0;
      virtual size_t get_data_size() = 0;
      virtual unsigned short get_sqlt_type() = 0;
      virtual datatypes::datatype get_datatype() = 0;
      virtual size_t buffer_size() = 0;


      virtual ~__base_datatype()
        {
        }
    private:
      __base_datatype(const __base_datatype& from)
        {
        }
    };


    class query
    {
      friend class __lob_locator;
    public:
      typedef std::vector<boost::shared_ptr<__base_datatype> > __define_container_t;
      typedef std::list<OCINumber> __ocinumber_container_t;
      typedef std::list<boost::shared_ptr<__handle<OCIDateTime> > > __ocidatetime_container_t;
      typedef std::list<std::string> __str_container_t;
      typedef std::list<std::vector<char> > __out_str_container_t;
      typedef boost::variant<char*, short*, int*, long long*
                             , unsigned char*, unsigned short*, unsigned int*, unsigned long long*
                             , float*, double*, long double*> __numeric_variants_t;
      typedef std::pair<OCINumber*, __numeric_variants_t> __numeric_out_struct_t;
      typedef std::pair<__handle<OCIDateTime>*, date_time*> __datetime_out_struct_t;
      typedef boost::variant<__numeric_out_struct_t, __datetime_out_struct_t, std::vector<char>* > __out_variants_t;
      typedef std::map<std::string, __out_variants_t> __out_container_t;
    private:
      session& __session;
      __handle<OCIStmt> __stmt;
      __define_container_t __define_container;
      std::map<std::string, unsigned int> __column_names;
      __ocinumber_container_t __ocinumber_container;
      __ocidatetime_container_t __ocidatetime_container;
      __str_container_t __str_container;
      std::list<short> __indices;
      __out_container_t __out_container;
      __out_str_container_t __out_str_container;

      size_t __row_fetched;
      size_t __buffer_size;
      size_t __current_position;

      unsigned int __statement_type;
    private:
      void __oci_define(unsigned int dtype, unsigned int length, unsigned int pos);
      bool check_for_error(int retval, bool ignore_no_data = false)
        {
          return __session.check_for_error(retval, ignore_no_data);
        }

      template<typename T>
      inline T __get_numeric(unsigned int column_index, bool unsigned_flag, bool float_flag);
      void __fill_numerics(__numeric_out_struct_t& data);
      void __fill_output_variables();
      template<typename T>
      void __set(unsigned int ind, T* value, size_t size, int sqlt = 0);
      template<typename T>
      void __set(const std::string& ind, T* value, size_t size, int sqlt = 0);
      template<typename TInd, typename T>
      void __set_int(const TInd& ind, T value);
      template<typename TInd, typename T>
      void __set_float(const TInd& ind, T value);
      template<typename T>
      void __set_out_numeric(const std::string& ind, T* value);

      void __clear_containers();

    public:
      query(session& sess)
        : __session(sess)
        , __stmt(sess.envhp())
        , __row_fetched(0)
        , __buffer_size(0)
        , __current_position(0)
        {
        }
    public:
      void prepare(const std::string& statement);
      void execute();
      bool fetch();
      unsigned int column_index(const std::string& name);
      datatypes::datatype column_datatype(unsigned int position);

      void set(const std::string& param_position, long long data);
      void set(const std::string& param_position, int data);
      void set(const std::string& param_position, short data);
      void set(const std::string& param_position, char data);
      void set(const std::string& param_position, unsigned long long data);
      void set(const std::string& param_position, unsigned int data);
      void set(const std::string& param_position, unsigned short data);
      void set(const std::string& param_position, unsigned char data);
      void set(const std::string& param_position, float data);
      void set(const std::string& param_position, double data);
      void set(const std::string& param_position, long double data);
      void set(const std::string& param_position, long long* data);
      void set(const std::string& param_position, int* data);
      void set(const std::string& param_position, short* data);
      void set(const std::string& param_position, char* data);
      void set(const std::string& param_position, const date_time& data);
      void set(const std::string& param_position, const std::string& data);
      void set(const std::string& param_position, unsigned long long* data);
      void set(const std::string& param_position, unsigned int* data);
      void set(const std::string& param_position, unsigned short* data);
      void set(const std::string& param_position, unsigned char* data);
      void set(const std::string& param_position, float* data);
      void set(const std::string& param_position, double* data);
      void set(const std::string& param_position, long double* data);
      void set(const std::string& param_position, date_time* data);
      void set_clob(const std::string& param_position
                    , /* const */ char* strdata, size_t length);
      void set_blob(const std::string& param_position
                    , /* const */ unsigned char* blob_data, size_t length);
      void set_null(const std::string& param_position);

      const std::vector<char>& get_lob(unsigned int column_index);

      const char* get_cstring(unsigned int column_index);
      const date_time& get_datetime(unsigned int column_index);
      long long get_int64(unsigned int column_index);
      int get_int32(unsigned int column_index);
      short get_int16(unsigned int column_index);
      char get_int8(unsigned int column_index);
      float get_float(unsigned int column_index);

      double get_double(unsigned int column_index);
      unsigned long long get_uint64(unsigned int column_index);
      unsigned int get_uint32(unsigned int column_index);
      unsigned short get_uint16(unsigned int column_index);
      unsigned char get_uint8(unsigned int column_index);
      bool is_null(unsigned int column_index);
    };

  };
};


#endif // __ORACLE_DRIVER_H__
