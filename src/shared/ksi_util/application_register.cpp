
#include "application_register.hpp"
#include <shared_modules/connect.hpp>
#include <ksi_include/ksi_exceptions.hpp>

//#include <wx_util.hpp>
//#include <wx/msgdlg.h>
extern int lang;

void try_register(int app_id)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"SELECT KSI.APPLICATION_REGISTER_FUNCT(:app_id) FROM DUAL"
	);
	query->set("app_id", app_id);
	query->execute_statement();
	if (query->next()) THROW_LOCATION(0, "void try_register(int app_id)");

	int res = static_cast<int>(boost::get<double>(query->get(0)));

	if (res == 2) {
		std::string str = "language is not set";
		std::string eng_str = "Invalid server type";
		std::string rus_str = "Не верный тип сервера";
		if (lang == 1) str = eng_str;
		else if (lang == 2) str = rus_str;
		THROW_MESSAGE(1, str, "void try_register(int app_id)");
	}
	else if (res == 3) {
		std::string str = "language is not set";
		std::string eng_str = "User does not have sufficient privilegies to run this application";
		std::string rus_str = "Пользователь не имеет привелегий для запуска этого приложения";
		if (lang == 1) str = eng_str;
		else if (lang == 2) str = rus_str;
		THROW_MESSAGE(2, str, "void try_register(int app_id)");
	}
	else if (res == 4) {
		std::string str = "language is not set";
		std::string eng_str = "Invalid application identifier or bad server config (settings)";
		std::string rus_str = "Ошибка в настройке сервера";
		if (lang == 1) str = eng_str;
		else if (lang == 2) str = rus_str;
		THROW_MESSAGE(3, str, "void try_register(int app_id)");
	}
	if (res != 1) THROW_LOCATION(4, "void try_register(int app_id)");
}

void try_cashier_register(const std::string& mac_addr) {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"SELECT KSI.TERMINAL_SEANCE_REGISTER_FUNCT(:mac) FROM DUAL"
	);
	query->set("mac", mac_addr);
	query->execute_statement();
	if (query->next()) THROW_LOCATION(0, "void try_cashier_register(const std::string&)");

	int res = static_cast<int>(boost::get<double>(query->get(0)));
	
	if (res == 2) {
		std::string str = "language is not set";
		std::string eng_str = "This terminal is not registered on the shift or the shift is not running.";
		std::string rus_str = "Этот терминал не внесён в смену или смена не запущена.";
		if (lang == 1) str = eng_str;
		else if (lang == 2) str = rus_str;
		THROW_MESSAGE(1, str, "void try_cashier_register(const std::string& mac_addr)");
	}
	else if (res == 3) {
		std::string str = "language is not set";
		std::string eng_str = "This terminal is not registered on the cafe server.";
		std::string rus_str = "Этот терминал не зарегистрирован в этом кафе.";
		if (lang == 1) str = eng_str;
		else if (lang == 2) str = rus_str;	
		THROW_MESSAGE(2, str, "void try_cashier_register(const std::string& mac_addr)");
	}
	
	if (res != 1) THROW_LOCATION(3, "void try_cashier_register(const std::string& mac_addr)");
}

void try_cashierW_register(const std::string& mac_addr) {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
		"SELECT KSI.USER_TERMINAL_SEANCE_REG_FUNCT(:mac) as vv FROM DUAL"
	);
	query->set("mac", mac_addr);
	query->execute_statement();
	if (query->next()) THROW_LOCATION(0, "void try_cashierW_register(const std::string&)");
	
	int res = static_cast<int>(boost::get<double>(query->get(0)));
	if (res == 2) {
		std::string str = "language is not set";
		std::string eng_str = "This terminal is not registered on the shift or the shift is not running.";
		std::string rus_str = "Этот терминал не внесен в смену или смена не запущена.";
		if (lang == 1) str = eng_str;
		else if (lang == 2) str = rus_str;
		THROW_MESSAGE(1, str, "void try_cashierW_register(const std::string&)");
	}
	else if (res == 3) {
		std::string str = "language is not set";
		std::string eng_str = "This terminal is not registered on the cafe server.";
		std::string rus_str = "Этот терминал не зарегистрирован в этом кафе.";
		if (lang == 1) str = eng_str;
		else if (lang == 2) str = rus_str;
		THROW_MESSAGE(2, str, "void try_cashierW_register(const std::string&)");
	}
	else if (res == 4) {
		std::string str = "language is not set";
		std::string eng_str = "This person is not registered in the shift";
		std::string rus_str = "Этот пользователь не внесен в смену.";
		if (lang == 1) str = eng_str;
		else if (lang == 2) str = rus_str;
		THROW_MESSAGE(3, str, "void try_cashierW_register(const std::string&)");
	}
	
	if (res != 1) THROW_LOCATION(4, "void try_cashierW_register(const std::string&)");
}
