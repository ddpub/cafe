#ifndef DMITRY_DERGACHEV_27_03_2008_SYNC_H
#define DMITRY_DERGACHEV_27_03_2008_SYNC_H


#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace ksi_kitchen {

class dish_sync_t
{
public:
  dish_sync_t() { refresh(); }
public:
  const std::vector<int>& order_id_vec() const { return order_id_vec_; }
private:
  void refresh();
private:
  std::vector<int> order_id_vec_;
private:
  dish_sync_t(dish_sync_t&);
};

class mod_sync_t
{
public:
  mod_sync_t() { refresh(); }
public:
  const std::vector<int>& order_id_vec() const { return order_id_vec_; }
private:
  void refresh();
private:
  std::vector<int> order_id_vec_;
private:
  mod_sync_t(mod_sync_t&);
};

}

#endif

