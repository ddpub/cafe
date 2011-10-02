#ifndef dialog_fH
#define dialog_fH

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
//#include <boost/lexical_cast.hpp>
#include <wx/datectrl.h>
#include <time.h>
#include <wx/datetime.h>

#include "HR_users.h"

class HRSelectFrame : public wxDialog
{
private:
	vecparam_t* params;
	wxCheckListBox *sel_list;
protected:
	void OnClose(wxCommandEvent& event);
public:
    std::vector<int> res_filter;
	HRSelectFrame(wxWindow *parent, vecparam_t* sel_params, const wxString& caption, std::vector<int> is);
	int Showdlg() { return ShowModal(); }
	std::string res_str();
	int get_res_size() { return res_filter.size(); }
	int get_res(int id) { return res_filter[id]; }
	DECLARE_EVENT_TABLE();
};



#endif



