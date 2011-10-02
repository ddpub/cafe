#ifndef HRpos_fH
#define HRpos_fH

//#include <wx/event.h>
//#include <wx/window.h>

#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/checklst.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/toolbar.h>
#include <wx/datectrl.h>
#include <time.h>
#include <wx/datetime.h>

#include "HR_users.h"

/*******************************************************************/
class HRListBox : public wxListBox
{
private:

	wxCheckListBox *cl_role;
protected:
	
public:
	void OnResize(wxSizeEvent& event);
	void OnClickPos(wxCommandEvent& event);
	HRListBox(wxWindow *parent, int n, wxString choices[]/*, vecparam_t& Positions*/);
	
	DECLARE_EVENT_TABLE();
};
/*******************************************************************/
class HRPosToolBar : public wxToolBar
{
private:
	HRListBox *lb_position;

	//vecparam_t positions;
protected:
	void OnClickTool(wxCommandEvent& event);// { this->SetCursor(*wxSTANDARD_CURSOR); for( int i=0; i<100000; i++) int c(0); this->SetCursor(*wxSTANDARD_CURSOR); }
public:
	void OnResize(wxSizeEvent& event);
	HRPosToolBar(wxWindow *parent);
	
	void reload_lang();
	
	DECLARE_EVENT_TABLE();
};



#endif



