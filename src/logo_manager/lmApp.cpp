
#if !defined(_UNIX_BUILD_) && !defined(_WIN32_BUILD_)
#error define _UNIX_BUILD_ or _WIN32_BUILD_
#endif

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "lmApp.h"
#include <wx/msgdlg.h>
#include "lm_f.h"

IMPLEMENT_APP(lmApp);

bool lmApp::OnInit()
{
  lm_f* mform = new lm_f(0);
#ifdef _WIN32_BUILD_
	mform->SetIcon(wxICON(main_ico));
#endif  
  mform->Show();
  return true;
}
