#include "HRpos_f.h"
#include <wx_util/wx_util.hpp>

#include "lang/xml_lang.h"
#include <wx_util/xml_config.hpp>
#include "position_f.h"
#include <memory>
#include "HR_ico.h"

extern int pos;

extern std::auto_ptr<ksi_cafe::users_t> users;
extern std::auto_ptr<vecparam_t> positionrole;
extern std::auto_ptr<vecparam_t> all_role;
extern std::auto_ptr<ksi_cafe::xml_lang_t> local;

/*******************************************************************/
/*  HRPosToolBar                                                   */
/*******************************************************************/
HRPosToolBar::HRPosToolBar(wxWindow *parent)
: wxToolBar(parent, wxID_ANY, wxPoint(20,20)/*DefaultPosition*/,
	wxSize(parent->GetSize().GetWidth(), 30), wxBORDER_NONE|wxTB_HORIZONTAL|wxTB_NODIVIDER|wxTB_FLAT|wxTB_TEXT|wxTAB_TRAVERSAL )
{
  try
  {
	SetToolBitmapSize(wxSize(30, 30));
	AddTool(1, local->get("HR_f", "add_pos", "Add"), HR_tool(2));  //BITMAP_TYPE_BMP
	AddTool(3, local->get("HR_f", "edit_pos", "Edit"), HR_tool(1));  //BITMAP_TYPE_BMP
	AddTool(2, local->get("HR_f", "del_pos", "Delete"), HR_tool(0));

	AddSeparator();
	
	if (!(users->user().administrator())) 
	{
		EnableTool(1, false);
		EnableTool(2, false);
		EnableTool(3, false);
	}

	if (!positionrole->loaded()) positionrole->load_positionrole();
	wxString pposition [1000];
	for ( int i=0; i< positionrole->size(); i++) pposition[i] = to_uc(positionrole->get_name(i));
	lb_position = new HRListBox(parent, positionrole->size(), pposition);
	wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_LISTBOX_SELECTED, 0);  
	lb_position->OnClickPos(evt);
	lb_position->Select(0);
	pos = 0;

	Realize();
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
  }
  catch(...)
  {
	exception_t e(5022, "unexpected error", "HRPosToolBar::HRPosToolBar");
	wxMessageBox(to_uc(e.message()));
  }
}
/*******************************************************************/
void HRPosToolBar::reload_lang()
{
    ClearTools();
	AddTool(1, local->get("HR_f", "add_pos", "Add"), HR_tool(3));  //BITMAP_TYPE_BMP
	AddTool(3, local->get("HR_f", "add_pos", "Edit"), HR_tool(2));
	AddTool(2, local->get("HR_f", "del_pos", "Delete"), HR_tool(0));
}
/*******************************************************************/
void HRPosToolBar::OnResize(wxSizeEvent& event)
{
  try
  {
	lb_position->SetSize(event.GetSize().GetWidth()/2-2,event.GetSize().GetHeight()-100);
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
  }
  catch(...)
  {
	exception_t e(5023, "unexpected error", "void HRPosToolBar::OnResize");
	wxMessageBox(to_uc(e.message()));
  }
}
/*******************************************************************/

void HRPosToolBar::OnClickTool(wxCommandEvent& event)
{
  try
  {	
	if (!(users->user().administrator())) 
	{
		wxMessageBox(local->get("HR_f", "dont_admin", "You dont't have privileges"));
		return;
	}
	if (event.GetId() == 1) //add new position
	{
			
		HRposition_f *form = new HRposition_f(this, -1);

		//HRPosFrame *pos_dlg = new HRPosFrame(this);
		if (form->Showdlg() != 1 ) return;
		
		//SetCursor(*wxHOURGLASS_CURSOR);
		
		param_t* new_pos = new param_t(pos, form->get_res());
		for (int i=0; i<form->get_size(); i++)	new_pos->add(form->get_role(i));
		positionrole->add_position(new_pos);
		wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_LISTBOX_SELECTED, pos);  
		lb_position->OnClickPos(evt);
		lb_position->Select(pos);
		
		//SetCursor(*wxSTANDARD_CURSOR);
	}
	else if (event.GetId() == 2) //delete position
	{
		//SetCursor(*wxHOURGLASS_CURSOR);
		int ress = wxMessageBox(local->get("HR_f", "is_del_question", "Are you sure?"), local->get("HR_f", "caption", "HRManager"), wxYES_NO );
		if (ress != wxYES) return;
		positionrole->del_position(pos);
		lb_position->Delete(pos);
		wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_LISTBOX_SELECTED, 0);  
		lb_position->OnClickPos(evt);
		lb_position->Select(0);
		
		//etCursor(*wxSTANDARD_CURSOR);
	}
	else if (event.GetId() == 3) //edit position
	{

		HRposition_f *form = new HRposition_f(this, pos);
		
		//SetCursor(*wxHOURGLASS_CURSOR);
		//HRPosFrame *pos_dlg = new HRPosFrame(this);
		
		if (form->Showdlg() != 1 ) return;
		param_t* ed_pos = new param_t(positionrole->get(pos)->get_id(), form->get_res());
		for (int i=0; i<form->get_size(); i++)	ed_pos->add(form->get_role(i));
		positionrole->edit_position(ed_pos);
		wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_LISTBOX_SELECTED, pos);  
		lb_position->OnClickPos(evt);
		lb_position->Select(pos);
		//SetCursor(*wxSTANDARD_CURSOR);
	}
  }
  catch(exception_t &e)
  {
	if (e.code() == 20701) wxMessageBox(local->get("HR_f", "error_del_pers", "Persons with this position exist"));
	else wxMessageBox(to_uc(e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
  }
  catch(...)
  {
	exception_t e(5024, "unexpected error", "HRPosToolBar::OnClickTool");
	wxMessageBox(to_uc(e.message()));
  }
}


/*******************************************************************/
/*  HRListBox                                                      */
/*******************************************************************/
HRListBox::HRListBox(wxWindow *parent, int n, wxString choices[]/*, vecparam_t& Positions*/)
: wxListBox(parent, wxID_ANY, wxPoint(0,50), wxSize(parent->GetSize().GetWidth()/2-2, parent->GetSize().GetHeight()-130), n, choices)
  /*positions(Positions),*/ 
{
  try
  {
	if (!all_role->loaded()) all_role->load_role();
	wxString rrole [1000];
	for ( int i=0; i < all_role->size(); i++) rrole[i] = to_uc(all_role->get_name(i));
	cl_role = new wxCheckListBox(parent, wxID_ANY, wxPoint(parent->GetSize().GetWidth()/2, 50), wxSize(parent->GetSize().GetWidth()/2-1,parent->GetSize().GetHeight()-30), all_role->size(), rrole, wxTE_READONLY);
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
  }
  catch(...)
  {
	exception_t e(5026, "unexpected error", "HRListBox::HRListBox");
	wxMessageBox(to_uc(e.message()));
  }
}

/*******************************************************************/
void HRListBox::OnClickPos(wxCommandEvent& event)
{
  try
  {
	pos = event.GetInt();
	int id = positionrole->get(pos)->get_id();//!!
	//wxMessageBox(to_uc(boost::lexical_cast<std::string>(pos) + " / " + boost::lexical_cast<std::string>(id)));
	std::string nid = positionrole->get(pos)->get_name();//!!
	for (int i=0; i<all_role->size(); i++)
	{
		if (positionrole->get(pos)->find_id(all_role->get(i)->get_id()) != -1) cl_role->Check(i, true);
		else cl_role->Check(i, false);
	}
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
  }
  catch(...)
  {
	exception_t e(5027, "unexpected error", "HRListBox::OnClickPos");
	wxMessageBox(to_uc(e.message()));
  }
}
/*******************************************************************/
void HRListBox::OnResize(wxSizeEvent& event)
{
  try
  {
	cl_role->Move(event.GetSize().GetWidth()+1/*/2*/, 50);
	cl_role->SetSize(event.GetSize().GetWidth()/*/2-1*/-8,event.GetSize().GetHeight()); //-100
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
  }
  catch(...)
  {
	exception_t e(5028, "unexpected error", "HRListBox::OnResize");
	wxMessageBox(to_uc(e.message()));
  }
}

/*******************************************************************/
BEGIN_EVENT_TABLE( HRListBox, wxListBox )
    EVT_LISTBOX(wxID_ANY, HRListBox::OnClickPos)
	EVT_SIZE(HRListBox::OnResize)
END_EVENT_TABLE()
/*******************************************************************/
BEGIN_EVENT_TABLE( HRPosToolBar, wxToolBar )
	EVT_TOOL(wxID_ANY, HRPosToolBar::OnClickTool)
	EVT_SIZE(HRPosToolBar::OnResize)
END_EVENT_TABLE()


/*******************************************************************/
/*******************************************************************/

