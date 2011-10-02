
#include "sync.hpp"
#include <memory>
#include <ksi_include/ksi_exceptions.hpp>
#include <connect.hpp>

namespace ksi_kitchen {


void dish_sync_t::refresh()
{
  order_id_vec_.clear();

  std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();

  query->create_statement
  (
  "select time, order_id \
  from ksi.fast_sync_order_list_v"
  );

  query->execute_statement();

  while (!query->next())
  {
    int order_id = static_cast<int>(boost::get<double>(query->get(1)));
    order_id_vec_.push_back(order_id);
  }
}

void mod_sync_t::refresh()
{
  order_id_vec_.clear();
  std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();

  query->create_statement
  (
  "select distinct time, order_id \
  from ksi.fast_sync_order_mod_list_v"
  );

  query->execute_statement();

  while (!query->next())
  {
    int order_id = static_cast<int>(boost::get<double>(query->get(1)));
    order_id_vec_.push_back(order_id);
  }
}

};

