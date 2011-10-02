#ifndef DMITRY_DERGACHEV_12_08_2008_WX_AUTHORISE_H
#define DMITRY_DERGACHEV_12_08_2008_WX_AUTHORISE_H

#include "auth_f.h"
#include <connect.hpp>

int wx_mng_auth();
int wx_person_auth();
ksi_client::oracle_session_i* wx_unlock();

#endif
