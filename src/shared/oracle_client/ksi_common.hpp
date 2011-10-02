/*
 * =====================================================================================
 * 
 *       Filename:  ksi_common.hpp
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  25.12.2007 14:54:23 MSK
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Pavel G. Koukoushkin (), koukoushkinpg@ksi.ru
 *        Company:  Phlint and K, Moscow, Russia
 * 
 * =====================================================================================
 */

#ifndef  KSI_COMMON_INC
#define  KSI_COMMON_INC
namespace ksi_client
{
  long initialize(int argc, char** argv);
  extern bool inited;
};

#endif   /* ----- #ifndef KSI_COMMON_INC  ----- */

