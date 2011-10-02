/*
 * =====================================================================================
 *
 *       Filename:  oci_lob.hh
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09.12.2008 17:24:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel G. Koukoushkin (mn), k.pavel.g@gmail.com
 *        Company:  Phlint and K, Moscow, Russia
 *
 * =====================================================================================
 */



#include <boost/shared_ptr.hpp>
#include <vector>

namespace database { namespace oracle { namespace __datatypes {
  typedef std::vector<unsigned char> blob_data_container;
  typedef boost::shared_ptr<blob_data_container> blob_data_ptr;
  typedef std::vector<char> clob_data_container;
  typedef boost::shared_ptr<clob_data_container> clob_data_ptr;

  struct __oci_lob_i
  {
    virtual void** handle_ptr() = 0;
    virtual void* handle() = 0;
    virtual size_t size() = 0;
    virtual unsigned int get_length() = 0;
    virtual blob_data_ptr extract_blob_data() = 0;
    virtual clob_data_ptr extract_clob_data() = 0;
    virtual void initialize(void* oci_env_handle, void* oci_svcctx_handle, void* oci_error_handle) = 0;

    virtual ~__oci_lob_i() {};
  };

  struct oci_blob
  {
    boost::shared_ptr<__oci_lob_i> lob;
    oci_blob();
    __oci_lob_i * operator ->() const
    {
      return lob.get();
    }
  };
  struct oci_clob
  {
    boost::shared_ptr<__oci_lob_i> lob;
    oci_clob();
    __oci_lob_i * operator ->() const
    {
      return lob.get();
    }
  };

}; }; };
