#include <boost/python.hpp>
#include "postgres.hh"
using namespace ksi_client;
using namespace boost::python;

std::string get_str(VARIANT& var)
{
  return boost::get< std::string >(var);
}

BOOST_PYTHON_MODULE(liboracle)
{
class_< VARIANT >("ksi_variant")
;
def("get_str", &get_str);

class_<postgres_query_impl>("query", init<postgres_session_impl>())
  .def("create_statement", &postgres_query_impl::create_statement)
  .def("execute_statement", &postgres_query_impl::execute_statement)
  .def("next", &postgres_query_impl::next)
  .def("get", &postgres_query_impl::get)
;
class_<postgres_session_impl>("session")
  .def("connect", &postgres_session_impl::connect)
  .def("disconnect", &postgres_session_impl::disconnect)
  .def("direct_execute", &postgres_session_impl::direct_execute)
  .def("commit", &postgres_session_impl::commit)
  .def("rollback", &postgres_session_impl::rollback)
//  .def("create_query", &postgres_session_impl::create_query)
;
};

