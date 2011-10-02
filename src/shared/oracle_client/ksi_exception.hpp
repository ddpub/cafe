/*
 * =====================================================================================
 * 
 *       Filename:  ksi_exception.hpp
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  25.12.2007 14:54:08 MSK
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Pavel G. Koukoushkin (), koukoushkinpg@ksi.ru
 *        Company:  Phlint and K, Moscow, Russia
 * 
 * =====================================================================================
 */

#ifndef  KSI_EXCEPTION_INC
#define  KSI_EXCEPTION_INC
#include <exception>
#include <string>


namespace ksi_client
{
  class ksi_exception : public std::exception
  {
  protected:
    std::string text;
    long __code;
    long __msg_id;
  public:
    ksi_exception(int msg_id, const std::string& msg, long code = 0) : __msg_id(msg_id), text(msg.c_str()), __code(code) {};

    virtual const char* what() const throw()
    {
      return text.c_str();
    }
    
    virtual int msg_id() const
    {
      return __msg_id;
    }

    virtual const long code() const throw() {return __code; }
    virtual ~ksi_exception() throw() {};
  };

/*  class oracle_exception : public ksi_exception
  {
    int text;
    std::string status;
  public:
    oracle_exception(const std::string& msg) : text(msg.c_str()) {};
    oracle_exception(const std::string& msg, const int status) : text(msg.c_str()), status(status) {};
    virtual ~oracle_exception() throw() {};
  };*/
};

#endif   /* ----- #ifndef KSI_EXCEPTION_INC  ----- */

