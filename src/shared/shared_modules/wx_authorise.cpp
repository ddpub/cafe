#include "wx_authorise.hpp"
		

#include <ksi_util/users.hpp>
#include <wx_util/wx_util.hpp>
#include <memory>
#include <wx/msgdlg.h>
#include <string>
#include <lang/xml_lang.h>

extern std::string server_host;
extern std::auto_ptr<ksi_cafe::users_t> users;
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;
extern std::auto_ptr<ksi_cafe::xml_lang_t> shared_locale;

int wx_mng_auth(wxWindow* wnd_ptr) {
	wx_window_ptr<auth_f> auth (new auth_f(wnd_ptr, locale->get("global", "manager_auth_required_str", "manager authorization required"), locale->get("global", "swipe_your_magnetic_card_to_sign_in_str", "swipe your magnetic card to sign in"), locale->get("global", "tpye_your_login_and_password_str", "Type your login and password")));
	auth->set_host(server_host, false);
	for (;;) {
		int result = auth->ShowModal();
		if (result == wxID_OK) {
				std::auto_ptr<ksi_client::oracle_session_i> new_session_ptr;
				new_session_ptr = ksi_client::create_session();
			try {	
				new_session_ptr->connect(auth->login_mb(), auth->password_mb(), auth->host_mb(), auth->sid_mb(), auth->port_mb());
				if (users->user(auth->login_mb()).manager()) {
					int mng_id = users->user(auth->login_mb()).user_id();
					return mng_id;
				}
				break;
			}
			catch (ksi_client::db_exception& ex) {
				wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Error"), wxOK, wnd_ptr);
			}
	        catch (ksi_client::ksi_exception& ex) {
				wxMessageBox(to_uc(ex.what()), _("Error"), wxOK, wnd_ptr);
			}
			catch (std::exception& ex) {
				wxMessageBox(to_uc(ex.what()), _("Error"), wxOK, wnd_ptr);
			}
		}
		else if (result == wxID_CANCEL) break;
	}
	return 0;
}

int wx_person_auth(wxWindow* wnd_ptr) {
	wx_window_ptr<auth_f> auth (new auth_f(wnd_ptr, locale->get("global", "manager_auth_required_str", "manager authorization required"), locale->get("global", "swipe_your_magnetic_card_to_sign_in_str", "swipe your magnetic card to sign in"), locale->get("global", "tpye_your_login_and_password_str", "Type your login and password")));
	auth->set_host(server_host, false);
	for (;;) {
		int result = auth->ShowModal();
		if (result == wxID_OK) {
				std::auto_ptr<ksi_client::oracle_session_i> new_session_ptr;
				new_session_ptr = ksi_client::create_session();
			try {	
				new_session_ptr->connect(auth->login_mb(), auth->password_mb(), auth->host_mb(), auth->sid_mb(), auth->port_mb());
				int person_id = users->user(auth->login_mb()).user_id();
				return person_id;
			}
			catch (ksi_client::db_exception& ex) {
				wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Error"), wxOK, wnd_ptr);
			}
	        catch (ksi_client::ksi_exception& ex) {
				wxMessageBox(to_uc(ex.what()), _("Error"), wxOK, wnd_ptr);
			}
		}
		else if (result == wxID_CANCEL) break;
	}
	return 0;
}

ksi_client::oracle_session_i* wx_unlock(wxWindow* wnd_ptr) {
	wx_window_ptr<auth_f> auth (new auth_f(wnd_ptr, locale->get("global", "unlock_application_str", "unlock application"), locale->get("global", "swipe_your_magnetic_card_to_sign_in_str", "swipe your magnetic card to sign in"), locale->get("global", "tpye_your_login_and_password_str", "Type your login and password")));
	auth->set_host(server_host, false);
	auth->cancel_btn->Enable(false);
	for (;;) {
		auth->ShowModal();
		std::auto_ptr<ksi_client::oracle_session_i> new_session_ptr;
		new_session_ptr = ksi_client::create_session();
		try {
			new_session_ptr->connect(auth->login_mb(), auth->password_mb(), auth->host_mb(), auth->sid_mb(), auth->port_mb());
			return new_session_ptr.release();
		}
		catch (ksi_client::db_exception& ex) {
			wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second), _("Error"), wxOK, wnd_ptr);
		}
		catch (ksi_client::ksi_exception& ex) {
			wxMessageBox(to_uc(ex.what()), _("Error"), wxOK, wnd_ptr);
		}
	}
	return 0;
}
