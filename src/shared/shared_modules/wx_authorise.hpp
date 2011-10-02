#ifndef DMITRY_DERGACHEV_12_08_2008_WX_AUTHORISE_H
#define DMITRY_DERGACHEV_12_08_2008_WX_AUTHORISE_H

#include "auth_f.h"
#include <shared_modules/connect.hpp>

int wx_mng_auth(wxWindow* wnd_ptr);
int wx_person_auth(wxWindow* wnd_ptr);
ksi_client::oracle_session_i* wx_unlock(wxWindow* wnd_ptr);

#endif
