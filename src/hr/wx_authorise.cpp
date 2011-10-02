#include "wx_authorise.hpp"
		
#include <ksi_util/users.hpp>
#include <wx_util/wx_util.hpp>
#include <memory>
#include <wx/msgdlg.h>
#include <string>

extern std::string server_host;
extern std::auto_ptr<ksi_cafe::users_t> users;

int wx_mng_auth() {
	wx_window_ptr<auth_f> auth (new auth_f(0, _("manager authorization required"), _("swipe your magnetic card to sign in"), _("Type your login and password")));
	auth->set_host(server_host, false);
	for (;;) {
		int result = auth->ShowModal();
		if (result == wxID_OK) {
				std::auto_ptr<ksi_client::oracle_session_i> new_session_ptr;
				new_session_ptr = ksi_client::create_session();
			try {	
				new_session_ptr->connect(auth->login_mb(), auth->password_mb(), auth->host_mb());
				if (users->user(auth->login_mb()).manager()) {
					int mng_id = users->user(auth->login_mb()).user_id();
					return mng_id;
				}
				break;
			}
	        catch (ksi_client::ksi_exception& ex) {
				wxMessageBox(to_uc(ex.what()));
			}
			catch (...) {
				wxMessageBox(_("int wx_mng_auth(), unexpected error"));
			}
		}
		else if (result == wxID_CANCEL) break;
	}
	return 0;
}

int wx_person_auth() {
	wx_window_ptr<auth_f> auth (new auth_f(0, _("authorization required"), _("swipe your magnetic card to sign in"), _("Type your login and password")));
	auth->set_host(server_host, false);
	for (;;) {
		int result = auth->ShowModal();
		if (result == wxID_OK) {
				std::auto_ptr<ksi_client::oracle_session_i> new_session_ptr;
				new_session_ptr = ksi_client::create_session();
			try {	
				new_session_ptr->connect(auth->login_mb(), auth->password_mb(), auth->host_mb());
				int person_id = users->user(auth->login_mb()).user_id();
				return person_id;
			}
	        catch (ksi_client::ksi_exception& ex) {
				wxMessageBox(to_uc(ex.what()));
			}
			catch (...) {
				wxMessageBox(_("int wx_person_auth(), unexpected error"));
			}
		}
		else if (result == wxID_CANCEL) break;
	}
	return 0;
}

ksi_client::oracle_session_i* wx_unlock() {
	wx_window_ptr<auth_f> auth (new auth_f(0, _("unlock application"), _("swipe your magnetic card to sign in"), _("Type your login and password")));
	auth->set_host(server_host, false);
	auth->cancel_btn->Enable(false);
	for (;;) {
		auth->ShowModal();
		std::auto_ptr<ksi_client::oracle_session_i> new_session_ptr;
		new_session_ptr = ksi_client::create_session();
		try {
			new_session_ptr->connect(auth->login_mb(), auth->password_mb(), auth->host_mb());
			return new_session_ptr.release();
		}
		catch (ksi_client::ksi_exception& ex) {
			wxMessageBox(to_uc(ex.what()));
		}
		catch (...) {
			wxMessageBox(_("int wx_unlock(), unexpected error"));
		}
	}
	return 0;
}
