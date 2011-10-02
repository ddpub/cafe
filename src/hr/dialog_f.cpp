#include "dialog_f.h"
#include <wx_util/wx_util.hpp>

#include "lang/xml_lang.h"

extern std::auto_ptr<ksi_cafe::xml_lang_t> local;

/*******************************************************************/
HRSelectFrame::HRSelectFrame(wxWindow *parent, vecparam_t* sel_params, const wxString& caption, std::vector<int> is) 
: wxDialog(parent, wxID_ANY, caption, wxDefaultPosition, wxSize(200, 330), wxDEFAULT_DIALOG_STYLE)
{
	params = sel_params;
	wxString llist [1000];
	for ( int i=0; i< sel_params->size(); i++) llist[i] = to_uc(sel_params->get_name(i));
	sel_list = new wxCheckListBox(this, wxID_ANY, wxPoint(0,0), wxSize(200,260), sel_params->size(), llist);
	for (int i=0; i< sel_params->size(); i++) 
		for (size_t j=0; j<is.size(); j++) 
			if (sel_params->get(i)->get_id() == is[j]) sel_list->Check(i, true);
	wxButton *btn_ok = new wxButton(this, 1, local->get("filter_f", "ok", "Ok"), wxPoint(30, 270), wxSize(65, 25));
	wxButton *btn_cancel = new wxButton(this, 2, local->get("filter_f", "cancel", "Cancel"), wxPoint(105, 270), wxSize(65, 25));
}

/*******************************************************************/
void HRSelectFrame::OnClose(wxCommandEvent& event)
{
	res_filter.clear();
	if (event.GetId() == 1)
	{
		for (int i=0; i<params->size(); i++)
			if (sel_list->IsChecked(i)) 
				res_filter.push_back(params->get(i)->get_id());
	}
	EndModal(event.GetId());
}

/*******************************************************************/
std::string HRSelectFrame::res_str()
{
	std::string res("");
	for (int i=0; i<params->size(); i++) 
		for (size_t j=0; j<res_filter.size(); j++) 
			if (params->get(i)->get_id() == res_filter[j]) res += params->get(i)->get_name() + ", ";
	if (res != "") res = res.substr(0, res.length()-2);
	return res;
}

BEGIN_EVENT_TABLE( HRSelectFrame, wxDialog )
    //EVT_TEXT(wxID_ANY, func) 
	EVT_BUTTON(wxID_ANY, HRSelectFrame::OnClose) 
END_EVENT_TABLE()


/*******************************************************************/
/*******************************************************************/

