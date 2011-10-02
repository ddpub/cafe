
#ifndef position_fH
#define position_fH

#include "base_position_f.h"
#include <wx/msgdlg.h>
//#include <wx/string.h>
#include <wx/dialog.h>
//#include <wx/calctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/textctrl.h>
//#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/checklst.h>
#include <wx/filedlg.h>
#include <vector>
#include <time.h>

class HRposition_f : public base_position_f
{
private:
/*	wxButton *btn_ok;
	wxButton *btn_cancel;
	wxTextCtrl *tc_posname;
	wxCheckListBox *cl_role;
	*/
	std::string res;
	std::vector<int> new_roles;
public:
	HRposition_f(wxWindow *parent, int pos_id);
	int Showdlg() { return ShowModal(); }
	int get_size() const { return new_roles.size(); }
	int get_role(int id) const { return new_roles[id]; }
	std::string get_res() const { return res; }
	std::vector<int>& get_new_roles() { return new_roles; }
	void OnClose(wxCommandEvent& event); 
	void OnEnd(wxCommandEvent& event); 
	
	//DECLARE_EVENT_TABLE();
};

#endif
