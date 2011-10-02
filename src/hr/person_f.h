
#ifndef person_fH
#define person_fH

#include <wx/menu.h>
#include "base_person_f.h"

#include <vector>
#include "HR_users.h"

class HRPerson_f : public base_person_f
{
private:
  HRuser_t& uuser;
  bool is_edit;
  bool card_w;
  //user_t new_usr;
  std::string new_password;
  std::vector<int> new_role;
  std::vector<int> new_cafe;
  
  //wxStaticBitmap *bmp_photo;
  bool exist;
  bool editt;
  bool is_low;
  
  wxMenu *popbmp_menu;
  //load_data();
  //int res;
protected:
	void OnClose(wxCommandEvent& event); 
	void OnCancel(wxCommandEvent& event); 
	void OnCheckPos(wxCommandEvent& event);
	void OnLoadBmp(wxMouseEvent& event);
	void OnPwdCtrl(wxKeyEvent& event);
	void OnConfPwdCtrl(wxKeyEvent& event);
	void OnZipCtrl(wxKeyEvent& event);
	void OnLoginCtrl(wxKeyEvent& event);
	void OnClearBmp(wxMouseEvent& event);
	//{ int c = 0; }
	//void LoginCtrl(wxKeyEvent& event);
	void OnClickHi(wxCommandEvent& event) { rb_low->SetValue(false); }
	void OnBmp(wxCommandEvent& event);
	void OnClickLow(wxCommandEvent& event) { rb_hi->SetValue(false); }
	void OnCheckCard(wxCommandEvent& event) 
	{
		if (event.IsChecked())  { rb_low->Enable(); rb_hi->Enable();	}
		else  {   rb_low->Disable(); rb_hi->Disable();  }
	}
	//void OnEdit(wxCommandEvent& event)
	bool is_clear;
	
	//{ int c=0; }
public:
	HRPerson_f(wxWindow *parent, HRuser_t& usr);
	//~HRPerson_f();
	HRuser_t* get_user() { return &uuser; }
	HRuser_t& get_user_() { return uuser; }

	bool get_is_low() const { return is_low; }
	int Showdlg() { return ShowModal(); }
	std::vector<int>* get_role() { return &new_role; }
	std::vector<int>* get_cafe() { return &new_cafe; }
	std::string get_password() { return new_password; } 
	bool Is_cardW() { return card_w; }	
	//DECLARE_EVENT_TABLE();
};

#endif
