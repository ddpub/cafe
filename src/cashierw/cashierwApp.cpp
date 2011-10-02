
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "cashierwApp.h"
#include <wx/splash.h>
#include <shared_modules/cashier_ico.h>
#include <shared_modules/auth_f.h>
#include <wx/msgdlg.h>
#include <wx_util/wx_util.hpp>
#include "main_f.h"
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

IMPLEMENT_APP(cashierwApp);


std::string app_name = "CashierW";
int app_id = 12;
std::string server_host ="";

std::auto_ptr<xml_config_t> config;
std::auto_ptr<file_log_t> flog;
std::auto_ptr<bag_spooler_t> bag_spooler;
std::auto_ptr<ksi_cafe::xml_lang_t> locale;
std::auto_ptr<ksi_cafe::xml_lang_t> shared_locale;

extern int lang;

namespace version {
extern std::string version;
extern std::string product;
};

bool cashierwApp::OnInit() {
	try {
		flog.reset(new file_log_t("cashier.log", 10));
		*flog << "start cashier";
	}
	catch (std::exception& ex) {
		wxMessageBox(to_uc(ex.what()));
		wxExit();
	}

	try {
		*flog << "create xml_config_t";
		config.reset(new xml_config_t("cashierw.xml"));
		*flog << "successful";
	}
	catch (std::exception &ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
		wxExit();
	}
	
	try {
		*flog << "create bag_spooler_t";
		std::string server = config->get("bag_spooler", "server");
		if (server == "") server = "cafeserv.ksi.ru";
		bag_spooler.reset(new bag_spooler_t(server));
		*flog << "successful";
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
		wxExit();
	}
	
	try {
		*flog << "create xml_lang_t";
		if (config->get("locale", "enable") == "1") {
			locale.reset(new ksi_cafe::xml_lang_t(config->get("locale", "file"), config->get("locale", "name"), "cashierw"));
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
	catch (std::exception& ex) {
		*flog << ex.what();
		locale.reset(new ksi_cafe::xml_lang_t());
		wxMessageBox(to_uc(ex.what()));
	}
	*flog << "app_version = " + version::version;
	wx_window_ptr<auth_f> auth (new auth_f(0, to_uc(version::product) + _(" v") + to_uc(version::version), locale->get("auth_f", "login_message_lb1", "Swipe your magnetic card to sign in"), locale->get("auth_f", "login_message_lb2", "Type your login and password")));
	
	auth->set_login(config->get("last_auth", "login"));
	auth->set_host(config->get("last_auth", "host"));
	
	wxIcon cashierIco;
	cashierIco.CopyFromBitmap(CashierBitmapsFunc(9));
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
				//session_ptr->connect(auth->login_mb(), auth->password_mb(), auth->host_mb());
				session_ptr->connect(auth->login_mb(), auth->password_mb(), auth->host_mb(), auth->sid_mb(), auth->port_mb());
				server_host = auth->raw_host_mb();
				ksi_cafe::session.reset(session_ptr.release());
				
				break;
			}
			catch (std::exception& ex) {
				*flog << ex.what();
				wxMessageBox(to_uc(ex.what()));
			}
		}
		else if (result == wxID_CANCEL) { splash->Close(); return true; }
	}
	
	*flog << "Current USER = " + ksi_cafe::session->username();
	
	config->set("last_auth", "login", auth->login_mb());
	config->set("last_auth", "host", auth->raw_host_mb());
	auth.reset();
	try {
		main_f* cashier = new main_f(0);
		cashier->SetIcon(cashierIco);
		if (config->get("resolution", "stay_on_top") == "0" || to_uc(config->get("resolution", "stay_on_top")).MakeUpper() == _("OFF")) {
		}
		else {
			long currStyle = cashier->GetWindowStyle();
			cashier->SetWindowStyle(currStyle|wxSTAY_ON_TOP);
		}
		if (config->get("resolution", "auto") == "1" || to_uc(config->get("resolution", "auto")).MakeUpper() == _("ON")) {
			//cashier->ShowFullScreen(true);
		}
		else {
			//cashier->Show();
		}
		cashier->Show();
		splash->Close();
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		bag_spooler->send_bag(bag_record_t(0, app_id, version::version, ex.what()));
		wxMessageBox(to_uc(ex.what()));
		wxExit();
	}
	
	splash->Close();
	return true;
}
