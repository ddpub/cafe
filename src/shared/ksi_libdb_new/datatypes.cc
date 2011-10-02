#include "datatypes.hh"
#include <iconv.h>
#include <cstring>
#include <boost/shared_array.hpp>
#include "oci_op.hh"


using namespace database::oracle::__datatypes;
using namespace database::oracle::__oci_op;
//using namespace database::oracle::__oci_handles;
using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace std;
using namespace boost;

boost::posix_time::ptime database::oracle::__datatypes::__ocidate2ptime(OCIDate const& odate)
{
  unsigned int tmp_date = (odate.OCIDateYYYY < 1400 || odate.OCIDateYYYY > 10000) ? 1400 : odate.OCIDateYYYY;
  return ptime (date(tmp_date, odate.OCIDateMM, odate.OCIDateDD),
      time_duration(odate.OCIDateTime.OCITimeHH, odate.OCIDateTime.OCITimeMI,
      odate.OCIDateTime.OCITimeSS));
}

OCIDate database::oracle::__datatypes::__ptime2ocidate(boost::posix_time::ptime const& odate)
{
  OCIDate oci_date;
  date pdate = odate.date();
  time_duration td = odate.time_of_day();
  oci_date.OCIDateYYYY = pdate.year();
  oci_date.OCIDateMM = pdate.month();
  oci_date.OCIDateDD = pdate.day();
  oci_date.OCIDateTime.OCITimeHH = td.hours();
  oci_date.OCIDateTime.OCITimeMI = td.minutes();
  oci_date.OCIDateTime.OCITimeSS = td.seconds();
  return oci_date;
}

std::wstring database::oracle::__datatypes::__cp1251towstring(const string& src)
{
  size_t src_len = src.length() + 1;
  if (!src_len)
    return wstring();
  iconv_t cb = iconv_open( "WCHAR_T", "CP1251");
  if (cb == reinterpret_cast<iconv_t>(-1))
    return wstring();
  size_t dst_len = src_len + 1;
  shared_array<wchar_t> out(new wchar_t[dst_len]);
  char *src_iter = const_cast<char*>(src.c_str());
  char *dst_iter = reinterpret_cast<char*>(out.get());
  size_t src_left = src_len; 
  size_t dst_left = dst_len * sizeof(wchar_t);

  iconv(cb, &src_iter, &src_left, &dst_iter, &dst_left);
  iconv(cb, NULL, NULL, &dst_iter, &dst_left);

  wstring ret(out.get());
  iconv_close(cb);

  return ret;
}


char_container_ptr database::oracle::__datatypes::__create_char(std::string const& from)
{
  return from;
}
