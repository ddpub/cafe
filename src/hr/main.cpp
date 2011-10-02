#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/app.h>
#include <wx/msgdlg.h>
#include <wx/splash.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include "HR_f.h"
#include "HR_ico.h"
#include "auth_f.h"
#include "base_connect.hpp"
#include "application_register.hpp"
#include <boost/lexical_cast.hpp>
//#include <wx_util/wx_util.hpp>
#include <wx_util/xml_config.hpp>
#include <connect.hpp>
#include "lang/xml_lang.h"
#include <ksi_util/bag_spooler.hpp>

#include <memory>

std::string app_name = "HR Manager";
int app_id = 7;
std::string server_host = "";

std::auto_ptr<xml_config_t> config;
std::auto_ptr<ksi_cafe::xml_lang_t> local;
std::auto_ptr<bag_spooler_t> spooler;

//wx_window_ptr<wxSplashScreen> splash;

class HRApp: public wxApp
{
public:
    virtual bool OnInit();
};

IMPLEMENT_APP(HRApp)

namespace version {
extern std::string version;
extern std::string compamny;
extern std::string product;
extern std::string copyright;
};

bool HRApp::OnInit()
{
    
    //app_name = "HRManager";
	spooler.reset(new bag_spooler_t("cafeserv.ksi.ru"));
    try {
		config.reset(new xml_config_t("hr_manager.xml"));
	}
	catch (exception_t& ex) {
		wxMessageBox(to_uc(ex.user_message()));
		wxExit();
	}
	catch (...) {
		wxMessageBox(_("can't create config file"));
		wxExit();
	}
	
	try
	{
		if (config->get("locale", "exist") == "1")	local.reset(new ksi_cafe::xml_lang_t(config->get("locale", "file"), config->get("locale", "name"), "hr_manager"));
		else local.reset(new ksi_cafe::xml_lang_t());
	}
	catch(exception_t &e)
	{
		local.reset(new ksi_cafe::xml_lang_t());
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.code() + " " + e.message()));
	}
	catch(std::exception &e)
	{
		local.reset(new ksi_cafe::xml_lang_t());
		wxMessageBox(to_uc(e.what()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	catch(...)
	{
		exception_t e(5001, "unexpected error", "HRFrame::HRFrame()");
		//wxMessageBox(to_uc(e.message()));
	}
    
    wx_window_ptr<auth_f> auth (new auth_f(0, to_uc(app_name + " v."+version::version), local->get("auth_f", "login_message_lb1", "Swipe your magnetic card to sign in"), local->get("auth_f", "login_message_lb2", "Type your login and password")));

	auth->set_login(config->get("last_auth", "login"));
	auth->set_host(config->get("last_auth", "host"));

	wxIcon HRIco;
	HRIco.CopyFromBitmap(HR_ico(1));
	auth->SetIcon(HRIco); // To Set App Icon
	
	wxBitmap splash_bitmap = HR_splash(0);
    
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
			try 
			{
					std::string tmp = auth->host_mb();
					int pos = tmp.rfind(":");
					std::string n_host = tmp;
					std::string n_sid = "cafe";
		
					if (pos != std::string::npos) 
					{
						n_host = tmp.substr(0, pos);
						n_sid = tmp.substr(pos+1, tmp.length()-pos);
					} 
					
					session_ptr->connect(auth->login_mb(), auth->password_mb(), n_host, n_sid/*auth->host_mb()*/);
					server_host = n_host; //auth->host_mb();
					ksi_cafe::session.reset(session_ptr.release());
					try_register(app_id);
					break;
			}
			catch (ksi_client::connection_problem_exception& ex) {
				//if (ex.code() == 1017) wxMessageBox(_T("Wrong login/password"));
				wxMessageBox(to_uc(ex.what()));
			}
			catch (ksi_client::ksi_exception& ex) {
				if (ex.code() == 1017) wxMessageBox(_T("Wrong login/password"));
				else wxMessageBox(to_uc(ex.what()));
			}
			catch (exception_t &e) {
				//if (e.code() == 1017) wxMessageBox(_T("Wrong login/password"));
				wxMessageBox(to_uc(e.user_message()));
			}
			catch (...) {
				wxMessageBox(to_uc("unexpected error. HRApp::OnInit(). line 101"));
			}
			/*catch (ksi_client::ksi_exception& ex) 
			{
				if (ex.code() == 0) wxMessageBox(_("ksi_exception, ") + to_uc(std::string(ex.what())));
				else wxMessageBox(_("ksi_exception code: ") + to_uc(boost::lexical_cast<std::string>(ex.code())));
			}*/
		}
		else if (result == wxID_CANCEL) { splash->Close(); return true; }
    }
    wxYield();
    config->set("last_auth", "login", auth->login_mb());
    config->set("last_auth", "host", auth->host_mb());

    auth.reset();
	//splash->GetSplashWindow()->SetTopWindow(wxSplashScreen(splash_bitmap1, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));// (new );
    HRFrame *frame = new HRFrame();

	wxYield();
	splash->Close();
    frame->Show( true );  
	wxYield();
    SetTopWindow( frame );
    return true;
}
