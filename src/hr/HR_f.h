
#ifndef frame_fH
#define frame_fH

#include <wx/menu.h>
#include "base_HR_f.h"
#include "HR_users.h"
#include <vector>

/*******************************************************************/
class HRFrame : public base_HR_f
{
private:
	int actived_id;
	int sel_row;
	
	std::vector<int> user_a;
	std::vector<int> user_f;
	
//----filer & sort	
	std::vector<int> f_cafe;
	std::vector<int> f_role;
	bool sort_order;
	int sort_col;
	
	
	bool actived;
	int actived_page;
	int pos;
	
	wxStaticBitmap *sort_bmp_a;
	wxStaticBitmap *sort_bmp_f;
	
	wxMenu *popup_menu;
/*	
	wxTextCtrl *ed_cafefilter;
	wxTextCtrl *ed_rolefilter;
	wxCheckBox *ck_cafefilter;
	wxCheckBox *ck_rolefilter;
	wxButton *btn_cafe;
	wxButton *btn_role;
	wxPanel *ppp;
*/	
protected:
	void OnPropertyClick(wxMouseEvent& event);
	//void OnResize(wxSizeEvent &event);
	
	void OnSort(wxGridEvent& event) { sort_persons(event.GetCol()); }
	void OnClickPos(wxCommandEvent& event);
	void OnChangeActive(wxNotebookEvent& event);
	
	void OnTextCafeFilter(wxCommandEvent& event);
	void OnTextRoleFilter(wxCommandEvent& event);
	void OnCheckCafeFilter(wxCommandEvent& event);
	void OnCheckRoleFilter(wxCommandEvent& event);
	
	void OnAddpos(wxCommandEvent& event);
	void OnDelpos(wxCommandEvent& event);
	void OnEditpos(wxCommandEvent& event);
	void OnGridDblClick(wxGridEvent& event);
	void OnGridClick(wxGridEvent& event);
	void load_lang();
	
//-----grid
	void load_persons_grid();
	void sort_persons(int ssort = -1);
	void filter();
	void reload_pers();
	void reload_person_grid();
	void add_person_grid();
	
	void OnAdd(wxCommandEvent& event);
	void OnEdit(wxCommandEvent& event);
	void OnFire(wxCommandEvent& event);
	void OnRehire(wxCommandEvent& event);
	void OnUnlock(wxCommandEvent& event);
	void OnDelete(wxCommandEvent& event);
	void OnPopupMenuClick( wxGridEvent& event );
	
	HRuser_t& get_user();
	
public:
	HRFrame();
};

#endif
