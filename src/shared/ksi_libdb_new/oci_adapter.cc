#include "oci_adapter.hh"
#include "oci_attributes.hh"
using namespace database::oracle::__oci_adapter;
using namespace database::oracle::__oci_op;
using namespace database::oracle::__oci_handles;
using namespace database::oracle::__oci_attributes;

namespace database
{
  namespace oracle
  {
    namespace __oci_handles
    {
      template <>
        __oci_handle<OCIEnv>::__oci_handle(OCIEnv* env_handle) :
          __env_handle(NULL)
            {
             __oci_op::__oci_create_environment_default(&__handle, true);
            }
      template <>
        __oci_handle<OCIEnv>::~__oci_handle()
            {
            }
    };
  };
}; 


__oci_handle<OCIEnv> database::oracle::__oci_handles::environment(NULL);


oci_adapter::oci_adapter()
  : __env(environment)
  , __err(__env.__handle)
  , __ctx(__env.__handle)
  , __server(__env.__handle)
  , __session(__env.__handle)
{

}


void oci_adapter::connect(const std::string& server_name, const std::string&
    database_name, unsigned int port)
{
  __oci_server_attach(__server.__handle, __err.__handle, server_name,
      database_name, port);
}

void oci_adapter::login(const std::string& login, const std::string& password)
{
  __oci_attach_server_handle(__ctx.__handle, __server.__handle,
      __err.__handle);
  __oci_session_set_login_information(__session.__handle, login, password,
      __err.__handle);
  __oci_session_begin(__ctx.__handle, __session.__handle,
      __err.__handle);
  __oci_attach_session_handle(__ctx.__handle, __session.__handle,
      __err.__handle);
}

void oci_adapter::commit()
{
  __oci_trans_commit(__ctx.__handle, __err.__handle);
}  

void oci_adapter::rollback()
{
  __oci_trans_rollback(__ctx.__handle, __err.__handle);
}

oci_adapter::~oci_adapter()
{
  __oci_trans_rollback(__ctx.__handle, __err.__handle, false);
  __oci_session_end(__ctx.__handle, __session.__handle, __err.__handle, false);
  __oci_server_detach(__server.__handle, __err.__handle, false);
}


statement_adapter::statement_adapter(const __oci_handle<OCIEnv>& env, 
    __oci_handle<OCIError>& err, const __oci_handle<OCISvcCtx>& svc)
  : __env(env)
  , __err(err)
  , __svc(svc)
  , __stmt(env.__handle)
{
}

void statement_adapter::prepare(const std::string& statement)
{
  if (!statement.size())
    __stmt = __oci_handle<OCIStmt>(__env.__handle);
  __oci_stmt_prepare(__stmt.__handle, __err.__handle,  statement, true);
}

unsigned int statement_adapter::execute()
{
  __oci_stmt_execute(__stmt.__handle, __err.__handle, __svc.__handle,
      OCI_DEFAULT, true);
  return __attr_getter<OCI_ATTR_ROW_COUNT>::get(__stmt.__handle, __err.__handle);
}


void statement_adapter::describe()
{
  unsigned int stmt_type =__attr_getter<OCI_ATTR_STMT_TYPE>::get(__stmt.__handle,
      __err.__handle);
  if (OCI_STMT_SELECT != stmt_type)
    return;
  __oci_stmt_execute(__stmt.__handle, __err.__handle, __svc.__handle,
      OCI_DESCRIBE_ONLY, true);


  unsigned int param_count = __attr_getter<OCI_ATTR_PARAM_COUNT>::get(__stmt.__handle, __err.__handle);
  __fetched_row.resize(param_count);
  for (unsigned int pos = 1; pos <= param_count; ++pos)
  {
    OCIParam* param = __get_param<OCIStmt>(__stmt.__handle, __err.__handle, pos);
    std::string name = __attr_getter<OCI_ATTR_NAME>::get(param, __err.__handle);
    unsigned short char_used = 0;
    get_attribute(param, &char_used, OCI_ATTR_CHAR_USED, __err.__handle);
    unsigned short typecode = 0;
    get_attribute(param, &typecode, OCI_ATTR_DATA_TYPE, __err.__handle);
    unsigned int len = 0;
    if (char_used)
      get_attribute(param, &len, OCI_ATTR_CHAR_SIZE, __err.__handle);
    else
      get_attribute(param, &len, OCI_ATTR_DATA_SIZE, __err.__handle);
    database::oracle::__datatypes::__datatype_i* p_value = create_oracle_value(typecode, len);
    boost::shared_ptr< database::oracle::__datatypes::__datatype_i >
      val_ptr(p_value);
    OCIDefine* defp;
    __oci_define_by_pos(__stmt.__handle, &defp, __err.__handle, pos, val_ptr->ptr(),
        val_ptr->size(), val_ptr->typecode(), val_ptr->ind_ptr(), SQLT_LNG == typecode, true);
    __fetched_row[pos - 1] = val_ptr;
  }
}

void statement_adapter::clear()
{
  before_first = true;
}

void statement_adapter::__append_bind_data(const boost::shared_ptr<data_type>&
    data_ptr, unsigned int position)
{
  if (__setted_data.find(position) != __setted_data.end())
    __setted_data[position] = data_ptr;
  else
  {
    std::pair<unsigned int, boost::shared_ptr<data_type> > pair_data
      (position, data_ptr);
    __setted_data.insert(pair_data);
  }
  database::oracle::__oci_op::__oci_bind_by_pos(__stmt.__handle, __err.__handle, position,
      data_ptr->ptr(), data_ptr->size(), data_ptr->typecode(),
      data_ptr->ind_ptr());
}

void statement_adapter::__append_bind_data(const boost::shared_ptr<data_type>& data_ptr,
    const std::string& name)
{
  if (__setted_by_name_data.find(name) != __setted_by_name_data.end())
    __setted_by_name_data[name] = data_ptr;
  else
  {
    std::pair<std::string, boost::shared_ptr<data_type> > pair_data
      (name, data_ptr);
    __setted_by_name_data.insert(pair_data);
  }
  database::oracle::__oci_op::__oci_bind_by_name(__stmt.__handle, __err.__handle, name,
      data_ptr->ptr(), data_ptr->size(), data_ptr->typecode(),
      data_ptr->ind_ptr());
}

template <typename From>
  database::oracle::__datatypes::__datatype_i* convert_to_oracle_value(From const& from)
  {
    using namespace database::oracle::__datatypes;
    __datatype_i* ret = new
      __datatype<__ord_type_code<From>::TYPE_CODE>(__to_cpp<From>::cast(from), 0);
    return ret;
  }

#define SUPPORTED_TYPES_DEFINITION(Type)\
        void statement_adapter::set(Type const& data, const std::string& alias)\
        {\
          boost::shared_ptr<data_type> data_ptr\
            (convert_to_oracle_value(data));\
          __append_bind_data(data_ptr, alias[0] == ':' ? alias : (std::string(":") + alias));\
        }\
        void statement_adapter::set(Type const& data, unsigned int position)\
        {\
          boost::shared_ptr<data_type> data_ptr\
            (convert_to_oracle_value(data));\
          __append_bind_data(data_ptr, position);\
        }

SUPPORTED_TYPES_DEFINITION(long long int)
SUPPORTED_TYPES_DEFINITION(std::string)
SUPPORTED_TYPES_DEFINITION(boost::posix_time::ptime)
SUPPORTED_TYPES_DEFINITION(double)


void statement_adapter::set_null(unsigned int position)
{
  boost::shared_ptr<data_type> data_ptr (create_null_value());
  __append_bind_data(data_ptr, position);
}

void statement_adapter::set_null(const std::string& alias)
{
  boost::shared_ptr<data_type> data_ptr
    (create_null_value());
  __append_bind_data(data_ptr, alias);
}

bool statement_adapter::next()
{
  bool data_need = false;
  bool fetch_status = __oci_stmt_fetch(__stmt.__handle, __err.__handle, 1, data_need, true);
/*  while (data_need)
  {
    OCIDefine* defhp;
    unsigned int type;
    unsigned char inout_flag;
    unsigned int iter_num;
    unsigned int ind;
    unsigned char piece_desc;
    __oci_stmt_get_piece_info(__stmt.__handle, __err.__handle, (void**) &defhp, &type, &inout_flag, &iter_num, &ind, &piece_desc);
    boost::shared_ptr<data_type> data = __define_map[defhp];
    unsigned short rcode = 0;
    unsigned int len = 4000;
    piece_desc = OCI_FIRST_PIECE;
    __oci_stmt_set_piece_info(defhp, type, __err.__handle, data->allocate_chunk(), &len, piece_desc, &ind, &rcode);
    fetch_status = __oci_stmt_fetch(__stmt.__handle, __err.__handle, 1, data_need, true);
  }*/
  return fetch_status;
}

boost::shared_array<char> statement_adapter::read_lob(OCILobLocator* lob,
    unsigned int& col_width)
{
  __oci_lob_get_length(__svc.__handle, __err.__handle, lob, (&col_width));
  boost::shared_array<char> ret (new char[col_width]);
  unsigned long long byte_ampt = col_width;
  unsigned long long char_ampt = 0;
  if (col_width)
    __oci_lob_read(__svc.__handle, __err.__handle, lob, &byte_ampt,
        &char_ampt, ret.get(), col_width);
  else
    ret.reset(new char[0]);
  return ret;
}


using namespace database::oracle::__datatypes;
using namespace boost;
using namespace std;

__datatype_i* statement_adapter::create_oracle_value(unsigned short type, size_t size)
{
  __datatype_i* retval = NULL;
  switch (type)
  {
  case OCI_TYPECODE_INTEGER:
    retval = new __datatype<SQLT_INT>(0);
    break;
  case OCI_TYPECODE_NUMBER:
  case OCI_TYPECODE_FLOAT:
    retval = new __datatype<SQLT_FLT>(size_t(0));
    break;
  case OCI_TYPECODE_VARCHAR2:
  case OCI_TYPECODE_VARCHAR:
  case OCI_TYPECODE_CHAR:
    retval = new __datatype<SQLT_STR>(size + 1);
    break;
  case OCI_TYPECODE_DATE:
  case OCI_TYPECODE_TIMESTAMP:
    retval = new __datatype<SQLT_ODT>(0);
    break;
  case OCI_TYPECODE_BLOB:
    retval = new __datatype<SQLT_BLOB>(0);
    dynamic_cast<__datatype<SQLT_BLOB>*>(retval)->__data->initialize(__env.__handle, __svc.__handle, __err.__handle);
    break;
  case OCI_TYPECODE_CLOB:
    retval = new __datatype<SQLT_CLOB>(0);
    dynamic_cast<__datatype<SQLT_CLOB>*>(retval)->__data->initialize(__env.__handle, __svc.__handle, __err.__handle);
    break;
  default:
    throw std::runtime_error(lexical_cast<string>(type));
  };
  return retval;
}

__datatype_i* statement_adapter::create_null_value()
{
  return new __datatype<SQLT_INT>();
}
