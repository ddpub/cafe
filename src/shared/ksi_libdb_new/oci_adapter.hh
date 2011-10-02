#ifndef OCI_ADAPTER_H
#define OCI_ADAPTER_H
#include "oci_handles.hh"
#include "datatypes.hh"
#include "oci_op.hh"
#include "boost/shared_array.hpp"

namespace database
{
  namespace oracle
  {
    namespace __oci_adapter
    {
      class statement_adapter;
      class oci_adapter
      {
        database::oracle::__oci_handles::__oci_handle<OCIEnv> __env;
        database::oracle::__oci_handles::__oci_handle<OCIError> __err;
        database::oracle::__oci_handles::__oci_handle<OCISvcCtx> __ctx;
        database::oracle::__oci_handles::__oci_handle<OCIServer> __server;
        database::oracle::__oci_handles::__oci_handle<OCISession> __session;
      public:
        oci_adapter();
        void connect(const std::string& server_name, const std::string&
            database_name, unsigned int port);
        void login(const std::string& login, const std::string& password);
        void commit();
        void rollback();
        database::oracle::__oci_handles::__oci_handle<OCIEnv>& env() { return __env; }
        database::oracle::__oci_handles::__oci_handle<OCIError>& err() { return __err; }
        database::oracle::__oci_handles::__oci_handle<OCISvcCtx>& ctx() { return __ctx; }
        ~oci_adapter();
      };

      typedef database::oracle::__datatypes::__datatype_i data_type;
      class statement_adapter
      {
        const database::oracle::__oci_handles::__oci_handle<OCIEnv>& __env;
        database::oracle::__oci_handles::__oci_handle<OCIError>& __err;
        const database::oracle::__oci_handles::__oci_handle<OCISvcCtx>& __svc;
        database::oracle::__oci_handles::__oci_handle<OCIStmt> __stmt;
        std::vector<boost::shared_ptr
          <database::oracle::__datatypes::__datatype_i > >
          __fetched_row;
        std::map<OCIDefine*, boost::shared_ptr<database::oracle::__datatypes::__datatype_i> > __define_map;
        bool before_first;
        std::map<unsigned int, boost::shared_ptr<data_type> > __setted_data;
        std::map<std::string, boost::shared_ptr<data_type> > __setted_by_name_data;
        std::map<std::string, uint32_t> __name_pos;

        void __append_bind_data(const boost::shared_ptr<data_type>& data_ptr,
            unsigned int position);
        void __append_bind_data(const boost::shared_ptr<data_type>& data_ptr,
            const std::string& name);
      public:
        statement_adapter(const database::oracle::__oci_handles::__oci_handle<OCIEnv>& env,
          database::oracle::__oci_handles::__oci_handle<OCIError>& err, const database::oracle::__oci_handles::__oci_handle<OCISvcCtx>& svc);
        void prepare(const std::string& statement);
        void describe();
        unsigned int execute();
        void clear();
        bool next();
        const data_type& get(unsigned int
            position) { return *__fetched_row[position]; }
#define SUPPORTED_TYPES_DECLARATION(Type)\
        void set(Type const& data, const std::string& alias);\
        void set(Type const& data, unsigned int position);
        SUPPORTED_TYPES_DECLARATION(long long int)
        SUPPORTED_TYPES_DECLARATION(std::string)
        SUPPORTED_TYPES_DECLARATION(boost::posix_time::ptime)
        SUPPORTED_TYPES_DECLARATION(double)
        void set_null(unsigned int position);
        void set_null(const std::string& alias);
        boost::shared_array<char> read_lob(OCILobLocator*, unsigned int&);
        database::oracle::__datatypes::__datatype_i* create_oracle_value(unsigned short type, size_t size);
        database::oracle::__datatypes::__datatype_i* create_null_value();
      };
    };
  };
};

#endif //OCI_ADAPTER_H

