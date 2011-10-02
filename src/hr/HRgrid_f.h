#ifndef HRgrid_fH
#define HRgrid_fH

//#include <wx/event.h>
//#include <wx/window.h>

#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/combobox.h>

#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/toolbar.h>
#include <wx/notebook.h>
#include <wx/statbmp.h>
#include <wx/checkbox.h>
#include <ksi_util/users.hpp>
#include <vector>


//#include <wx/checklst.h>
//#include <wx/checkbox.h>
//#include <wx/dialog.h>
//#include <boost/lexical_cast.hpp>
//#include <wx/datectrl.h>
//#include <time.h>
//#include <wx/datetime.h>

#include "HR_users.h"

class HRGridNotebook : public wxNotebook
{
private:
	std::vector<int> user_a;
	std::vector<int> user_f;
	wxGrid *gr_pers_a;
	wxGrid *gr_pers_f;
	
	std::vector<int> f_cafe;
	std::vector<int> f_role;
	int sel_row;
	int sort_col;
	bool actived;
	bool sort_order;
	
	int actived_page;
	
	wxStaticBitmap *sort_bmp_a;
	wxStaticBitmap *sort_bmp_f;
protected:
	wxWindow *CreatePersonGrid();
	void OnSelect(wxGridEvent& event);
	void OnSort(wxGridEvent& event);
	void OnDblClick(wxGridEvent& event);
	
	//void OnEdit(wxCommandEvent& event) { this->SetCursor(*wxSTANDARD_CURSOR); for( int i=0; i<100000; i++) int c(0); this->SetCursor(*wxSTANDARD_CURSOR); }
public:
	//HRusers_t* all_user;
	void Filter(/*std::vector<int> f_role, std::vector<int> f_cafe*/);
	void Sort(int ssort);
	void OnResize(wxSizeEvent& event);
	
	void OnChangeActive(wxNotebookEvent& event);
	
	void load_persons();
	void reload();
	
	void OnAdd();
	void OnEdit();
	void OnFire();
	void OnRehire();
	void OnUnlock();
	void OnDelete();
	HRuser_t *get_usr();
	bool IsSelected();
	HRGridNotebook(wxWindow *parent);
	int get_selected_id() { return sel_row; }
	void set_actived(bool res) { actived = res; }
	
	void clear_fcafe() { f_cafe.clear(); }
	void add_fcafe(int id) { f_cafe.push_back(id); }
	
	void clear_frole() { f_role.clear(); }
	void add_frole(int id) { f_role.push_back(id); }
	
	std::vector<int>& get_fcafe() { return f_cafe; }
	std::vector<int>& get_frole() { return f_role; }
	
	int size_fcafe() { return f_cafe.size(); }
	int size_frole() { return f_role.size(); }
	
	void reload_lang();
	
	bool is_busy;
	
	DECLARE_EVENT_TABLE()
};

/*******************************************************************/
class HRPersToolBar : public wxToolBar
{
private:
	wxTextCtrl *ed_cafefilter;
	wxTextCtrl *ed_rolefilter;

	wxCheckBox *ck_cafefilter;
	wxCheckBox *ck_rolefilter;

	wxButton *btn_cafe;
	wxButton *btn_role;
	
	wxPanel *ppp;
	wxPanel *ppp1;
	
	//vecparam_t all_role;
	//vecparam_t all_cafe;

	HRGridNotebook *nb_person;

protected:
	void OnClickTool(wxCommandEvent& event); //{ this->SetCursor(*wxSTANDARD_CURSOR); for( int i=0; i<100000; i++) int c(0); this->SetCursor(*wxSTANDARD_CURSOR); }
	void OnCheckFilter(wxCommandEvent& event);
	void OnTextFilter(wxCommandEvent& event);
public:
	void OnResize(wxSizeEvent& event);
	HRPersToolBar(wxWindow *parent);
	~HRPersToolBar();
	
	void OnChangeActive(wxNotebookEvent& event);

	void reload_lang();
	
	DECLARE_EVENT_TABLE();
};



#endif



