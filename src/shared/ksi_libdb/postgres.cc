#include "postgres.hh"
#include <ksi_common.hpp>
#include <ksi_exception.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#ifndef __WIN32__
#include <arpa/inet.h>
#endif


#include <vector>

#define __PQ_OP(OP, MSG, WAIT) __res = OP; \
  if (PQresultStatus(__res) != WAIT) \
  {\
__err_code = 0;\
__err_str = boost::lexical_cast<std::string>("ERROR in ");\
__err_str += boost::lexical_cast<std::string>(#OP);\
__err_str += boost::lexical_cast<std::string>(". At oracle.cc line ");\
__err_str += boost::lexical_cast<std::string>(__LINE__);\
__err_str += std::string(". With message: ");\
__err_str += string(PQerrorMessage(__conn));\
PQexec(__conn, "ROLLBACK TO last_savepoint");\
PQclear(__res);    \
throw ksi_client::db_exception(DB_ERROR_INTERNAL, __err_str, __err_code); \
    }

using namespace ksi_client;
using namespace std;
using namespace boost;


void ksi_client::postgres_session_impl::disconnect()
{
}

void ksi_client::postgres_session_impl::connect(const std::string& login, const std::string& password, const std::string& tns_name)
{
  string connect_str = string("user=\'") + login + string("\' ");
  connect_str +=  string("password=\'") + password + string("\' ");
  connect_str +=  string("host=") + tns_name + string(" ");
  connect_str +=  string("dbname=cafe");
  __conn = PQconnectdb(connect_str.c_str());
  if (PQstatus(__conn) != CONNECTION_OK)
    throw db_exception(DB_ERROR_INTERNAL, PQerrorMessage(__conn), 0);
  __PQ_OP(PQexec(__conn, "BEGIN"), "at connect", PGRES_COMMAND_OK);
  __PQ_OP(PQexec(__conn, "SAVEPOINT last_savepoint"), "at connect", PGRES_COMMAND_OK);
  __login = login;
}

void ksi_client::postgres_session_impl::commit()
{
  __PQ_OP(PQexec(__conn, "COMMIT"), "at commit", PGRES_COMMAND_OK);
  __PQ_OP(PQexec(__conn, "BEGIN"), "at commit", PGRES_COMMAND_OK);
}

void ksi_client::postgres_session_impl::rollback()
{
  cout << "ROLLBACK" <<endl;
  __PQ_OP(PQexec(__conn, "ROLLBACK"), "at rollback", PGRES_COMMAND_OK);
  __PQ_OP(PQexec(__conn, "BEGIN"), "at commit", PGRES_COMMAND_OK);
}

void ksi_client::postgres_session_impl::direct_execute(const std::string& statement)
{
  boost::regex user_regex;
  std::ostringstream oss;
  ostream_iterator<char, char> osi(oss);
  string tmp_statement = "";
  string regex = "[Ii][Dd][Ee][Nn][Tt][Ii][Ff][Ii][Ee][Dd] +[Bb][Yy] +\"([^\"]*)\"";
  user_regex.assign(regex);
  boost::regex_replace(osi, statement.begin(), statement.end(), user_regex, "UNENCRYPTED PASSWORD '$1'", boost::match_default | boost::format_all);
  tmp_statement = oss.str();
  std::ostringstream oss1;
  osi = ostream_iterator<char, char>(oss1);
  user_regex.assign("[uU][tT][iI][lL]\\.");
  boost::regex_replace(osi, tmp_statement.begin(), tmp_statement.end(), user_regex, "util_", boost::match_default | boost::format_all);
  tmp_statement = oss1.str();
  std::ostringstream oss2;
  osi = ostream_iterator<char, char>(oss2);
  user_regex.assign("[eE][mM][pP][tT][yY]_[bB][lL][oO][bB] *\\( *\\)");
  boost::regex_replace(osi, tmp_statement.begin(), tmp_statement.end(), user_regex, "NULL", boost::match_default | boost::format_all);
  tmp_statement = oss2.str();
  cout << "DIRECT execute:" << tmp_statement << endl;
  __PQ_OP(PQexec(__conn, "SAVEPOINT last_savepoint"), "at connect", PGRES_COMMAND_OK);
  __PQ_OP(PQexec(__conn, tmp_statement.c_str()), "at direct execute", PGRES_COMMAND_OK);
  return;
}

std::auto_ptr<oracle_query_i> ksi_client::postgres_session_impl::create_query()
{
  return auto_ptr< oracle_query_i >(new postgres_query_impl(__conn));
}

VARIANT ksi_client::postgres_query_impl::get(const long col_index)
{
  if (__fetch_iter == __data.end())
    throw ksi_client::db_exception(DB_ERROR_INTERNAL, "End of dataset reached", 0);
  if (col_index >= (*__fetch_iter).size())
    throw db_exception(DB_ERROR_INTERNAL, "Column index is out if range", 0);
  return (*__fetch_iter)[col_index];
}

void ksi_client::postgres_query_impl::set(const std::string& col_name, const VARIANT& value)
{
  if (__ora_post_map.find(col_name) != __ora_post_map.end())
  {
    __params[__ora_post_map[col_name]] = value;
    return;
  }
  __out_params[col_name] = value;
}

long ksi_client::postgres_query_impl::next()
{
  if (__before_first)
  {
    if (__fetch_iter == __data.end())
      return 1;
    if (__fetch_iter != __data.begin())
      throw ksi_client::db_exception(DB_ERROR_INTERNAL, "Execute statement before using next()", 0);
    __before_first = false;
    return 0;
  }
  if (__fetch_iter == __data.end())
    throw ksi_client::db_exception(DB_ERROR_INTERNAL, "End of dataset reached", 0);
  ++__fetch_iter;
  ++__nulls_iter;
  if (__fetch_iter == __data.end())
    return 1;
  return 0;
}

void ksi_client::postgres_query_impl::create_statement(const std::string& statement)
{
  __statement = "";
  boost::regex param_regex;
  param_regex.assign("[Ii][Nn][Tt][Oo] +:([^( |,)]*)");
  boost::sregex_iterator iter (statement.begin(), statement.end(), param_regex);
  boost::sregex_iterator iter_surr; 
  string::const_iterator prev_iter = statement.begin();
  string tmp_statement;
  for (int i = 1;iter != iter_surr; ++iter, ++i)
  {
    string tmp_str(((*iter)[1]).str().c_str());
    tmp_statement.append(prev_iter, (*iter)[0].first);
    tmp_statement += string(" AS ") + tmp_str;
    prev_iter = (*iter)[0].second;
  }
  if (statement.begin() == prev_iter)
    tmp_statement = statement;
  param_regex.assign("[fF][rR][oO][mM]\\s+[dD][uU][aA][lL]");
  iter = boost::sregex_iterator(tmp_statement.begin(), tmp_statement.end(), param_regex);
  if (iter != iter_surr)
  {
    string s = "";
    s.append(prev_iter, (*iter)[0].first);
    tmp_statement = s;
  }
  
  param_regex.assign("[nN][oO][cC][aA][cC][hH][eE] +[oO][rR][dD][eE][rR]");
  iter = boost::sregex_iterator(tmp_statement.begin(), tmp_statement.end(), param_regex);
  prev_iter = tmp_statement.begin();
  if (iter != iter_surr)
  {
    string s = "";
    s.append(prev_iter, (*iter)[0].first);
    tmp_statement = s;
  }


  param_regex.assign(":([^( |,)]*)");
  iter = boost::sregex_iterator(tmp_statement.begin(), tmp_statement.end(), param_regex);
  prev_iter = tmp_statement.begin();                         
  for (int i = 1;iter != iter_surr; ++iter, ++i)
  {
    string tmp_str(((*iter)[1]).str().c_str());
    __statement.append(prev_iter, (*iter)[0].first);
    __ora_post_map[tmp_str] = i;
    __statement += string("$") + boost::lexical_cast< string >(i);
    prev_iter = (*iter)[0].second;
  }
  string::const_iterator end_iter = tmp_statement.end();
  __statement.append(prev_iter, end_iter);
  std::ostringstream oss;
  ostream_iterator<char, char> osi(oss);
  param_regex.assign("[Ss][Yy][Ss][Dd][Aa][Tt][Ee]");
  boost::regex_replace(osi, __statement.begin(), __statement.end(), param_regex, "current_timestamp", boost::match_default | boost::format_all);
  __statement = oss.str();
  std::ostringstream oss1;
  osi = ostream_iterator<char, char>(oss1);
  param_regex.assign("[uU][tT][iI][lL]\\.");
  boost::regex_replace(osi, __statement.begin(), __statement.end(), param_regex, "util_", boost::match_default | boost::format_all);
  __statement = oss1.str();
  __params.clear();
}

void ksi_client::postgres_query_impl::execute_statement()
{
  cout << __statement << endl;
  map< int, VARIANT >::const_iterator iter = __params.begin();
  map< int, VARIANT >::const_iterator out_iter = __params.end();
  size_t len = __params.size();
  vector< const char* > ptr_values;
  vector< boost::shared_ptr< const char > > values_container;
  vector< int > lengths;
  vector< int > formats;
  vector< string > str_values;
  int64_t val64;
  const posix_time::ptime epoch(gregorian::date(2000, 1, 1));
  int i = 0;
  for (; iter != __params.end(); ++i, ++iter)
  {
    try
    {
      boost::get<null_t>(iter->second);
      cout << iter->first << ": NULL" << endl;
      lengths.push_back(0);
      formats.push_back(1);
      ptr_values.push_back(NULL);
      values_container.push_back(shared_ptr< const char >());
      continue;
    }
    catch (bad_get&)
    { }
    try
    {
      const posix_time::ptime& t_val = boost::get<posix_time::ptime>(iter->second);
      cout << iter->first << ": " << boost::get<posix_time::ptime>(iter->second) << " ptime" << endl;
      int64_t val64_tmp;
      char* c_data = new char[8];
      shared_ptr< const char > sp_data (c_data);
      val64 = (t_val - epoch).total_microseconds();
      for (int i = 7; i >= 4; --i)
      {
        c_data[i] = reinterpret_cast<char*>(&val64)[7 - i]; 
      }
      for (int i = 3; i >= 0; --i)
      {
        c_data[i] = reinterpret_cast<char*>(&val64)[7 - i]; 
      }
      lengths.push_back(8);
      formats.push_back(1);
      values_container.push_back(sp_data);
      ptr_values.push_back(values_container.back().get());
      continue;
    }
    catch (bad_get& ex)
    {  }
    try
    {
      const string& int_val = boost::get<string>(iter->second);
      cout << iter->first << ": " << boost::get<string>(iter->second) << " str" << endl;
      str_values.push_back(int_val);
      ptr_values.push_back(str_values.back().c_str());
      values_container.push_back(shared_ptr< const char >());
      formats.push_back(0);
      lengths.push_back(0);
      continue;
    }
    catch (bad_get& ex)
    {  }
    try
    {
      const vector< char >& value = boost::get< vector< char > >(iter->second);
      lengths.push_back(value.size());
      formats.push_back(1);
      ptr_values.push_back(&(value[0]));
      values_container.push_back(shared_ptr< const char >());
      continue;
    }
    catch (bad_get& ex)
    {  }
    try
    {
      const int& int_val = boost::get<int>(iter->second);
      cout << iter->first << ": " << boost::get<int>(iter->second) << " int"<< endl;
      str_values.push_back(lexical_cast< string >(int_val));
      ptr_values.push_back(str_values[i].c_str());
      values_container.push_back(shared_ptr< const char >());
      formats.push_back(0);
      lengths.push_back(0);
      continue;
    }
    catch (bad_get& ex)
    {  }
    try
    {
      const double& d_val = boost::get<double>(iter->second);
      cout << iter->first << ": " << boost::get<double>(iter->second) << " double" << endl;
      str_values.push_back(lexical_cast< string >(d_val));
      ptr_values.push_back(str_values.back().c_str());
      values_container.push_back(shared_ptr< const char >());
      formats.push_back(0);
      lengths.push_back(0);
//      values.push_back(reinterpret_cast<const char*>(&d_val));
//      lengths.push_back(sizeof(double));
//      formats.push_back(1);
      continue;
    }
    catch (bad_get& ex)
    { --i; out_iter = iter; }
  }
  __PQ_OP(PQexec(__conn, "SAVEPOINT last_savepoint"), "at connect", PGRES_COMMAND_OK);
  PGresult *res = PQexecParams(__conn, __statement.c_str(), i, NULL, &(ptr_values[0]), &(lengths[0]), &(formats[0]), 1);
  __data.clear();
  if (PQresultStatus(res)== PGRES_FATAL_ERROR)
  {
    cout << PQresultErrorField(res, PG_DIAG_SEVERITY) << PQresultErrorField(res, PG_DIAG_MESSAGE_PRIMARY) << PQresultErrorField(res, PG_DIAG_MESSAGE_HINT) << endl;
    cout << PQresultErrorField(res, PG_DIAG_MESSAGE_DETAIL) << endl;
  __PQ_OP(PQexec(__conn, "ROLLBACK TO SAVEPOINT last_savepoint"), "at connect", PGRES_COMMAND_OK);
    PQclear(res);
    throw db_exception(DB_ERROR_INTERNAL, PQerrorMessage(__conn), 0);
  }

  if (PQresultStatus(res) != PGRES_TUPLES_OK)
  {
    PQclear(res);
    __fetch_iter = __data.begin();
    __nulls_iter = __nulls.begin();
    __before_first = true;
    return;
  }
  int col_count = PQnfields(res);
  int row_count = PQntuples(res);
  bool out_exists = __out_params.size();
  for (int row = 0; row < row_count; ++row)
  {
    __data.push_back(vector< VARIANT >());
    __nulls.push_back(std::set< int >());
    for (int col = 0; col < col_count; ++col)
    {
      Oid oid = PQftype(res, col);
      const char *data = NULL;
      int data_len = PQgetlength(res, row, col);
      uint32_t val = 0;
      double d_val;
      auto_ptr< char > c_data;
      string str_val;
      posix_time::ptime ptime_val;
      vector< char > vc_data;
      switch (oid)
      {
      case 20:
        if (PQgetisnull(res, row, col))
        {
          __data[row].push_back(double(0));
          __nulls.back().insert(col);
          break;
        }
        val64 = 0;
        data = PQgetvalue(res, row, col);
        for (int i = 7; i >= 4; --i)
        {
          reinterpret_cast<char*>(&val64)[7 - i] = data[i]; 
        }
        for (int i = 3; i >= 0; --i)
        {
          reinterpret_cast<char*>(&val64)[7 - i] = data[i]; 
        }
        __data[row].push_back(double(val64));
        if (out_exists)
          *(boost::get< shared_ptr<double> >(__out_params[0]).get()) = double(val);
        break;
      case 23:
        if (PQgetisnull(res, row, col))
        {
          __data[row].push_back(double(0));
          __nulls.back().insert(col);
          break;
        }
        data = PQgetvalue(res, row, col);
        memcpy(&val, data, 4);
        val = ntohl(val);
        __data[row].push_back(double(val));
        if (out_exists)
          *(boost::get< shared_ptr<double> >(__out_params[0]).get()) = double(val);
        break;
      case 1114:
        if (PQgetisnull(res, row, col))
        {
          __nulls.back().insert(col);
          __data[row].push_back(posix_time::ptime());
          break;
        }
        data = PQgetvalue(res, row, col);
        val64 = 0;
        for (int i = 7; i >= 4; --i)
        {
          reinterpret_cast<char*>(&val64)[7 - i] = data[i]; 
        }
        for (int i = 3; i >= 0; --i)
        {
          reinterpret_cast<char*>(&val64)[7 - i] = data[i]; 
        }
        ptime_val = epoch + posix_time::seconds(int64_t(val64/1000000));
        __data[row].push_back(ptime_val);
        if (out_exists)
          *(boost::get< shared_ptr< posix_time::ptime > >(__out_params[0])) = ptime_val;
        //__data[row].push_back(double(val64));
        break;
      case 1043:
        if (PQgetisnull(res, row, col))
        {
          __data[row].push_back(string());
          __nulls.back().insert(col);
          break;
        }
        data = PQgetvalue(res, row, col);
        __data[row].push_back(string(data, data_len));
        if (out_exists)
          *(boost::get< shared_ptr< string > >(__out_params[0])) = string(data, data_len);
        break;
      case 701:
        if (PQgetisnull(res, row, col))
        {
          __data[row].push_back(double());
          __nulls.back().insert(col);
          break;
        }
        data = PQgetvalue(res, row, col);
        c_data.reset(new char[data_len]);
        for (int i = 0; i < data_len; i++)
          c_data.get()[i] = data[data_len - i - 1];
        memcpy(&d_val, c_data.get(), 8);
        __data[row].push_back(d_val);
        if (out_exists)
          *(boost::get< shared_ptr< double > >(__out_params[0])) = d_val;
        break;
      case 17:
        if (PQgetisnull(res, row, col))
        {
          __data[row].push_back(vector< char >());
          __nulls.back().insert(col);
          break;
        }
        vc_data.resize(data_len);
        data = PQgetvalue(res, row, col);
        memcpy(const_cast< char* >(&(vc_data[0])), data, data_len);
        __data[row].push_back(vc_data);
        break;
      case 1700:
        if (PQgetisnull(res, row, col))
        {
          __data[row].push_back(double());
          __nulls.back().insert(col);
          break;
        }
        cout << data_len << endl;
        data = PQgetvalue(res, row, col);
        c_data.reset(new char[data_len]);
        str_val = "";
        for (int i = 0; i < data_len; i+=2)
        {
          c_data.get()[i] = data[i + 1];
          c_data.get()[i + 1] = data[i];
        }
        {
          int16_t len;
          int16_t comma;
          int16_t neg;
          int tot_len = int(data_len / 2);
          memcpy(&len, c_data.get(), 2);
          int16_t start_pos = tot_len - len;
          memcpy(&comma, c_data.get() + 2, 2);
          memcpy(&neg, c_data.get() + 4, 2);
          if (len)
            for (int i = 0; i < tot_len; ++i)
            {
              int16_t tmp;
              memcpy(&tmp, c_data.get() + i*2, 2);
//              cout << "|" << tmp << "|";
              if (i < start_pos) continue;
              if (neg)
                str_val += "-";
              if (comma == -1)
                str_val += ".";
              str_val += lexical_cast< string >(tmp); 
              if (i - start_pos == comma)
                str_val += ".";
            }
          else
            str_val = "0";
        }
        __data[row].push_back(lexical_cast< double >(str_val));
        if (out_exists)
          *(boost::get< shared_ptr< double > >(__out_params[0])) = lexical_cast< double >(str_val);
        break;
      default:
        cout << oid << endl;
        throw db_exception(DB_ERROR_INTERNAL, string("unexpected oid"), 0);
      }
    }
  }
  PQclear(res);
  __params.clear();
  __fetch_iter = __data.begin();
  __nulls_iter = __nulls.begin();
  __before_first = true;
}

bool ksi_client::postgres_query_impl::out_is_null(const std::string& col_name)
{

}

bool ksi_client::postgres_query_impl::out_is_null(const int col_index)
{
  return __nulls_iter->find(col_index) != __nulls_iter->end();
}

