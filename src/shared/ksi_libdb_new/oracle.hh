/*
 * =====================================================================================
 *
 *       Filename:  oracle.hh
 *
 *    Description:  Oracle connection implementation
 *
 *        Version:  1.0
 *        Created:  28.09.2008 19:45:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel G. Koukoushkin (mn), k.pavel.g@gmail.com
 *        Company:  Phlint and K, Moscow, Russia
 *
 * =====================================================================================
 */


#include "oci_adapter.hh"
#include <string>

namespace database
{
  namespace oracle
  {
    class session
    {
      friend class statement;
      database::oracle::__oci_adapter::oci_adapter __adapter;
      std::string __username;
    public:
      void connect(const std::string& login,
          const std::string& password,
          const std::string& host,
          const std::string& database_name,
          int port = 1521);
      void commit();
      void rollback();
      const std::string& username() { return __username; };
      database::oracle::__oci_adapter::oci_adapter& adapter() { return __adapter; };
      unsigned int execute(const std::string& statement);
    }; 

    class statement
    {
      database::oracle::__oci_adapter::statement_adapter __adapter;
      std::string __statement;
    public:
      statement(database::oracle::__oci_adapter::oci_adapter& session_adapter);
      void prepare(const std::string& statement_string);
      template<typename Type>
        void set(Type const& data, unsigned int position)
        {
          __adapter.set(data, position);
        }
      template<typename Type>
        void set(Type const& data, const std::string& alias)
        {
          __adapter.set(data, alias);
        }
      void set_null(unsigned int position);
      void set_null(const std::string& alias);
      unsigned int execute();
      bool next();
      const database::oracle::__oci_adapter::data_type& get(unsigned int position) { return __adapter.get(position); };
      const boost::shared_array<char> get_lob_data(unsigned int position,
          size_t& size);
    };
  }; 
};
