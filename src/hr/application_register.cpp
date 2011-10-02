
#include "application_register.hpp"
#include <connect.hpp>
#include <ksi_include/ksi_exceptions.hpp>

//#include <wx_util.hpp>
//#include <wx/msgdlg.h>

void try_register(int app_id)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select ksi.application_register_funct(:app_id) from dual"
	);
	query->set("app_id", app_id);
	query->execute_statement();
	if (query->next()) throw exception_t(0, "","void try_register(int app_id)");

	int res = static_cast<int>(boost::get<double>(query->get(0)));

	if (res == 2) {
		std::string str = "language is not set";
		std::string eng_str = "Invalid server type";
		std::string rus_str = "Не верный тип сервера";
		throw exception_t(0, eng_str, "void try_register(int app_id)");
	}
	else if (res == 3) {
		std::string str = "language is not set";
		std::string eng_str = "User does not have sufficient privilegies to run this application";
		std::string rus_str = "Пользователь не имеет привелегий для запуска этого приложения";
		throw exception_t(1, eng_str, "void try_register(int app_id)");
	}
	else if (res == 4) {
		std::string str = "language is not set";
		std::string eng_str = "Invalid application identifier or bad server config (settings)";
		std::string rus_str = "Ошибка в настройке сервера";
		throw exception_t(2, eng_str, "void try_register(int app_id)");
	}
}
