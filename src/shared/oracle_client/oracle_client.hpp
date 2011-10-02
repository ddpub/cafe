/*
 * =====================================================================================
 * 
 *       Filename:  oracle_client_lib.hpp
 * 
 *    Description:  Oracle client library source
 * 
 *        Version:  1.0
 *        Created:  18.10.2007 17:05:06 MSD
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Pavel G. Koukoushkin (), koukoushkinpg@ksi.ru
 *        Company:  Phlint |COMPANY| K, Moscow, Russia
 * 
 * =====================================================================================
 */

#ifndef  ORACLE_CLIENT_LIB_INC
#define  ORACLE_CLIENT_LIB_INC


//#include "oracle.hh"

#include <utility>
#include <boost/variant.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include "ksi_exception.hpp"
/**
 * Суррогатная структура. Используется в типе VARIANT для передачи NULLов
 */
struct null_t {
};

#define VARIANT boost::variant< ::null_t, double, boost::shared_ptr< double >, int, boost::shared_ptr< int >, std::string, boost::shared_ptr< std::string >, boost::posix_time::ptime, boost::shared_ptr< boost::posix_time::ptime >, std::vector< char >, std::vector< unsigned char> >

#define ORALCE_SESSION 0
#define POSTGRES_SESSION 1 


enum __db_msgcodes
{
  DB_ERROR_LOGICAL = -1,
  DB_ERROR_INTERNAL = 0,
  DB_ERROR_INVALID_CREDENTIALS = 1001,
  DB_ERROR_EMPTY_PASSWORD,
  DB_ERROR_CHILD_RECORDS_EXISTS,
  DB_ERROR_INVALID_HOSTNAME,
  DB_ERROR_CONNECTION_PROBLEM,
  DB_ERROR_ACCOUNT_LOCKED,
  DB_ERROR_RESOURCE_BUSY
};
  


namespace ksi_client
{

//  static CORBA::Object_ptr getObjectReference(void* orb);
//
  
  class db_exception : public ksi_exception
  {
  public:
    db_exception(int msg_id, const std::string& msg, long code = 0) : ksi_exception(msg_id, msg, code) {};
	std::pair<std::string, std::string> userFriendly() {
		if (__code == 1017) return std::pair<std::string, std::string>("invalid_username_password", "Wrong login/password");
		return std::pair<std::string, std::string>("fake", what());
	}
  };

  class connection_problem_exception : public db_exception
  {
  public:
    connection_problem_exception(int msg_id, const std::string& msg, long code = 0) : db_exception(msg_id, msg, code) {};
  };

  /**
   * Интерфейс запроса. Содержит функции для работы с запросом. Предоставляет
   * доступ к данным, короые сфетчились в результате запроса.
   */
  class oracle_query_i
  {
  public:
    /**
     * Функция доступа к полученным в результате выполнения запроса данным.
     * Возвращает значение, хранящееся в указанной колонке текущей строки (см.
     * next()).
     * \param col_index 0-based индекс колонки, из которой следует получить значение.
     * \return Значение колонки типа boost::variant< ::null_t, int, std::string, double, boost::posix_time::ptime>
     */
    virtual VARIANT get(const long col_index) = 0;

    /**
     * Функция установкм значеня параметра запроса.
     * \param col_name Имя параметра в виде ":имя_параметра"
     * \param value Требуемое значение параметра, типа boost::variant< ::null_t, int, std::string, double, boost::posix_time::ptime>
     */
    virtual void set(const std::string& col_name, const VARIANT& value) = 0;

    /**
     * Перевод текщей строки в результатах запроса на следующую.
     * \return 0 если стока переведена на существующую строку. 1 если строка переведена на строку после последней (последующие get() или next() вызовут исключение!).
     */
    virtual long next() = 0;

    /**
     * Устанавливает текст запроса для объекта.
     * \param statement Текст запроса.
     */
    virtual void create_statement(const std::string& statement) = 0;

    /**
     * Заполянет параметры значениями. Исполняет запрос. Получает данные с сервера.
     */
    virtual void execute_statement() = 0;

    virtual bool out_is_null(const std::string& col_name) = 0;
    virtual bool out_is_null(const int col_index) = 0;

    virtual ~oracle_query_i() {};
  };

  /**
   * Интерфейс объекта сессии. Основной интерфейс для работы с БД.
   * Рекомендуется создавать объет в единственном экзепляре.
   */
  class oracle_session_i
  {
  public:
     //interface functions
     /**
      * Устанавливает подключение к серверу.
      * \param login Имя пользователя oracle
      * \param password Пароль пользователя oracle
      * \param tns_name TNS name экземпляря БД
      */
    virtual void connect(const std::string& login, const std::string& password, const std::string& tns_name, const std::string& sid, unsigned short port) = 0;
    virtual void disconnect() = 0;
    virtual void direct_execute(const std::string& statement) = 0;
    virtual void commit() = 0;
    virtual void rollback() = 0;
    virtual std::auto_ptr<oracle_query_i> create_query() = 0;
    virtual const std::string& username() = 0;
    virtual ~oracle_session_i() {};
  };

  extern std::auto_ptr<oracle_session_i> create_session(int sesstype = 0);
};


#endif   /* ----- #ifndef ORACLE_CLIENT_LIB_INC  ----- */
