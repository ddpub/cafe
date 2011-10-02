/***************************************************************
 * Name:      cashierApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2008-03-07
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "cashierApp.h"
#include <wx/splash.h>
#include <shared_modules/cashier_ico.h>
#include <shared_modules/auth_f.h>
#include <wx/msgdlg.h>
#include <wx_util/wx_util.hpp>
#include "cashier_f.h"
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <memory>
#include <boost/lexical_cast.hpp>
#include <shared_modules/connect.hpp>
#include <wx_util/xml_config.hpp>
#include <ksi_util/log.hpp>
#include <oracle_client/ksi_exception.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <oracle_client/oracle_client.hpp>
#include <ksi_util/bag_spooler.hpp>
#include <ksi_util/url.hpp>
#include <lang/xml_lang.h>
#include <ksi_util/inet.hpp>
#include <fiscal_register/fiscal.hpp>
#include <serial/com.hpp>

IMPLEMENT_APP(cashierApp);


std::string app_name = "Cashier";
int app_id = 5;
std::string server_host ="";
extern int lang;

std::auto_ptr<xml_config_t> config;
std::auto_ptr<file_log_t> flog;
std::auto_ptr<bag_spooler_t> bag_spooler;
std::auto_ptr<ksi_cafe::xml_lang_t> locale;
std::auto_ptr<ksi_cafe::xml_lang_t> shared_locale;


namespace version {
	extern std::string version;
	extern std::string product;
};


bool cashierApp::OnInit() {
	try {
		flog.reset(new file_log_t("cashier.log", 10));
		*flog << "start cashier";
	}
	catch (...) {
		wxMessageBox(_("can't create log file"));
		wxExit();
		return false;
	}

	try {
		*flog << "create xml_config_t";
		config.reset(new xml_config_t("cashier.xml"));
		*flog << "successful";
		if (config->get("app", "frac") == "") config->set("app", "frac", "2");
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()));
		wxExit();
		return false;
	}
	catch (std::exception &ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
		wxExit();
		return false;
	}
	catch (...) {
		*flog << "can't create config file";
		wxMessageBox(_("can't create config file"));
		wxExit();
		return false;
	}
	
	try {
		*flog << "create bag_spooler_t";
		std::string server = config->get("bag_spooler", "server");
		if (server == "") server = "cafeserv.ksi.ru";
		bag_spooler.reset(new bag_spooler_t(server));
		*flog << "successful";
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
		wxExit();
		return false;
	}
	catch (...) {
		*flog << "can't create bag_spooler";
		wxMessageBox(_("can't create bag_spooler"));
		wxExit();
		return false;
	}
		
	try {
		*flog << "create xml_lang_t";
		if (config->get("locale", "enable") == "1") {
			locale.reset(new ksi_cafe::xml_lang_t(config->get("locale", "file"), config->get("locale", "name"), "cashier"));
			shared_locale.reset(new ksi_cafe::xml_lang_t(config->get("locale", "file"), config->get("locale", "name"), "shared"));
			lang = 2;
		}
		else {
			locale.reset(new ksi_cafe::xml_lang_t());
			shared_locale.reset(new ksi_cafe::xml_lang_t());
			lang = 1;
		}
		*flog << "successful";
	}
	catch (exception_t& ex) {
		*flog << ex.what();
		locale.reset(new ksi_cafe::xml_lang_t());
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		locale.reset(new ksi_cafe::xml_lang_t());
		wxMessageBox(to_uc(ex.what()));		
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
		wxExit();
		return false;
	}
	*flog << "app_version = " + version::version;
	wx_window_ptr<auth_f> auth (new auth_f(0, to_uc(version::product) + _(" v") + to_uc(version::version), locale->get("auth_f", "login_message_lb1", "Swipe your magnetic card to sign in"), locale->get("auth_f", "login_message_lb2", "Type your login and password")));
	
	auth->set_login(config->get("last_auth", "login"));
	auth->set_host(config->get("last_auth", "host"));
	
	wxIcon cashierIco;
	cashierIco.CopyFromBitmap(CashierBitmapsFunc(0));
	auth->SetIcon(cashierIco); // To Set App Icon

	wxBitmap splash_bitmap = CashierBitmapsFunc(8);
	{
		wxMemoryDC splashDC;
		splashDC.SelectObject(splash_bitmap);
		wxString text = _("v")+to_uc(version::version);
		int text_len = splashDC.GetTextExtent(text).x;
		splashDC.DrawText(text, splashDC.GetSize().GetWidth()-text_len - 20, splashDC.GetSize().GetHeight()/2);
		splashDC.SelectObject( wxNullBitmap );
	}
	
	wx_window_ptr<wxSplashScreen> splash (new wxSplashScreen(splash_bitmap, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));
	
    for (;;) {
			splash->Hide();
			int result = auth->ShowModal();
			if (result == wxID_OK) {
				splash->Show();
				std::auto_ptr<ksi_client::oracle_session_i> session_ptr = ksi_client::create_session();
				try {
					session_ptr->connect(auth->login_mb(), auth->password_mb(), auth->host_mb(), auth->sid_mb(), auth->port_mb());
					server_host = auth->host_mb() + ":" + boost::lexical_cast<std::string>(auth->port_mb()) + ":" + auth->sid_mb();
					ksi_cafe::session.reset(session_ptr.release());
					break;
				}
				catch (ksi_client::connection_problem_exception& ex) {
					*flog << ex.what();
					wxMessageBox(to_uc(ex.what()));
				}
				catch (ksi_client::db_exception& ex) {
					*flog << ex.what();
					wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
				}
				catch (ksi_client::ksi_exception& ex) {
					*flog << ex.what();
					wxMessageBox(to_uc(ex.what()));
				}
				catch (std::exception& ex) {
					*flog << LLOCATION;
					*flog << ex.what();
					wxMessageBox(to_uc(ex.what()));
				}
				catch (...) {
					*flog << ERROR_IN_LOCATION;
					wxMessageBox(to_uc(ERROR_IN_LOCATION));
				}
			}
			else if (result == wxID_CANCEL) { splash->Close(); return true; }
    }
	
		*flog << "Current USER = " + ksi_cafe::session->username();

		config->set("last_auth", "login", auth->login_mb());
		config->set("last_auth", "host", server_host);
		auth.reset();

	if (config->get("ext", "mode") == "1") {
		try {
			*flog << "ext_mode=1";
			std::vector<std::string> mac_vec = get_mac();
			std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
			int successful = -1;
			std::string mac = "";
			for (size_t i=0; i<mac_vec.size(); ++i) {
				query->create_statement
				(
				"select ksi.SEANCE_EXIST('" + mac_vec[i] + "') from dual"
				);
				query->execute_statement();
				int res =  static_cast<int>(boost::get<double>(query->get(0)));
				if (res == 0) { // not exist
					mac = mac_vec[i];
					successful = 0;
					break;
				}
				else if (res == 1) { // exist
					mac = mac_vec[i];
					successful = 1;
					break;
				}
				else if (res == -1) { // error
					successful = -1;
				}
			}
			if (successful == 0 && mac != "") { // not exist
				*flog << "seance not exist";
				std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
				query->create_statement
				(
				"begin ksi.CREATE_SESSION('" + mac + "'); end;"
				);
				query->execute_statement();
				
				/*begin work*/
				if ((to_uc(config->get("printer", "source")).MakeUpper() == _("LOCAL")) && (to_uc(config->get("printer", "command_set")).MakeUpper() == _("FISCAL REGISTRAR"))) {
					//com_t com(config->get("printer", "port"), boost::lexical_cast<int>(config->get("printer", "baud_rate")), 3);
					//fiscal::registrator_t registrar(com);
					ksi_cafe::session->commit();
				}
				else
					wxMessageBox(_("Bad config"));
				ksi_cafe::session->rollback();
				/*end work*/
			}
			else if (successful = 1 && mac != "") { // exist
				*flog << "seance exist";
			}
			else { // error
				*flog << "seace error";
				wxMessageBox(_("This terminal is not registered on the shift or the shift is not running."));
				wxExit();
				return false;
			}
		}
		catch (exception_t& ex) {
			*flog << LLOCATION;
			*flog << ex.comp_message();
			bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.comp_message()));
			wxExit();
			return false;
		}
		catch (ksi_client::db_exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ex.what()));
			wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
			wxExit();
			return false;
		}
		catch (ksi_client::ksi_exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			wxExit();
			return false;
		}
		catch (fiscal::exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			wxExit();
			return false;
		}
		catch (std::exception& ex) {
			*flog << LLOCATION;
			*flog << ex.what();
			bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ex.what()));
			wxMessageBox(to_uc(ex.what()));
			wxExit();
			return false;
		}
		catch (...) {
			*flog << ERROR_IN_LOCATION;
			bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ERROR_IN_LOCATION));
			wxMessageBox(to_uc(ERROR_IN_LOCATION));
			wxExit();
			return false;
		}
	}
	
	try {
		cashier_f* cashier = new cashier_f(0);
		cashier->SetIcon(cashierIco);
		if (config->get("resolution", "stay_on_top") == "0" || to_uc(config->get("resolution", "stay_on_top")).MakeUpper() == _("OFF")) {
		}
		else {
			cashier->SetWindowStyle(wxSTAY_ON_TOP|wxTAB_TRAVERSAL);
		}
		if (config->get("resolution", "auto") == "1" || to_uc(config->get("resolution", "auto")).MakeUpper() == _("ON")) {
			cashier->ShowFullScreen(true);
		}
		else cashier->Show();
		splash->Close();
	}
	catch (exception_t& ex) {
		*flog << LLOCATION;
		*flog << ex.comp_message();
		bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.user_message()));
		wxExit();
		return false;
	}
	catch (ksi_client::ksi_exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		wxExit();
		return false;
	}
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		wxExit();
		return false;
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ERROR_IN_LOCATION));
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
		wxExit();
		return false;
	}
	
	splash->Close();
	return true;
}
