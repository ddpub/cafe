#include "oracle_driver.h"


#include <sstream>
#include <vector>
#include <list>

#include <boost/foreach.hpp>

using namespace std;

using namespace database;
using namespace oracle;


database::oracle::exception::exception(const std::string& statement,
                unsigned int code,
                const std::string& message)
  : __statement(statement)
  , __code(code)
  , __message(message)
{
  __init_complete_message();
}

database::oracle::exception::exception(const std::string& message)
  : __statement()
  , __code(0)
  , __message(message)
{
  __init_complete_message();
}

void database::oracle::exception::__init_complete_message()
{
  stringstream ss;
  ss << "Error in oracle driver. ";
  if (__code)
    ss << "Code: " << __code << ". ";
  if (__message.length())
    ss << "Message: " << __message << ". ";
  __complete_message = ss.str();
}

database::oracle::exception::exception(int oci_return_value, OCIError* errhp)
  : __code(0)
{
  text errbuf[512];
  sb4 errcode = 0;

  switch (oci_return_value)
  {
  case OCI_SUCCESS:
    break;
  case OCI_SUCCESS_WITH_INFO:
    __message = "Success with info (Driver bug)";
    break;
  case OCI_NEED_DATA:
    __message = "Need data (Driver bug)";
    break;
  case OCI_NO_DATA:
    __message = "No more data for fetch";
    break;
  case OCI_ERROR:
    OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
                       errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    __code = errcode;
    if (12545 == __code)
    {
      __message = "Incorrect host or database name, or connection problem";
      break;
    }
    __message = string((char*)errbuf);
    break;
  case OCI_INVALID_HANDLE:
    __message = "Invalid handle (Driver Bug)";
    break;
  case OCI_STILL_EXECUTING:
    __message = "Still executing (Probably driver bug)";
    break;
  case OCI_CONTINUE:
    __message = "Continue (Probably driver bug)";
    break;
  default:
    break;
  }
  __init_complete_message();
}

bool database::oracle::server::check_for_error(int retval, bool ignore_no_data)
{
  if (OCI_SUCCESS == retval)
    return false;
  if (OCI_NO_DATA == retval && ignore_no_data)
    return true;
  throw exception(retval, __err.hndl);
}

void database::oracle::server::attach(const std::string& host, const std::string& database, unsigned short port)
{
  stringstream connection_string;
  connection_string << "(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)"
                    << "(HOST=" << host << ")"
                    << "(PORT=" << port << ")))(CONNECT_DATA="
                    << "(SERVICE_NAME=" << database << ")))";

  check_for_error(OCIServerAttach(__srv.hndl, __err.hndl,
                                  (text *)connection_string.str().c_str()
                                  , connection_string.str().length() + 1
                                  , OCI_DEFAULT));
}

void database::oracle::server::detach()
{
  check_for_error(OCIServerDetach(__srv.hndl, __err.hndl, OCI_DEFAULT));
}


void database::oracle::session::login(const std::string& username, const std::string& password)
{
  string user = username;
  string pass = password;
  check_for_error(OCIAttrSet(__ctx.hndl, OCI_HTYPE_SVCCTX,
                             __server.srvhp(), 0, OCI_ATTR_SERVER, __server.errhp()));
  check_for_error(OCIAttrSet(__ctx.hndl, OCI_HTYPE_SVCCTX,
                             __sess.hndl, 0, OCI_ATTR_SESSION, __server.errhp()));
  check_for_error(OCIAttrSet(__sess.hndl, OCI_HTYPE_SESSION
                             , &user[0], user.length()
                             , OCI_ATTR_USERNAME, __server.errhp()));
  check_for_error(OCIAttrSet(__sess.hndl, OCI_HTYPE_SESSION
                             , &pass[0], pass.length()
                             , OCI_ATTR_PASSWORD, __server.errhp()));
  check_for_error(OCISessionBegin(__ctx.hndl, __server.errhp(), __sess.hndl,
                                  OCI_CRED_RDBMS, OCI_DEFAULT));
}

void database::oracle::session::logout()
{
  check_for_error(OCISessionEnd(__ctx.hndl, __server.errhp(), __sess.hndl, OCI_DEFAULT));
}

void database::oracle::session::commit()
{
  check_for_error(OCITransCommit(__ctx.hndl, __server.errhp(), 0));
}

void database::oracle::session::rollback()
{
  check_for_error(OCITransRollback(__ctx.hndl, __server.errhp(), 0));
}


//**************************** Query **************************\\




template<datatypes::datatype DataType>
struct __define_storage_info
{
  typedef void* storage_type;
  enum enm_sqlt
  {
    SQLT_TYPE_NUMBER = 0
  };
};
#define DEFINE_STORAGE_TYPE(DATABASE_TYPE, SQLT_TYPE, STORAGE_TYPE) template<> \
struct __define_storage_info<DATABASE_TYPE> \
  {                                     \
  typedef STORAGE_TYPE storage_type;    \
  enum enm_sqlt \
{ \
  sqlt_type_id = SQLT_TYPE \
    };                         \
  }; 
//  typedef __define_storage_info<DATABASE_TYPE>::storage_type  DATABASE_TYPE ## _storage;
struct __lob_locator
{
  OCILobLocator* locator;
  database::oracle::session* __session;
  std::vector<char> __fetched_data;
  __lob_locator(database::oracle::session* sess)
    : __session(sess)
    {
      OCIDescriptorAlloc(__session->envhp(), reinterpret_cast<void**>(&locator), OCI_DTYPE_LOB, 0, NULL);
    }
  const std::vector<char>& fetch()
    {
      if (!__fetched_data.size())
      {
        unsigned int size;
        __session->check_for_error(OCILobGetLength(__session->ctxhp(), __session->errhp(), locator, &size));
        __fetched_data.reserve(size + 1);
        __fetched_data.resize(size);
        __fetched_data.push_back('\0');
        __session->check_for_error(OCILobRead(__session->ctxhp(), __session->errhp(), locator, &size, 1,
                                            &__fetched_data[0], size, NULL, NULL, 0, 0));
      }
      return __fetched_data;
    }
  ~__lob_locator()
    {
      OCIDescriptorFree(locator, OCI_DTYPE_LOB);
    }
};


void ocidatetime_to_ptime(OCIDateTime* odt, date_time& dt, database::oracle::session* sess)
{
  signed short year = 0;
  unsigned char month = 0;
  unsigned char day = 0;
  OCIDateTimeGetDate(sess->envhp(), sess->errhp()
                     , odt, &year, &month, &day);
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
  unsigned int nanosecond;
  OCIDateTimeGetTime(sess->envhp(), sess->errhp()
                     , odt, &hour, &minute, &second, &nanosecond);
  dt = date_time(date(year, month, day)
                 , boost::posix_time::hours(hour)
                 + boost::posix_time::minutes(minute)
                 + boost::posix_time::seconds(second)
                 + boost::date_time::subsecond_duration<
                 boost::posix_time::time_duration, 1000000000
                 >(nanosecond));
}

struct __oci_datetime
{
  OCIDateTime* dt;
  database::oracle::session* __session;
  date_time parsed_dt;
  __oci_datetime(database::oracle::session* sess)
    : __session(sess)
    {
      OCIDescriptorAlloc(__session->envhp(), reinterpret_cast<void**>(&dt)
                         , OCI_DTYPE_TIMESTAMP, 0, NULL);
    }
  const date_time& get_datetime()
    {
      if (!parsed_dt.is_not_a_date_time())
        return parsed_dt;
      ocidatetime_to_ptime(dt, parsed_dt, __session);
      return parsed_dt;
    }

  ~__oci_datetime()
    {
      OCIDescriptorFree(dt, OCI_DTYPE_TIMESTAMP);
    }
};

DEFINE_STORAGE_TYPE(datatypes::numeric, SQLT_VNU, OCINumber)
typedef __define_storage_info<datatypes::numeric>::storage_type numeric_storage;
DEFINE_STORAGE_TYPE(datatypes::datetime, SQLT_TIMESTAMP, __oci_datetime)
typedef __define_storage_info<datatypes::datetime>::storage_type datetime_storage;
DEFINE_STORAGE_TYPE(datatypes::varchar, SQLT_STR, vector<char>)
typedef __define_storage_info<datatypes::varchar>::storage_type varchar_storage;
DEFINE_STORAGE_TYPE(datatypes::clob, SQLT_CLOB, __lob_locator)
typedef __define_storage_info<datatypes::clob>::storage_type clob_storage;
DEFINE_STORAGE_TYPE(datatypes::blob, SQLT_BLOB, __lob_locator)
typedef __define_storage_info<datatypes::clob>::storage_type clob_storage;

//******* Helpers for complex types

template<typename Stor>
void* ptr(Stor& data)
{
  return &data;
}
template<>
void* ptr<varchar_storage>(varchar_storage& data)
{
  return &(data[0]);
}
template<>
void* ptr<clob_storage>(clob_storage& data)
{
  return &data.locator;
}
template<>
void * ptr<datetime_storage>(datetime_storage& data)
{
  return &data.dt;
}

template<typename Stor>
size_t size(Stor& data)
{
  return sizeof(data);
}

template<>
size_t size<varchar_storage>(varchar_storage& data)
{
  return data.size();
}
template<>
size_t size<clob_storage>(clob_storage& data)
{
  return sizeof(OCILobLocator*);
}
template<>
size_t size<datetime_storage>(datetime_storage& data)
{
  return sizeof(OCIDateTime*);
}

template<typename Stor>
inline Stor* allocate(size_t size, database::oracle::session* sess)
{
  return new Stor;
}
template<>
inline varchar_storage* allocate<varchar_storage>(size_t size, database::oracle::session* sess)
{
  return new varchar_storage(size + 1);
}
template<>
inline clob_storage* allocate<clob_storage>(size_t size, database::oracle::session* sess)
{
  return new clob_storage(sess);
}
template<>
inline datetime_storage* allocate<datetime_storage>(size_t size, database::oracle::session* sess)
{
  return new datetime_storage(sess);
}

__base_datatype::__base_datatype()
{
  try
  {
    __iov.reset(new OCIIOV[MAX_FETCH_BUFFER_SIZE]);
    __indicator.reset(new short[MAX_FETCH_BUFFER_SIZE]);
  }
  catch (bad_alloc& e)
  {
    throw oracle::exception("Bad alloc (Driver bug TODO)");
  }
}

void* __base_datatype::get_ptr_for_define()
{
  return __iov.get();
}

void* __base_datatype::get_ptr(size_t row)
{
  return __iov[row].bfp;
}

bool __base_datatype::is_null(size_t row)
{
  return __indicator[row] == -1;
}

short* __base_datatype::get_indicator()
{
  return __indicator.get();
}


template<datatypes::datatype Data_Type>
struct __define_datatype : public __base_datatype
{
  typedef typename __define_storage_info<Data_Type>::storage_type container_type;
  typedef std::vector<boost::shared_ptr<container_type> > storage_type;
  database::oracle::session* __session;

  size_t __target_size;
  storage_type __container;

  __define_datatype(size_t size, database::oracle::session* sess);

  virtual size_t get_data_size();
  virtual size_t extend();
  virtual unsigned short get_sqlt_type();
  virtual datatypes::datatype get_datatype();
  virtual size_t buffer_size();

  container_type& get_ref(size_t row);

  virtual ~__define_datatype();
};

template<datatypes::datatype Data_Type>
__define_datatype<Data_Type>::__define_datatype(size_t size, database::oracle::session* sess)
  : __base_datatype()
  , __target_size(size)
  , __session(sess)
{
  __container.reserve(MAX_FETCH_BUFFER_SIZE);
}

template<datatypes::datatype Data_Type>
size_t __define_datatype<Data_Type>::get_data_size()
{
  return __target_size;
}

template<datatypes::datatype Data_Type>
unsigned short __define_datatype<Data_Type>::get_sqlt_type()
{
  return __define_storage_info<Data_Type>::sqlt_type_id;
}

template<datatypes::datatype Data_Type>
datatypes::datatype __define_datatype<Data_Type>::get_datatype()
{
  return Data_Type;
}


template<datatypes::datatype Data_Type>
size_t __define_datatype<Data_Type>::buffer_size()
{
  return __container.size();
}

template<datatypes::datatype Data_Type>
size_t __define_datatype<Data_Type>::extend()
{
  size_t new_size = buffer_extend_policy(__container.size());
  for (; __container.size() < new_size;)
  {
    typename storage_type::value_type new_data (allocate<container_type>(__target_size, __session));
    __container.push_back(new_data);
    __iov[__container.size() - 1].bfp = ptr(*new_data);
    __iov[__container.size() - 1].bfl = size(*new_data);
  }
  return new_size;
}

// TODO: HACK
template<datatypes::datatype Data_Type>
typename __define_datatype<Data_Type>::container_type& __define_datatype<Data_Type>::get_ref(size_t row)
{
  return *__container[row];
}

template<datatypes::datatype Data_Type>
__define_datatype<Data_Type>::~__define_datatype()
{
}

void database::oracle::query::__oci_define(unsigned int dtype
                                           , unsigned int length
                                           , unsigned int pos)
{
  __define_container_t::value_type define_storage;
  ub2 type;
  switch (dtype)
  {
  case OCI_TYPECODE_FLOAT:
  case OCI_TYPECODE_DOUBLE:
  case OCI_TYPECODE_INTEGER:
  case OCI_TYPECODE_NUMBER:
  case OCI_TYPECODE_DECIMAL:
    define_storage.reset(new __define_datatype<datatypes::numeric>(sizeof(OCINumber), &__session));
    break;
  case OCI_TYPECODE_CHAR:
  case OCI_TYPECODE_VARCHAR:
  case OCI_TYPECODE_VARCHAR2:
    define_storage.reset(new __define_datatype<datatypes::varchar>(length, &__session));
    break;
  case OCI_TYPECODE_DATE:
  case OCI_TYPECODE_TIMESTAMP:
    define_storage.reset(new __define_datatype<datatypes::datetime>(sizeof(OCIDateTime*), &__session));
    break;
  case OCI_TYPECODE_CLOB:
    define_storage.reset(new __define_datatype<datatypes::clob>(sizeof(OCILobLocator*), &__session));
    break;
  default:
    throw oracle::exception("Invalid datatype (Driver bug)");
  }
  __define_container.push_back(define_storage);
  OCIDefine* defhp;
  check_for_error(OCIDefineByPos(__stmt.hndl, &defhp, __session.errhp()
                                 , pos
                                 , define_storage->get_ptr_for_define()
                                 , define_storage->get_data_size()
                                 , define_storage->get_sqlt_type()
                                 , define_storage->get_indicator()
                                 , NULL , NULL, OCI_IOV));
}


void database::oracle::query::prepare(const std::string& statement)
{
  __stmt.reset();
  __clear_containers();
  check_for_error(OCIStmtPrepare(__stmt.hndl, __session.errhp()
                                 , (text*) statement.c_str(), statement.length()
                                 , OCI_NTV_SYNTAX, OCI_DEFAULT));
  __statement_type = 0;
  check_for_error(OCIAttrGet(__stmt.hndl, OCI_HTYPE_STMT
                             , &__statement_type, 0
                             , OCI_ATTR_STMT_TYPE, __session.errhp()));

  if (OCI_STMT_SELECT != __statement_type)
    return;

  check_for_error(OCIStmtExecute(__session.ctxhp(), __stmt.hndl, __session.errhp()
                                 , 1, 0, NULL, NULL, OCI_DESCRIBE_ONLY));
  int col_count = 0;
  check_for_error(OCIAttrGet(__stmt.hndl, OCI_HTYPE_STMT
                             , (dvoid *)&col_count, 0
                             , OCI_ATTR_PARAM_COUNT, __session.errhp()));

  OCIParam *colhd = (OCIParam *) 0;
  ub2 type;
  ub4 length;
  __define_container.reserve(col_count);
  for (int i = 1; i <= col_count; ++i)
  {
    check_for_error(OCIParamGet(__stmt.hndl, OCI_HTYPE_STMT, __session.errhp()
                                , (void**)&colhd, i));
    type = 0;
    check_for_error(OCIAttrGet(colhd, OCI_DTYPE_PARAM, &type, 0, OCI_ATTR_DATA_TYPE
                               , __session.errhp()));

    char* name;
    ub4 name_sz;
    check_for_error(OCIAttrGet(colhd, OCI_DTYPE_PARAM, &name, &name_sz, OCI_ATTR_NAME
                               , __session.errhp()));
    string str_name(name, name_sz);
    std::transform(str_name.begin(), str_name.end(), str_name.begin(), (int(*)(int)) std::toupper);
    __column_names[str_name] = i - 1;

    length = 0;
    check_for_error(OCIAttrGet(colhd, OCI_DTYPE_PARAM, &length, 0, OCI_ATTR_DATA_SIZE
                               , __session.errhp()));
//    __OCI_OP(__oci_define(i, type, length), "");
    __oci_define(type, length, i);
  }
}



void database::oracle::query::execute()
{
  __current_position = 0;
  __buffer_size = 0;
  __row_fetched = 0;
  check_for_error(OCIStmtExecute(__session.ctxhp(), __stmt.hndl, __session.errhp()
                                 , __statement_type == OCI_STMT_SELECT ? 0 : 1
                                 , 0 , NULL, NULL, OCI_DEFAULT));
  __fill_output_variables();
}

bool database::oracle::query::fetch()
{
  if (__current_position && (__current_position >= __buffer_size
                             || __current_position >= __row_fetched))
    throw oracle::exception("Cursor cannot go over the edge of fetch buffer");

  ++__current_position;
  if (!__buffer_size
      || (__current_position && __current_position == __buffer_size))
  {
    for (__define_container_t::iterator def_iter = __define_container.begin();
         def_iter != __define_container.end(); ++def_iter)
      __buffer_size = (*def_iter)->extend();

    check_for_error(OCIStmtFetch(__stmt.hndl, __session.errhp()
                                 , __buffer_size,
                                 OCI_FETCH_NEXT, OCI_DEFAULT), true);
    check_for_error(OCIAttrGet(__stmt.hndl, OCI_HTYPE_STMT, &__row_fetched,
                               0, OCI_ATTR_ROWS_FETCHED, __session.errhp()));
    if (!__row_fetched )
      return false;
    else
    {
      __current_position = 0;
      return true;
    }
  }
  if (__current_position == __row_fetched)
    return false;
  return true;
}

unsigned int database::oracle::query::column_index(const std::string& name)
{
  return __column_names[name];
}

datatypes::datatype database::oracle::query::column_datatype(unsigned int position)
{
  return __define_container[position]->get_datatype();
}

template<datatypes::datatype Data_Type>
inline typename __define_datatype<Data_Type>::container_type&
extract_dynamic(query::__define_container_t& data, unsigned int index, size_t row)
{
  __base_datatype& base = *data[index];
  __define_datatype<Data_Type>& deftype_data =
    dynamic_cast<__define_datatype<Data_Type>&>(base);
  //if (!deftype_data) throw runtime_error("Get type differs from type of field");
  return deftype_data.get_ref(row);
}

template<datatypes::datatype Data_Type>
inline typename __define_datatype<Data_Type>::container_type&
extract_reinterpret(query::__define_container_t& data, unsigned int index, size_t row)
{
  return *reinterpret_cast<
    typename __define_datatype<Data_Type>::container_type*>(
      data[index].get()->get_ptr(row));
}

template<datatypes::datatype Data_Type>
inline typename __define_datatype<Data_Type>::container_type&
extract(query::__define_container_t& data, unsigned int index, size_t row)
{
  return extract_dynamic<Data_Type>(data, index, row);
}

template <typename T>
inline short cast_oci_number(OCINumber number, T& target
                             , bool unsigned_flag, bool float_flag
                             , OCIError* errhp)
{
  return float_flag
    ? OCINumberToReal(errhp, &number, sizeof(T), &target)
    : OCINumberToInt(errhp, &number, sizeof(T)
                      , unsigned_flag ? OCI_NUMBER_UNSIGNED : OCI_NUMBER_SIGNED, &target);
}

template <typename T>
inline T query::__get_numeric(unsigned int index, bool unsigned_flag, bool float_flag)
{
  T target;
  if (__define_container[index]->is_null(__current_position))
    return 0;
  check_for_error(cast_oci_number<T>(
                    extract<datatypes::numeric>(__define_container, index, __current_position)
                    , target
                    , unsigned_flag, float_flag
                    , __session.errhp()));
  return target;
}

const std::vector<char>& database::oracle::query::get_lob(unsigned int column_index)
{
  datatypes::datatype type = __define_container[column_index]->get_datatype();
  if (datatypes::clob == type)
  {
    __define_datatype<datatypes::clob>::container_type& data = 
      extract<datatypes::clob>(__define_container, column_index, __current_position);
    return data.fetch();
  }
  if (datatypes::blob == type)
  {
    __define_datatype<datatypes::blob>::container_type& data = 
      extract<datatypes::blob>(__define_container, column_index, __current_position);
    return data.fetch();
  }
}

const char* database::oracle::query::get_cstring(unsigned int column_index)
{
  datatypes::datatype type = __define_container[column_index]->get_datatype();
  if (datatypes::varchar == type)
  {
    __define_datatype<datatypes::varchar>::container_type& data =
      extract<datatypes::varchar>(__define_container, column_index, __current_position);
    return &(data[0]);
  }
  if (datatypes::clob == type)
  {
    __define_datatype<datatypes::clob>::container_type& data =
      extract<datatypes::clob>(__define_container, column_index, __current_position);
    return &data.fetch()[0];
  }
}

const date_time& database::oracle::query::get_datetime(unsigned int column_index)
{
  if (datatypes::datetime != __define_container[column_index]->get_datatype())
    throw database::oracle::exception("Type of this field is not a datetime");
  __define_datatype<datatypes::datetime>::container_type& data =
    extract<datatypes::datetime>(__define_container, column_index, __current_position);
  return data.get_datetime();
}

long long database::oracle::query::get_int64(unsigned int column_index)
{
  return __get_numeric<long long>(column_index, false, false);
}

int database::oracle::query::get_int32(unsigned int column_index)
{
  return __get_numeric<int>(column_index, false, false);
}

short database::oracle::query::get_int16(unsigned int column_index)
{
  return __get_numeric<short>(column_index, false, false);
}

char database::oracle::query::get_int8(unsigned int column_index)
{
  return __get_numeric<char>(column_index, false, false);
}

float database::oracle::query::get_float(unsigned int column_index)
{
  return __get_numeric<float>(column_index, false, true);
}

double database::oracle::query::get_double(unsigned int column_index)
{
  return __get_numeric<double>(column_index, false, true);
}

unsigned long long database::oracle::query::get_uint64(unsigned int column_index)
{
  return __get_numeric<unsigned long long>(column_index, true, false);
}

unsigned int database::oracle::query::get_uint32(unsigned int column_index)
{
  return __get_numeric<unsigned int>(column_index, true, false);
}

unsigned short database::oracle::query::get_uint16(unsigned int column_index)
{
  return __get_numeric<unsigned short>(column_index, true, false);
}

unsigned char database::oracle::query::get_uint8(unsigned int column_index)
{
  return __get_numeric<unsigned char>(column_index, true, false);
}

bool database::oracle::query::is_null(unsigned int column_index)
{
  return __define_container[column_index]->is_null(__current_position);
}


template<typename T>
struct oci_int_type_rule
{
  enum sign_flag_en { sign_flag = 0 };
};
#define INT_TYPE_RULE(TYPE)                                   \
  template<>                                                  \
  struct oci_int_type_rule<unsigned TYPE>                     \
  {                                                           \
    enum sign_flag_en { sign_flag = OCI_NUMBER_UNSIGNED };    \
  };                                                          \
  template<>                                                  \
  struct oci_int_type_rule<TYPE>                              \
  {                                                           \
    enum sign_flag_en { sign_flag = OCI_NUMBER_SIGNED };      \
  };
INT_TYPE_RULE(char)
INT_TYPE_RULE(int)
INT_TYPE_RULE(long)
INT_TYPE_RULE(long long)

template<typename T>
struct __bind_storage_type
{
  enum datatype_enm { sqlt_dt = 0 };
};
template<datatypes::datatype Dt>
struct __bind_out_param_type
{
  enum datatype_enm { sqlt_dt = 0 };
};
#define BIND_STORAGE_TYPE(TYPE, SQLT, DT)           \
  template<>                                        \
  struct __bind_storage_type<TYPE>                  \
  {                                                 \
    enum datatype_enm { sqlt_dt = SQLT };           \
  };                                                \
  template<>                                        \
  struct __bind_out_param_type<DT>                  \
  {                                                 \
    enum datatype_enm { sqlt_dt = SQLT };           \
  };


BIND_STORAGE_TYPE(OCINumber, SQLT_VNU, datatypes::numeric)
BIND_STORAGE_TYPE(OCIDateTime*, SQLT_TIMESTAMP, datatypes::datetime)
BIND_STORAGE_TYPE(char, SQLT_STR, datatypes::varchar)

template<typename CType>
void int_to_ocinumber(CType value, OCINumber* num, database::oracle::session& sess)
{
  OCINumberFromInt(sess.errhp(), &value, sizeof(CType), oci_int_type_rule<CType>::sign_flag, num);
}

template<typename CType>
void real_to_ocinumber(CType value, OCINumber* num, database::oracle::session& sess)
{
  OCINumberFromReal(sess.errhp(), &value, sizeof(CType), num);
}

void datetime_to_ocidatetime(OCIDateTime* dthp, const database::oracle::date_time& dt
                             , database::oracle::session& sess)
{
  OCIDateTimeConstruct(sess.envhp(), sess.errhp(), dthp
                       , dt.date().year(), dt.date().month(), dt.date().day()
                       , dt.time_of_day().hours(), dt.time_of_day().minutes()
                       , dt.time_of_day().seconds(), dt.time_of_day().seconds()
                       , NULL, 0);
}

template<typename T>
void database::oracle::query::__set(unsigned int ind, T* value, size_t size, int sqlt)
{
  __indices.push_back(0);
  short& indp = __indices.back();
  OCIBind* bindhp;
  check_for_error(OCIBindByPos(__stmt.hndl, &bindhp, __session.errhp()
                               , ind
                               , value, size
                               , sqlt ? __bind_storage_type<T>::sqlt_dt : sqlt
                               , &indp
                               , NULL, NULL, 0, NULL, OCI_DEFAULT));
}

template<typename T>
void database::oracle::query::__set(const string& ind, T* value, size_t size, int sqlt)
{
  if (!value)
    __indices.push_back(-1);
  else
    __indices.push_back(0);
  short& indp = __indices.back();
  OCIBind* bindhp;
  check_for_error(OCIBindByName(__stmt.hndl, &bindhp, __session.errhp()
                                , (text*) ind.c_str(), ind.length()
                                , value, size
                                , sqlt ? sqlt : __bind_storage_type<T>::sqlt_dt
                                , &indp
                                , NULL, NULL, 0, NULL, OCI_DEFAULT));
}


template<typename TInd, typename T>
void database::oracle::query::__set_int(const TInd& ind, T value)
{
  OCIBind* bindhp;
  __ocinumber_container.push_back(OCINumber());
  OCINumber& num = __ocinumber_container.back();
  int_to_ocinumber(value, &num, __session);
  __set(ind, &num, sizeof(OCINumber));
}
template<typename TInd, typename T>
void database::oracle::query::__set_float(const TInd& ind, T value)
{
  OCIBind* bindhp;
  __ocinumber_container.push_back(OCINumber());
  OCINumber& num = __ocinumber_container.back();
  real_to_ocinumber(value, &num, __session);
  __set(ind, &num, sizeof(OCINumber));
}

void database::oracle::query::set(const std::string& param_position, long long data)
{
  __set_int(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, int data)
{
  __set_int(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, short data)
{
  __set_int(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, char data)
{
  __set_int(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, unsigned long long data)
{
  __set_int(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, unsigned int data)
{
  __set_int(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, unsigned short data)
{
  __set_int(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, unsigned char data)
{
  __set_int(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, float data)
{
  __set_float(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, double data)
{
  __set_float(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, long double data)
{
  __set_float(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, const date_time& data)
{
  boost::shared_ptr<__handle<OCIDateTime> > dt_hndl_ptr (
    new __handle<OCIDateTime>(__session.envhp()));
  OCIDateTime*& hndl = dt_hndl_ptr.get()->hndl;
  __ocidatetime_container.push_back(dt_hndl_ptr);
  datetime_to_ocidatetime(hndl, data, __session);
  __set(param_position, &hndl, sizeof(OCIDateTime*));
}

void database::oracle::query::set(const std::string& param_position, const std::string& data)
{
  __str_container.push_back(data);
  __set(param_position,
        const_cast<char*>(__str_container.back().c_str())
        , __str_container.back().length() + 1);
}

void database::oracle::query::set_clob(const std::string& param_position
                                , /* const */ char* strdata, size_t length)
{
  __set(param_position, strdata, length, SQLT_LNG);
}

void database::oracle::query::set_blob(const std::string& param_position
                                , /* const */ unsigned char* blob_data, size_t length)
{
  __set(param_position, blob_data, length, SQLT_LBI);
}

void database::oracle::query::set_null(const std::string& param_position)
{
  __set(param_position, reinterpret_cast<int*>(NULL), 0);
}

void database::oracle::query::__fill_numerics(__numeric_out_struct_t& data)
{
  switch (data.second.which())
  {
  case 0:
    cast_oci_number(*data.first, *boost::get<char*>(data.second), false, false, __session.errhp());
  case 1:
    cast_oci_number(*data.first, *boost::get<short*>(data.second), false, false, __session.errhp());
  case 2:
    cast_oci_number(*data.first, *boost::get<int*>(data.second), false, false, __session.errhp());
  case 3:
    cast_oci_number(*data.first, *boost::get<long*>(data.second), false, false, __session.errhp());
  case 4:
    cast_oci_number(*data.first, *boost::get<unsigned char*>(data.second), true, false, __session.errhp());
  case 5:
    cast_oci_number(*data.first, *boost::get<unsigned short*>(data.second), true, false, __session.errhp());
  case 6:
    cast_oci_number(*data.first, *boost::get<unsigned int*>(data.second), true, false, __session.errhp());
  case 8:
    cast_oci_number(*data.first, *boost::get<unsigned long*>(data.second), true, false, __session.errhp());
  case 9:
    cast_oci_number(*data.first, *boost::get<float*>(data.second), false, true, __session.errhp());
  case 10:
    cast_oci_number(*data.first, *boost::get<double*>(data.second), false, true, __session.errhp());
  case 11:
    cast_oci_number(*data.first, *boost::get<long double*>(data.second), true, false, __session.errhp());
  }
}

void database::oracle::query::__fill_output_variables()
{
  BOOST_FOREACH(__out_container_t::value_type& out_pair, __out_container)
  {
    if (out_pair.second.which() == 0)
    {
      __fill_numerics(boost::get<__numeric_out_struct_t>(out_pair.second));
      continue;
    }
    if (out_pair.second.which() == 1)
    {
      __datetime_out_struct_t& dts = boost::get<__datetime_out_struct_t>(out_pair.second);
      ocidatetime_to_ptime(dts.first->hndl, *dts.second, &__session);
    }
  }
}

template<typename T>
void database::oracle::query::__set_out_numeric(const string& param_position, T* value)
{
  string myparam;
  if (':' != param_position[0])
    myparam = ":";
  myparam += param_position;
  __ocinumber_container.push_back(OCINumber());
  __numeric_out_struct_t ios = __numeric_out_struct_t(&__ocinumber_container.back(), value);
  __out_container[myparam] = ios;
  __set(param_position, &__ocinumber_container.back(), sizeof(OCINumber));
}

void database::oracle::query::set(const std::string& param_position, long long* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, int* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, short* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, char* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, unsigned long long* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, unsigned int* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, unsigned short* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, unsigned char* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, float* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, double* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, long double* data)
{
  __set_out_numeric(param_position, data);
}

void database::oracle::query::set(const std::string& param_position, date_time* data)
{
  string myparam;
  if (':' != param_position[0])
    myparam = ":";
  myparam += param_position;
  boost::shared_ptr<__handle<OCIDateTime> > dt_ptr;
  __ocidatetime_container.push_back(dt_ptr);
  __datetime_out_struct_t dos = __datetime_out_struct_t(dt_ptr.get(), data);
  __out_container[myparam] = dos;
  __set(param_position, &dt_ptr->hndl, sizeof(OCIDateTime*));
}


void database::oracle::query::__clear_containers()
{
  __define_container.clear();
  __column_names.clear();

  __ocinumber_container.clear();
  __ocidatetime_container.clear();
  __str_container.clear();
  __indices.clear();
  __out_container.clear();
  __out_str_container.clear();
}
