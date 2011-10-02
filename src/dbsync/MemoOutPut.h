//---------------------------------------------------------------------------

#ifndef MemoOutPutH
#define MemoOutPutH

#include <ksi_util/bag_spooler.hpp>
#include <string>

//---------------------------------------------------------------------------

class memo_out_put_t
{
private  :
  std::string last_upload_log;
  std::string last_download_log;
  std::string last_db_log;
  std::string last_other_log;
  std::string open_upload_log;
  std::string open_download_log;
  std::string memo;
  int direct;
  void put_download(std::string str);
  void put_upload(std::string str);
  void put_db(std::string str);
  void put_other(std::string str);
  void put_ifill(std::string str);
  void add_other(std::string str);
  void warn_download(std::string str);
  void warn_upload(std::string str);
  void add(std::string str);
  bool download() const { if (direct == 0) return true; return false; } //download
  bool upload() const { if (direct == 1) return true; return false; }  // upload
  bool add() const { if (direct == 2) return true; return false; }  
  bool other() const { if (direct == 3) return true; return false; }  // other
  bool warn_up() const { if (direct == 5) return true; return false; }
  bool warn_down() const { if (direct == 4) return true; return false; }
  bool db() const { if (direct == 6) return true; return false; }  // dbaction
  bool ifill() const { if (direct == 7) return true; return false; }  // ifill
  std::auto_ptr<bag_spooler_t> spooler;
public:
  void clear() { init(); memo = ""; }
  memo_out_put_t(std::string memo) : memo(memo) { init(); }
  memo_out_put_t() : memo("") { init(); }
  std::string put(std::string str, int d, bool warn = false);
  void init();
};

#endif
