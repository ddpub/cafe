#include "position_f.h"

#include <wx_util/xml_config.hpp>
#include <serial/com.hpp>
#include "lang/xml_lang.h"

#include "HR_users.h"

extern std::auto_ptr<ksi_cafe::xml_lang_t> local;
extern std::auto_ptr<vecparam_t> positionrole;
extern std::auto_ptr<vecparam_t> all_role;

HRposition_f::HRposition_f(wxWindow *parent, int pos_id) 
: base_position_f(parent, wxID_ANY, wxT("auth"), wxDefaultPosition, wxSize(300, 300), wxDEFAULT_DIALOG_STYLE)
{
    //wxWindow *pos_frame = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxSize(GetSize()), wxTAB_TRAVERSAL);

    //res = "";
	//wxStaticText *lb_name = new wxStaticText(pos_frame, 1, local->get("position_f", "pos_name", "Position name"), wxPoint(5, 7), wxSize(70, 16));
	lb_name->SetLabel(local->get("position_f", "pos_name", "Position name"));
	//tc_posname = new wxTextCtrl(pos_frame, 2, _T(""), wxPoint(75, 5), wxSize(150, 20), wxALIGN_LEFT);
	
	//wxStaticText *lt_role = new wxStaticText(pos_frame, 1, local->get("position_f", "roles_lb", "Roles"), wxPoint(5, 25), wxSize(70, 16));
	lb_role->SetLabel(local->get("position_f", "roles_lb", "Roles"));
	if (!all_role->loaded()) all_role->load_role();
	//wxString rrole [1000];
	for ( int i=0; i < all_role->size(); i++) cl_role->Append(to_uc(all_role->get_name(i))) ; //rrole[i] = to_uc(all_role->get_name(i));
	//cl_role = new wxCheckListBox(pos_frame, wxID_ANY, wxPoint(2, 45), wxSize(280, 150), all_role->size(), rrole);

	if (pos_id != -1)
	{
		tc_posname->SetValue(to_uc(positionrole->get(pos_id)->get_name()));
		for (int i=0; i<all_role->size(); i++)
		{
			if (positionrole->get(pos_id)->find_id(all_role->get(i)->get_id()) != -1) cl_role->Check(i, true);
			else cl_role->Check(i, false);
		}
	}
	//wxButton *btn_ok = new wxButton(pos_frame, 1, local->get("position_f", "ok", "Ok"), wxPoint(60, 210), wxSize(65, 25));
	//wxButton *btn_cancel = new wxButton(pos_frame, 2, local->get("position_f", "cancel", "Cancel"), wxPoint(165, 210), wxSize(65, 25));
	
	btn_ok->SetLabel(local->get("position_f", "ok", "Ok"));
	btn_cancel->SetLabel(local->get("position_f", "cancel", "Cancel"));
}
/*******************************************************************/
void HRposition_f::OnClose(wxCommandEvent& event) 
{ 
	if (event.GetId() == 1) 
	{
		res = to_mb(tc_posname->GetValue());
		if ( res == "") 
		{
			wxMessageBox(local->get("position_f", "pos_error", "Position name must be defined"), local->get("addpos_f", "caption", "HR manager"), wxOK);
			return;
		}
	}
	
	for (int i=0; i<all_role->size(); i++)
	{
		if (cl_role->IsChecked(i)) new_roles.push_back(all_role->get(i)->get_id());
	}
	
	EndModal(event.GetId());
}

void HRposition_f::OnEnd(wxCommandEvent& event) 
{ 
	EndModal(event.GetId());
}


/*BEGIN_EVENT_TABLE( HRposition_f, wxDialog )
	EVT_BUTTON(wxID_ANY, HRposition_f::OnClose) 
END_EVENT_TABLE()*/


