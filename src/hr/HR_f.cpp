#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/app.h>
#include <wx/msgdlg.h>
//#include <wx/splash.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>

#include <wx/toplevel.h>
//#include <wx/splash.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>

#include "HR_f.h"
#include "HR_ico.h"
#include "person_f.h"
#include "property_f.h"
#include "lang/xml_lang.h"
#include "mcard.h"
#include "dialog_f.h"
#include "position_f.h"

#include <wx_util/wx_util.hpp>
#include <wx_util/xml_config.hpp>
#include <serial/com.hpp>
#include <ksi_util/users.hpp>
#include <oracle_client/ksi_exception.hpp>
#include "lang/xml_lang.h"
#include <ksi_util/bag_spooler.hpp>


#include <memory>

namespace version {
extern std::string version;
extern std::string compamny;
extern std::string product;
extern std::string copyright;
};

extern std::string app_name;
extern int app_id;
extern std::string server_host;

extern std::auto_ptr<xml_config_t> config;
extern std::auto_ptr<ksi_cafe::xml_lang_t> local;
extern std::auto_ptr<bag_spooler_t> spooler;

//extern wx_window_ptr<wxSplashScreen> splash;
//extern std::string splash_str;

//int pos;
bool is_valide_com = false;

std::auto_ptr<ksi_cafe::users_t> users;
std::auto_ptr<vecparam_t> state;
std::auto_ptr<vecparam_t> positionrole;
std::auto_ptr<vecparam_t> all_cafe;
std::auto_ptr<vecparam_t> all_role;
std::auto_ptr<HRusers_t> all_user;
std::auto_ptr<com_t> com;

HRFrame::HRFrame() : base_HR_f( (wxFrame *)NULL ) //, -1, _T("HRManager")/*, wxDefaultPosition, wxSize(600, 400)*/)
{
  wxIcon ico_;
  ico_.CopyFromBitmap(HR_ico(0));
  SetIcon(ico_);
  
  wxBitmap splash_bitmap1  = wxBitmap(50, 150); //HR_splash(0);
    
	/*{
		wxMemoryDC splashDC;
		splashDC.SelectObject(splash_bitmap1);
		//wxString text = _("v")+to_uc(version::version);
		wxString text1 = local->get("load_f", "load_state", "Loading states");
		int text_len = splashDC.GetTextExtent(text).x;
		splashDC.DrawText(text, splashDC.GetSize().GetWidth()-text_len - 20, splashDC.GetSize().GetHeight()/2 + 25);
		splashDC.SelectObject( wxNullBitmap );
	}
	wx_window_ptr<wxSplashScreen> splash1 (new wxSplashScreen(splash_bitmap1, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));*/
  
  try
  {	
    //wxString ss = local->get("HR_f", "cc", "cc");
	//double gg = local->reverse_currency(ss);
	/*wxBitmap splash_bitmap1 = wxBitmap(50, 150);
	{
		wxMemoryDC splashDC;
		splashDC.SelectObject(splash_bitmap1);
		wxString text1 = local->get("load_f", "load_state", "Loading states");
		int text_len1 = splashDC.GetTextExtent(text1).x;
		splashDC.DrawText(text1, splashDC.GetSize().GetWidth()-text_len1 - 20, splashDC.GetSize().GetHeight()/2+25);
		splashDC.SelectObject( wxNullBitmap );
	}
	splash1.reset(new wxSplashScreen(splash_bitmap1, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));
	*/
	state.reset(new vecparam_t());	
	users.reset(new ksi_cafe::users_t());
	/*splash_bitmap1 = wxBitmap(50, 150);
	{
		wxMemoryDC splashDC;
		splashDC.SelectObject(splash_bitmap1);
		wxString text1 = local->get("load_f", "load_pos", "Loading positions");
		int text_len1 = splashDC.GetTextExtent(text1).x;
		splashDC.DrawText(text1, splashDC.GetSize().GetWidth()-text_len1 - 20, splashDC.GetSize().GetHeight()/2 + 25);
		splashDC.SelectObject( wxNullBitmap );
	}
	splash1.reset(new wxSplashScreen(splash_bitmap1, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));
	wxYield();*/
	positionrole.reset(new vecparam_t());
	/*splash_bitmap1 = wxBitmap(50, 150);
	{
		wxMemoryDC splashDC;
		splashDC.SelectObject(splash_bitmap1);
		wxString text1 = local->get("load_f", "load_cafe", "Loading cafes");
		int text_len1 = splashDC.GetTextExtent(text1).x;
		splashDC.DrawText(text1, splashDC.GetSize().GetWidth()-text_len1 - 20, splashDC.GetSize().GetHeight()/2 + 25);
		splashDC.SelectObject( wxNullBitmap );
	}
	splash1.reset(new wxSplashScreen(splash_bitmap1, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));
	wxYield();*/
	all_cafe.reset(new vecparam_t());
	/*splash_bitmap1 = wxBitmap(50, 150);
	{
		wxMemoryDC splashDC;
		splashDC.SelectObject(splash_bitmap1);
		wxString text1 = local->get("load_f", "load_role", "Loading role");
		int text_len1 = splashDC.GetTextExtent(text1).x;
		splashDC.DrawText(text1, splashDC.GetSize().GetWidth()-text_len1 - 20, splashDC.GetSize().GetHeight()/2 + 25);
		splashDC.SelectObject( wxNullBitmap );
	}
	splash1.reset(new wxSplashScreen(splash_bitmap1, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));
	wxYield();*/
	all_role.reset(new vecparam_t());
	/*splash_bitmap1 = wxBitmap(50, 150);
	{
		wxMemoryDC splashDC;
		splashDC.SelectObject(splash_bitmap1);
		wxString text1 = local->get("load_f", "load_state", "Loading users");
		int text_len1 = splashDC.GetTextExtent(text1).x;
		splashDC.DrawText(text1, splashDC.GetSize().GetWidth()-text_len1 - 20, splashDC.GetSize().GetHeight()/2 + 25);
		splashDC.SelectObject( wxNullBitmap );
	}
	splash1.reset(new wxSplashScreen(splash_bitmap1, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));
	wxYield();*/
	all_user.reset(new HRusers_t());
	wxYield();
	wxInitAllImageHandlers();
	wxYield();
	
	/*splash_bitmap1 = wxBitmap(50, 150);
	{
		wxMemoryDC splashDC;
		splashDC.SelectObject(splash_bitmap1);
		wxString text1 = local->get("load_f", "load_com", "Connect com");
		int text_len1 = splashDC.GetTextExtent(text1).x;
		splashDC.DrawText(text1, splashDC.GetSize().GetWidth()-text_len1 - 20, splashDC.GetSize().GetHeight()/2 + 25);
		splashDC.SelectObject( wxNullBitmap );
	}
	splash1.reset(new wxSplashScreen(splash_bitmap1, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));
	wxYield();*/
	try
	{
		int baud_rate(9600);
		std::string ccom_name = config->get("com_port", "port");
		if (ccom_name != "")
		{
		   com.reset(new com_t(config->get("com_port", "port"), 9600/*boost::lexical_cast<int>(config->get("com_port", "baud_rate"))*/, 2));
		   if ((config->get("com_port", "baud_rate")) != "9600") config->set("com_port", "baud_rate", "9600");
		   is_valide_com = true;
		}
		else is_valide_com = false; 
	}
	catch (exception_t& ex) 
	{
	    wxMessageBox(to_uc(ex.user_message()));
		//wxMessageBox(local->get("errors","cannt_read_com","Cannot read COM port"));
		spooler->send_bag(bag_record_t(1, app_id, version::version, ex.message()));
		//
		is_valide_com = false;
	}
	catch (...) 
	{
		wxMessageBox(local->get("errors","cannt_open_com","Can't open serial port"));
		is_valide_com = false;
	}
	/*splash_bitmap1 = wxBitmap(50, 150);
	{
		wxMemoryDC splashDC;
		splashDC.SelectObject(splash_bitmap1);
		wxString text1 = local->get("load_f", "load_if", "Loading interface");
		int text_len1 = splashDC.GetTextExtent(text1).x;
		splashDC.DrawText(text1, splashDC.GetSize().GetWidth()-text_len1 - 20, splashDC.GetSize().GetHeight()/2 + 25);
		splashDC.SelectObject( wxNullBitmap );
	}
	splash1.reset(new wxSplashScreen(splash_bitmap1, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));
	wxYield();*/
	//wxString ss_test = local->get_currency(10000.2345);
	//wxMessageBox(ss_test);
	
	//double cccc = local->reverse_currency(ss_test);
	
	SetLabel(to_uc(std::string("HRManager - ") + users->user().fio()));
	st_bar->SetLabel(to_uc(version::copyright + ", " + version::product + " ver. " + version::version)); 
	
	/**ppp = new wxPanel(tb_person, wxID_ANY, wxDefaultPosition, wxSize(210, 50));
	
	ck_cafefilter = new wxCheckBox(ppp, wxID_ANY, local->get("HR_f", "cafe_f", "Cafe filter"), wxPoint(0,5));
	ck_rolefilter = new wxCheckBox(ppp, wxID_ANY, local->get("HR_f", "role_f", "Role filter"), wxPoint(0,30));
	ck_cafefilter->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(HRFrame::OnCheckCafeFilter), NULL, this );
	ck_rolefilter->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(HRFrame::OnCheckRoleFilter), NULL, this );
	
	ed_cafefilter = new wxTextCtrl(ppp, wxID_ANY, _T(""), wxPoint(100,2), wxSize(150, 20), wxTE_READONLY );
	ed_rolefilter = new wxTextCtrl(ppp, wxID_ANY, _T(""), wxPoint(100,27), wxSize(150, 20), wxTE_READONLY );
	btn_cafe = new wxButton(ed_cafefilter, wxID_ANY, _T("..."), wxPoint(130,0), wxSize(16,17));
	btn_role = new wxButton(ed_rolefilter, wxID_ANY, _T("..."), wxPoint(130,0), wxSize(16,17));
	btn_cafe->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HRFrame::OnTextCafeFilter), NULL, this );
	btn_role->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HRFrame::OnTextRoleFilter), NULL, this );
	
	tb_person->AddControl((wxControl*) ppp);
	tb_person->Realize();
	*/
	if (!all_cafe->loaded()) all_cafe->load_cafe();
	if (!all_role->loaded()) all_role->load_role();

	/*try
	{
	tb_person->SetToolNormalBitmap( ADDPERS_ID, HR_tool(4) );
	tb_person->SetToolNormalBitmap( EDITPERS_ID, HR_tool(1) );
	tb_person->SetToolNormalBitmap( FIREPERS_ID, HR_tool(5) );
	tb_person->SetToolNormalBitmap( REHIREPERS_ID, HR_tool(4) );
	tb_person->SetToolNormalBitmap( UNLOCKPERS_ID, HR_tool(3) );
	tb_person->SetToolNormalBitmap( DELPERS_ID, HR_tool(0) );
	
	tb_position->SetToolNormalBitmap( ADDPOS_ID, HR_tool(2) );
	tb_position->SetToolNormalBitmap( EDITPOS_ID, HR_tool(1) );
	tb_position->SetToolNormalBitmap( DELPOS_ID, HR_tool(0) );
	}
	catch(...)
	{  }*/
	
	popup_menu = new wxMenu();
	//popup_menu->Append(ADDPERS_ID, local->get("HR_f", "btn_add", "Hire") );
	popup_menu->Append(EDITPERS_ID, local->get("HR_f", "btn_edit", "Edit"));
	popup_menu->Append(FIREPERS_ID, local->get("HR_f", "btn_fire", "Fire"));
	popup_menu->Append(REHIREPERS_ID, local->get("HR_f", "btn_rehire", "Rehire"));
	popup_menu->Append(UNLOCKPERS_ID, local->get("HR_f", "btn_unlock", "Unlock"));
	popup_menu->Append(DELPERS_ID, local->get("HR_f", "btn_del", "Delete"));
	
	popup_menu->Enable(REHIREPERS_ID, false);
	
	
	load_lang();
	
	tb_person->EnableTool(REHIREPERS_ID, false);
	if (!(users->user().administrator())) 
	{
		tb_position->EnableTool(ADDPOS_ID, false);
		tb_position->EnableTool(EDITPOS_ID, false);
		tb_position->EnableTool(DELPOS_ID, false);
		tb_person->EnableTool(DELPERS_ID, false);
	}
	
	sort_bmp_a = new wxStaticBitmap( gr_pers_a, wxID_ANY, wxNullBitmap, wxPoint(0,0), wxDefaultSize, 0 );
	sort_bmp_f = new wxStaticBitmap( gr_pers_f, wxID_ANY, wxNullBitmap, wxPoint(0,0), wxDefaultSize, 0 );
	
	for (int i=0; i<all_user->size(); i++)
		if (all_user->get_user(i)->fired()) user_f.push_back(all_user->get_user(i)->user_id());
		else user_a.push_back(all_user->get_user(i)->user_id());
	load_persons_grid();
	
	if (!positionrole->loaded()) positionrole->load_positionrole();
	for ( int i=0; i< positionrole->size(); i++) lb_position->Append(to_uc(positionrole->get_name(i))); // pposition[i] = to_uc(positionrole->get_name(i));
	
	if (!all_role->loaded()) all_role->load_role();
	for ( int i=0; i < all_role->size(); i++) cl_role->Append(to_uc(all_role->get_name(i)));
	
	wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_LISTBOX_SELECTED, 0);  
	OnClickPos(evt);
	lb_position->Select(0);
	pos = 0;
	
	actived_id = -1;
	sel_row = -1;
	nb_person->ChangeSelection(0);
	
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.user_message()));
	spooler->send_bag(bag_record_t(1, app_id, version::version, e.code() + " " + e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
	//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));	
  }
  //splash1->Close();
}

/*HRFrame::~HRFrame() //: ~base_HR_f()
{
	ck_cafefilter->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(HRFrame::OnCheckCafeFilter), NULL, this );
	ck_rolefilter->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(HRFrame::OnCheckRoleFilter), NULL, this );
	btn_cafe->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HRFrame::OnTextCafeFilter), NULL, this );
	btn_role->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HRFrame::OnTextRoleFilter), NULL, this );
	//~base_HR_f();
	
}*/
/*******************************************************************/
void HRFrame::load_lang()
{
	tb_person->FindById(ADDPERS_ID)->SetLabel( local->get("HR_f", "btn_add", "Hire") );
	tb_person->FindById(EDITPERS_ID)->SetLabel( local->get("HR_f", "btn_edit", "Edit") );
	tb_person->FindById(FIREPERS_ID)->SetLabel( local->get("HR_f", "btn_fire", "Fire") );
	tb_person->FindById(REHIREPERS_ID)->SetLabel( local->get("HR_f", "btn_rehire", "Rehire") );
	tb_person->FindById(UNLOCKPERS_ID)->SetLabel( local->get("HR_f", "btn_unlock", "Unlock") );
	tb_person->FindById(DELPERS_ID)->SetLabel( local->get("HR_f", "btn_del", "Delete") );
	
	ck_cafefilter->SetLabel(local->get("HR_f", "cafe_filter", "Cafe filter"));
	ck_rolefilter->SetLabel(local->get("HR_f", "role_filter", "Role filter"));
	
	tb_person->Realize();
	
	tb_position->FindById(ADDPOS_ID)->SetLabel( local->get("HR_f", "add_pos", "Add") );
	tb_position->FindById(EDITPOS_ID)->SetLabel( local->get("HR_f", "edit_pos", "Edit") );
	tb_position->FindById(DELPOS_ID)->SetLabel( local->get("HR_f", "del_pos", "Delete") );
	tb_position->Realize();
	
	gr_pers_a->SetColLabelValue(0, local->get("HR_f", "login", " login "));
	gr_pers_a->SetColLabelValue(1, local->get("HR_f", "firstname", "firstname"));
	gr_pers_a->SetColLabelValue(2, local->get("HR_f", "lastname", "lastname"));
	gr_pers_a->SetColLabelValue(3, local->get("HR_f", "surname", "surname"));
	gr_pers_a->SetColLabelValue(4, local->get("HR_f", "birthday", "birthday"));
	
	gr_pers_f->SetColLabelValue(0, local->get("HR_f", "login", " login "));
	gr_pers_f->SetColLabelValue(1, local->get("HR_f", "firstname", "firstname"));
	gr_pers_f->SetColLabelValue(2, local->get("HR_f", "lastname", "lastname"));
	gr_pers_f->SetColLabelValue(3, local->get("HR_f", "surname", "surname"));
	gr_pers_f->SetColLabelValue(4, local->get("HR_f", "birthday", "birthday"));
	
	nb_HR->SetPageText(0, local->get("HR_f", "person", "Personnel"));
	nb_HR->SetPageText(1, local->get("HR_f", "position", "Positions"));
	
	nb_person->SetPageText(0, local->get("HR_f", "active", "Active"));
	nb_person->SetPageText(1, local->get("HR_f", "fired", "Fired"));
	
	//popup_menu->SetLabel(ADDPERS_ID, local->get("HR_f", "btn_add", "Hire") );
	popup_menu->SetLabel(EDITPERS_ID, local->get("HR_f", "btn_edit", "Edit"));
	popup_menu->SetLabel(FIREPERS_ID, local->get("HR_f", "btn_fire", "Fire"));
	popup_menu->SetLabel(REHIREPERS_ID, local->get("HR_f", "btn_rehire", "Rehire"));
	popup_menu->SetLabel(UNLOCKPERS_ID, local->get("HR_f", "btn_unlock", "Force unlock"));
	popup_menu->SetLabel(DELPERS_ID, local->get("HR_f", "btn_del", "Delete"));
/*	
	ck_cafefilter->SetLabel(local->get("HR_f", "cafe_f", "Cafe filter"));
	ck_rolefilter->SetLabel(local->get("HR_f", "role_f", "Role filter"));
*/
}
/*******************************************************************/
/*void HRFrame::OnResize(wxSizeEvent &event)
{
  try
  {
	/*int wight_new = gr_pers_a->GetSize().GetWidth()- 30;
	for (int i=0; i<5; i++)
	{
		gr_pers_a->SetColSize(i, wight_new/5);
		gr_pers_f->SetColSize(i, wight_new/5);
	}
	//gr_pers_a->SetSize(event.GetSize().GetWidth()- 28, event.GetSize().GetHeight()-7 );//-40
	//gr_pers_f->SetSize(event.GetSize().GetWidth()- 28, event.GetSize().GetHeight()-7 );//-28  -7	
		
	wxPoint pos_a = gr_pers_a->CellToRect(1, sort_col+1).GetPosition();
	wxPoint pos_f = gr_pers_f->CellToRect(1, sort_col+1).GetPosition();
	
	sort_bmp_a->Move(pos_a.x-20, 5);
	sort_bmp_f->Move(pos_f.x-20, 5);
	*/
	//nb_active->SetSize(event.GetSize().GetWidth()-7, event.GetSize().GetHeight()-27);
	//Refresh();
	//Update();
 /* }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
  }
}

/*******************************************************************/

void HRFrame::OnChangeActive(wxNotebookEvent& event)
{
	if (tb_person->GetToolsCount() < 5) return;
		
	if (event.GetSelection() == 0) 
	{
		tb_person->EnableTool(REHIREPERS_ID, false);
		tb_person->EnableTool(ADDPERS_ID, true);
		tb_person->EnableTool(FIREPERS_ID, true);
		tb_person->EnableTool(UNLOCKPERS_ID, true);
		//nb_person->ChangeSelection(event.GetSelection());
		
		popup_menu->Enable(REHIREPERS_ID, false);
		popup_menu->Enable(FIREPERS_ID, true);
		popup_menu->Enable(UNLOCKPERS_ID, true);
	}
	else if (event.GetSelection() == 1) 
	{
		tb_person->EnableTool(REHIREPERS_ID, true);
		tb_person->EnableTool(ADDPERS_ID, false);
		tb_person->EnableTool(FIREPERS_ID, false);
		tb_person->EnableTool(UNLOCKPERS_ID, false);
		
		popup_menu->Enable(REHIREPERS_ID, true);
		popup_menu->Enable(FIREPERS_ID, false);
		popup_menu->Enable(UNLOCKPERS_ID, false);
		//nb_person->ChangeSelection(event.GetSelection());
	}
	event.Skip();
	//Update();
}

/*BEGIN_EVENT_TABLE( HRFrame, wxFrame )
	EVT_SIZE(HRFrame::OnResize)
END_EVENT_TABLE()*/

/*******************************************************************/
/*-----------------------------------------personnel ----------------------------------------------*/
/*******************************************************************/
void HRFrame::load_persons_grid()
{
  try
  {	
	SetCursor(*wxHOURGLASS_CURSOR);

	//----- active
	if (gr_pers_a->GetNumberRows() > 0) gr_pers_a->DeleteRows(0, gr_pers_a->GetNumberRows());
	gr_pers_a->AppendRows(user_a.size());
	for (size_t i=0; i<user_a.size(); i++)
	{
		//gr_pers_a->AppendRows(1);
		HRuser_t& usr = all_user->find_user(user_a[i]);
		std::string nnname = usr.get_firstname();
		gr_pers_a->SetCellValue(i, 0, to_uc(usr.get_login()));
		gr_pers_a->SetCellValue(i, 1, to_uc(usr.get_firstname()));
		gr_pers_a->SetCellValue(i, 2, to_uc(usr.get_lastname()));
		gr_pers_a->SetCellValue(i, 3, to_uc(usr.get_surname()));
		gr_pers_a->SetCellValue(i, 4, (usr.get_birthdate() == boost::date_time::not_a_date_time ? _T("") : local->get_date(usr.get_birthdate())));
	}

//----- fired	
	if (gr_pers_f->GetNumberRows() > 0) gr_pers_f->DeleteRows(0, gr_pers_f->GetNumberRows());
	gr_pers_f->AppendRows(user_f.size());
	for (size_t i=0; i<user_f.size(); i++)
	{
		HRuser_t& usr = all_user->find_user(user_f[i]);
		std::string nnname = usr.get_firstname();
		gr_pers_f->SetCellValue(i, 0, to_uc(usr.get_login()));
		gr_pers_f->SetCellValue(i, 1, to_uc(usr.get_firstname()));
		gr_pers_f->SetCellValue(i, 2, to_uc(usr.get_lastname()));
		gr_pers_f->SetCellValue(i, 3, to_uc(usr.get_surname()));
		gr_pers_f->SetCellValue(i, 4, (usr.get_birthdate() == boost::date_time::not_a_date_time ? _T("") : local->get_date(usr.get_birthdate())));
	}
	Update();
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.user_message()));
	spooler->send_bag(bag_record_t(1, app_id, version::version, e.code() + " " + e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
	//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
  }
  SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
void HRFrame::reload_person_grid()
{
	try
	{
		if (nb_person->GetSelection() == 0) 
		{
			HRuser_t& usr = all_user->find_user(user_a[sel_row]);
			gr_pers_a->SetCellValue(sel_row, 0, to_uc(usr.get_login()));
			gr_pers_a->SetCellValue(sel_row, 1, to_uc(usr.get_firstname()));
			gr_pers_a->SetCellValue(sel_row, 2, to_uc(usr.get_lastname()));
			gr_pers_a->SetCellValue(sel_row, 3, to_uc(usr.get_surname()));
			gr_pers_a->SetCellValue(sel_row, 4, (usr.get_birthdate() == boost::date_time::not_a_date_time ? _T("") : local->get_date(usr.get_birthdate())));
		}
		else if (nb_person->GetSelection() == 1) 
		{
			HRuser_t& usr = all_user->find_user(user_f[sel_row]);
			gr_pers_f->SetCellValue(sel_row, 0, to_uc(usr.get_login()));
			gr_pers_f->SetCellValue(sel_row, 1, to_uc(usr.get_firstname()));
			gr_pers_f->SetCellValue(sel_row, 2, to_uc(usr.get_lastname()));
			gr_pers_f->SetCellValue(sel_row, 3, to_uc(usr.get_surname()));
			gr_pers_f->SetCellValue(sel_row, 4, (usr.get_birthdate() == boost::date_time::not_a_date_time ? _T("") : local->get_date(usr.get_birthdate())));
		}
		Update();
	}
	catch(exception_t &e)
	{
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.code() + " " + e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
void HRFrame::add_person_grid()
{
	try
	{
		gr_pers_a->AppendRows(1);
		if (nb_person->GetSelection() == 0) 
		{
			
			sel_row = gr_pers_a->GetNumberRows();
			HRuser_t& usr = all_user->find_user(user_a[sel_row]);
			gr_pers_a->SetCellValue(sel_row, 0, to_uc(usr.get_login()));
			gr_pers_a->SetCellValue(sel_row, 1, to_uc(usr.get_firstname()));
			gr_pers_a->SetCellValue(sel_row, 2, to_uc(usr.get_lastname()));
			gr_pers_a->SetCellValue(sel_row, 3, to_uc(usr.get_surname()));
			gr_pers_a->SetCellValue(sel_row, 4, (usr.get_birthdate() == boost::date_time::not_a_date_time ? _T("") : local->get_date(usr.get_birthdate())));
			user_a.push_back(usr.user_id());
		}
		else if (nb_person->GetSelection() == 1) 
		{
			sel_row = gr_pers_f->GetNumberRows();
			HRuser_t& usr = all_user->find_user(user_f[sel_row]);
			gr_pers_f->SetCellValue(sel_row, 0, to_uc(usr.get_login()));
			gr_pers_f->SetCellValue(sel_row, 1, to_uc(usr.get_firstname()));
			gr_pers_f->SetCellValue(sel_row, 2, to_uc(usr.get_lastname()));
			gr_pers_f->SetCellValue(sel_row, 3, to_uc(usr.get_surname()));
			gr_pers_f->SetCellValue(sel_row, 4, (usr.get_birthdate() == boost::date_time::not_a_date_time ? _T("") : local->get_date(usr.get_birthdate())));
			user_f.push_back(usr.user_id());
		}

		Update();
	}
	catch(exception_t &e)
	{
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}

/*******************************************************************/
void HRFrame::sort_persons(int ssort)
{
  try
  {
	//all_user.sort(event.GetCol, sort_order);
	if (ssort != -1) 
	{
		sort_col = ssort;
		if (sort_order) sort_order = false; else sort_order = true;
	}
	if (ssort == 0 || ssort == 1 || ssort == 2 || ssort == 3)
	{
	
	//if (sort_col != 0 && sort_col != 1 && sort_col != 2 && sort_col != 3 && sort_col != -1) return;

	std::vector<std::string> new_user_a;
	std::vector<std::string> new_user_f;
	
	for (size_t i=0; i<user_a.size(); i++)
		new_user_a.push_back(to_mb(gr_pers_a->GetCellValue(i, sort_col)));

	for (size_t i=0; i<user_f.size(); i++)
		new_user_f.push_back(to_mb(gr_pers_f->GetCellValue(i, sort_col)));

	if (user_a.size() > 1)
	for (size_t i=0; i < user_a.size()-1; i++)
	{
		for (int j=i+1; j<user_a.size(); j++)
			if (!sort_order && new_user_a[i] > new_user_a[j])
			{
				int buf = user_a[i];	   std::string buf_str = new_user_a[i];
				user_a[i] = user_a[j];     new_user_a[i] = new_user_a[j];
				user_a[j] = buf;           new_user_a[j] = buf_str;
			}
			else if (sort_order && new_user_a[i] < new_user_a[j])
			{
				int buf = user_a[i];	   std::string buf_str = new_user_a[i];
				user_a[i] = user_a[j];     new_user_a[i] = new_user_a[j];
				user_a[j] = buf;           new_user_a[j] = buf_str;
			}
	}

	if (user_f.size() > 1)
	for (size_t i=0; i<user_f.size()-1; i++)
	{
		for (int j=i+1; j<user_f.size(); j++)
			if (!sort_order && new_user_f[i] > new_user_f[j])
			{
				int buf = user_f[i];	   std::string buf_str = new_user_f[i];
				user_f[i] = user_f[j];     new_user_f[i] = new_user_f[j];
				user_f[j] = buf;           new_user_f[j] = buf_str;
			}
			else if (sort_order && new_user_f[i] < new_user_f[j])
			{
				int buf = user_f[i];	   std::string buf_str = new_user_f[i];
				user_f[i] = user_f[j];     new_user_f[i] = new_user_f[j];
				user_f[j] = buf;           new_user_f[j] = buf_str;
			}
	}
	load_persons_grid();
	
	wxBitmap sort_map;
	if (sort_order) sort_map = HR_sort(0); else sort_map = HR_sort(1);
	
	wxPoint pos_a = gr_pers_a->CellToRect(0,ssort+1).GetPosition();
	wxPoint pos_f = gr_pers_f->CellToRect(0,ssort+1).GetPosition();
	
	sort_bmp_a->Move(pos_a.x-20, 7);
	sort_bmp_f->Move(pos_f.x-20, 7);
	
	sort_bmp_a->SetBitmap(sort_map);
	sort_bmp_f->SetBitmap(sort_map);

	Refresh();
	Update();
	}
		
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.user_message()));
	spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
	//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
  }
}
/*******************************************************************/
void HRFrame::reload_pers()
{
  try
  {
	user_a.clear(); user_f.clear();
	for (int i=0; i<all_user->size(); i++)
	{
		if (all_user->get_user(i)->fired()) user_f.push_back(all_user->get_user(i)->user_id());
		else user_a.push_back(all_user->get_user(i)->user_id());
	}
	filter();
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.user_message()));
	spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
	//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
  }
}
/*******************************************************************/
void HRFrame::filter()
{
  try
  {
	user_f.clear(); user_a.clear();
	bool add_role;
	bool add_cafe;
	for (int i=0; i<all_user->size(); i++)
	{
		add_role = false;   if (f_role.size() == 0) add_role = true;
		add_cafe = false;   if (f_cafe.size() == 0) add_cafe = true;
		for (size_t j=0; j<f_role.size(); j++)	if (all_user->get_user(i)->find_role(f_role[j]) != -1)
		{
			add_role = true;
			break;
		}
		for (size_t j=0; j<f_cafe.size(); j++)	if (all_user->get_user(i)->find_cafe(f_cafe[j]) != -1)
		{
			add_cafe = true;
			break;
		}
		if (add_cafe && add_role)
			if (all_user->get_user(i)->fired()) user_f.push_back(all_user->get_user(i)->user_id());
			else user_a.push_back(all_user->get_user(i)->user_id());
	}
	//sort_persons();
	load_persons_grid();
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.user_message()));
	spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
	//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
  }
}
/*******************************************************************/

void HRFrame::OnTextCafeFilter(wxCommandEvent& event)
{
	try
	{
		std::auto_ptr<HRSelectFrame> dlg (new HRSelectFrame(this, all_cafe.get(), local->get("filter_f", "caption_cafe", "cafe filter"), f_cafe));
		if (dlg->Showdlg() != 1) 
		{
			if (f_cafe.size() == 0) ck_cafefilter->SetValue(false);	
			ed_cafefilter->SetValue(_T(""));
			return;
		}
		f_cafe.clear();
		if (dlg->get_res_size() == 0) 
		{
			ck_cafefilter->SetValue(false);
			ed_cafefilter->SetValue(_T(""));
			return;
		}
		ck_cafefilter->SetValue(true);
		for (int i=0; i< dlg->get_res_size(); i++)
			f_cafe.push_back(dlg->get_res(i));

		ed_cafefilter->SetValue(to_uc(dlg->res_str()));
		filter();
	}
	catch(exception_t &e)
	{
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
}
/*******************************************************************/
void HRFrame::OnTextRoleFilter(wxCommandEvent& event)
{
	try
	{
		std::auto_ptr<HRSelectFrame> dlg (new HRSelectFrame(this, all_role.get(), local->get("filter_f", "caption_role", "role filter"), f_role));
		if (dlg->Showdlg() != 1) 
		{
			if (f_role.size() == 0) ck_rolefilter->SetValue(false);	
			ed_rolefilter->SetValue(_T(""));			
			return;
		}
		f_role.clear();
		if (dlg->get_res_size() == 0) 
		{
			ck_rolefilter->SetValue(false);
			ed_rolefilter->SetValue(_T(""));
			return;
		}
		ck_rolefilter->SetValue(true);
		for (int i=0; i< dlg->get_res_size(); i++)
			f_role.push_back(dlg->get_res(i));

		ed_rolefilter->SetValue(to_uc(dlg->res_str()));
		filter();
	}
	catch(exception_t &e)
	{
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
}

/*******************************************************************/
void HRFrame::OnCheckCafeFilter(wxCommandEvent& event)
{
	try
	{
		if (ck_cafefilter->IsChecked())
		{
			wxCommandEvent ee = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, wxID_ANY);
			OnTextCafeFilter(ee);
		}
		else
		{
			f_cafe.clear();
			ed_cafefilter->SetValue(_T(""));
			filter();
		}
	}
	catch(exception_t &e)
	{
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
}
/*******************************************************************/
void HRFrame::OnCheckRoleFilter(wxCommandEvent& event)
{
	try
	{
		if (ck_rolefilter->IsChecked())
		{
			wxCommandEvent ee = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, wxID_ANY);
			OnTextRoleFilter(ee);
		}
		else
		{
			f_role.clear();
			ed_rolefilter->SetValue(_T(""));
			filter();
		}

	}
	catch(exception_t &e)
	{
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
}
/*******************************************************************/
void HRFrame::OnGridDblClick(wxGridEvent& event)
{
	wxCommandEvent ee = wxCommandEvent(wxEVT_COMMAND_TOOL_CLICKED, EDITPERS_ID);
	OnEdit(ee);
}
/*******************************************************************/
void HRFrame::OnGridClick(wxGridEvent& event)
{
	sel_row = event.GetRow();

	gr_pers_a->ClearSelection();
	gr_pers_a->SelectRow(event.GetRow());
	gr_pers_a->SetGridCursor(event.GetRow(), event.GetCol());

	gr_pers_f->ClearSelection();
	gr_pers_f->SelectRow(event.GetRow());
	gr_pers_f->SetGridCursor(event.GetRow(), event.GetCol());
}
/*******************************************************************/
HRuser_t& HRFrame::get_user()
{
	if (nb_person->GetSelection() == 0) return all_user->find_user( user_a[sel_row] );
	else if (nb_person->GetSelection() == 1) return all_user->find_user( user_f[sel_row] );
	else throw exception_t(5001, to_mb(local->get("errors", "usr_not_found", "user not found")), "HRFrame::get_user. line 708");
}
/*******************************************************************/
void HRFrame::OnAdd(wxCommandEvent& event)
{
	try
	{  
		SetCursor(*wxHOURGLASS_CURSOR);
		
		bool wr_card(false);
		std::string n_login("");
		std::string n_pwd("");
		bool n_low(false);
		{
			HRuser_t load_usr = HRuser_t(0);

			std::auto_ptr<HRPerson_f> p_frame (new HRPerson_f(this, load_usr));
			SetCursor(*wxSTANDARD_CURSOR);

			if (p_frame->Showdlg() != 1) return;
			SetCursor(*wxHOURGLASS_CURSOR);
			load_usr.upd(p_frame->get_user_());
			all_user->insert_user(load_usr, p_frame->get_role(), p_frame->get_cafe(), p_frame->get_password());			
			if (p_frame->Is_cardW())
			{
				wr_card = true;
				n_login = p_frame->get_user()->get_login();
				n_pwd = p_frame->get_password();
				n_low = p_frame->get_is_low();
			}
		}
		if (wr_card )
		{
			SetCursor(*wxSTANDARD_CURSOR);
			if (is_valide_com) WriteCard(n_login, n_pwd, n_low);
			else wxMessageBox(local->get("errors", "com_notvalid", "COM-port is not valid"));
			SetCursor(*wxHOURGLASS_CURSOR);
		}
		reload_pers();
		sort_persons();
	}
	catch(exception_t &e)
	{
		if (e.code() == 1) 
			wxMessageBox(local->get("HR_f", "unique_login", "User with this login already exist. You must change login."));
		else 
		{
			wxMessageBox(to_uc(e.user_message()));
			spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
		}
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
void HRFrame::OnEdit(wxCommandEvent& event)
{
	try 
	{
		
		if (nb_person->GetSelection() == -1) return;
		/*if (nb_person->GetSelection() == 0 && sel_row == 0) 
		{
			wxMessageBox(_T("Select user "), _T("HR manager"), wxOK);
			return;
		}
		if (nb_person->GetSelection() == 0 && !gr_pers_f->IsSelection())
		{
			wxMessageBox(_T("Select user "), _T("HR manager"), wxOK);
			return;
		}*/
		//SetCursor(*wxHOURGLASS_CURSOR);
		bool wr_card(false);
		std::string n_login("");
		std::string n_pwd("");
		bool n_low(false);
		{
			HRuser_t& load_usr = get_user();

			load_usr.reload();

			std::auto_ptr<HRPerson_f> p_frame(new HRPerson_f(this, load_usr));
			//SetCursor(*wxSTANDARD_CURSOR);

			if (p_frame->Showdlg() != 1) return;

			//SetCursor(*wxHOURGLASS_CURSOR);
			
			all_user->update_user(p_frame->get_user_(), load_usr.user_id(), p_frame->get_role(), p_frame->get_cafe(), p_frame->get_password()); 
			if (p_frame->Is_cardW())
			{
				wr_card = true;
				n_login = p_frame->get_user()->get_login();
				n_pwd = p_frame->get_password();
				n_low = p_frame->get_is_low();
				//SetCursor(*wxSTANDARD_CURSOR);
				//WriteCard(p_frame->get_user()->get_login(), p_frame->get_password(), p_frame->get_is_low());
				//SetCursor(*wxHOURGLASS_CURSOR);
			}
		}
		if (wr_card)
		{
			if (is_valide_com) WriteCard(n_login, n_pwd, n_low);
			else wxMessageBox(local->get("errors", "com_notvalid", "COM-port is not valid"));
		}
		load_persons_grid();
	}
	catch (exception_t &e)
	{
		if (e.code() == 1) 
			wxMessageBox(local->get("HR_f", "unique_login", "User with this login already exist. You must change login."));
		else
		{
			wxMessageBox(to_uc(e.user_message()));
			spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
		}
	}
	catch (std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/	
void HRFrame::OnFire(wxCommandEvent& event)
//Fire
{
	try 
	{
		if (nb_person->GetSelection() == -1) return;
		if (nb_person->GetSelection() == 0 && !gr_pers_a->IsSelection()) 
		{
			wxMessageBox(local->get("HR_f", "select_usr", "Select user"), local->get("HR_f", "caption", "HR manager"), wxOK);
			return;
		}
		if (nb_person->GetSelection() == 0 && !gr_pers_f->IsSelection())
		{
			wxMessageBox(local->get("HR_f", "select_usr", "Select user"), local->get("HR_f", "caption", "HR manager"), wxOK);
			return;
		}
		
		int ress = wxMessageBox(local->get("HR_f", "is_firepers_question", "Are you sure?"), local->get("HR_f", "caption", "HRManager"), wxYES_NO );
		if (ress != wxYES) return;
		
		SetCursor(*wxHOURGLASS_CURSOR);

		
		HRuser_t& load_usr = get_user();
		
		all_user->fire_user(load_usr);
		reload_pers();
		sort_persons();
		
		//sort_persons();
					//Filter();
	}
	catch (exception_t &e)
	{
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
	}
	catch (std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
void HRFrame::OnRehire(wxCommandEvent& event)
//Rehire
{
	try 
	{
		if (nb_person->GetSelection() == -1) return;
		if (nb_person->GetSelection() == 0 && !gr_pers_a->IsSelection()) 
		{
			wxMessageBox(local->get("HR_f", "select_usr", "Select user"), local->get("HR_f", "caption", "HR manager"), wxOK);
			return;
		}
		if (nb_person->GetSelection() == 0 && !gr_pers_f->IsSelection())
		{
			wxMessageBox(local->get("HR_f", "select_usr", "Select user"), local->get("HR_f", "caption", "HR manager"), wxOK);
			return;
		}
		SetCursor(*wxHOURGLASS_CURSOR);

		HRuser_t& load_usr = get_user();

		all_user->rehire_user(load_usr);
		reload_pers();
		sort_persons();
		//sort_persons();
	}
	catch (exception_t &e)
	{
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
	}
	catch (std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
void HRFrame::OnUnlock(wxCommandEvent& event)
//Unlock
{
	try 
	{
		if (nb_person->GetSelection() == -1) return;
		if (nb_person->GetSelection() == 0 && !gr_pers_a->IsSelection()) 
		{
			wxMessageBox(local->get("HR_f", "select_usr", "Select user"), local->get("HR_f", "caption", "HR manager"), wxOK);
			return;
		}
		if (nb_person->GetSelection() == 0 && !gr_pers_f->IsSelection())
		{
			wxMessageBox(local->get("HR_f", "select_usr", "Select user"), local->get("HR_f", "caption", "HR manager"), wxOK);
			return;
		}
		SetCursor(*wxHOURGLASS_CURSOR);

		HRuser_t& load_usr = get_user();
		
		all_user->unlock_user(load_usr);
	}
	catch (exception_t &e)
	{
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
	}
	catch (std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
void HRFrame::OnDelete(wxCommandEvent& event)
//Delete
{
	try 
	{
		if (nb_person->GetSelection() == -1) return;
		if (nb_person->GetSelection() == 0 && !gr_pers_a->IsSelection()) 
		{
			wxMessageBox(local->get("HR_f", "select_usr", "Select user"), local->get("HR_f", "caption", "HR manager"), wxOK);
			return;
		}
		if (nb_person->GetSelection() == 0 && !gr_pers_f->IsSelection())
		{
			wxMessageBox(local->get("HR_f", "select_usr", "Select user"), local->get("HR_f", "caption", "HR manager"), wxOK);
			return;
		}
		
		int ress = wxMessageBox(local->get("HR_f", "is_delpers_question", "Are you sure?"), local->get("HR_f", "caption", "HRManager"), wxYES_NO );
		if (ress != wxYES) return;
		SetCursor(*wxHOURGLASS_CURSOR);

		HRuser_t& load_usr = get_user();
		
		all_user->delete_user(load_usr.user_id());
		reload_pers();
		sort_persons();
		
		//sort_persons();
	}
	catch (exception_t &e)
	{
		if (e.code() == 4091 || e.code() == 6512 || e.code() == 2292)
			wxMessageBox(local->get("HR_f", "cant_del", "User has activity. You cannot delete this user."));
		else
		{
			wxMessageBox(to_uc(e.user_message()));
			spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
		}
	}
	catch (std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}

/*******************************************************************/
/*-----------------------------------------positions ----------------------------------------------*/
/*******************************************************************/
void HRFrame::OnClickPos(wxCommandEvent& event)
{
	try
	{
		pos = event.GetInt();
		for (int i=0; i<all_role->size(); i++)
		{
			if (positionrole->get(pos)->find_id(all_role->get(i)->get_id()) != -1) cl_role->Check(i, true);
			else cl_role->Check(i, false);
		}
	}
	catch(exception_t &e)
	{
		wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
}
/*******************************************************************/
void HRFrame::OnAddpos(wxCommandEvent& event)
{
	try
	{	
		if (!(users->user().administrator())) 
		{
			wxMessageBox(local->get("HR_f", "dont_admin", "You dont't have privileges"));
			return;
		}
		//add new position
		std::auto_ptr<HRposition_f> form (new HRposition_f(this, -1));

		if (form->Showdlg() != 1 ) return;
		
		SetCursor(*wxHOURGLASS_CURSOR);
		
		param_t* new_pos = new param_t(pos, form->get_res());
		for (int i=0; i<form->get_size(); i++)	new_pos->add(form->get_role(i));
		positionrole->add_position(new_pos);
		lb_position->Append(to_uc(positionrole->get(positionrole->size()-1)->get_name()));
		wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_LISTBOX_SELECTED, pos);  
		OnClickPos(evt);
		lb_position->Select(pos);
	}
	catch(exception_t &e)
	{
		if (e.code() == 20701) wxMessageBox(local->get("HR_f", "error_del_pers", "Persons with this position exist"));
		else wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.code() + " " + e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
void HRFrame::OnDelpos(wxCommandEvent& event)
{
	try //delete position
	{

		if (!(users->user().administrator())) 
		{
			wxMessageBox(local->get("HR_f", "dont_admin", "You dont't have privileges"));
			return;
		}
		
		int ress = wxMessageBox(local->get("HR_f", "is_del_question", "Are you sure?"), local->get("HR_f", "caption", "HRManager"), wxYES_NO );
		if (ress != wxYES) return;
		
		SetCursor(*wxHOURGLASS_CURSOR);
		
		positionrole->del_position(pos);
		lb_position->Delete(pos);
		wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_LISTBOX_SELECTED, 0);  
		OnClickPos(evt);
		lb_position->Select(0);
	}
	catch(exception_t &e)
	{
		if (e.code() == 20701) wxMessageBox(local->get("HR_f", "error_del_pers", "Persons with this position exist"));
		else wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.code() + " " + e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
void HRFrame::OnEditpos(wxCommandEvent& event)
{
	try //edit position
	{
		if (!(users->user().administrator())) 
		{
			wxMessageBox(local->get("HR_f", "dont_admin", "You dont't have privileges"));
			return;
		}
		std::auto_ptr<HRposition_f> form (new HRposition_f(this, pos));
		
		if (form->Showdlg() != 1 ) return;
		
		SetCursor(*wxHOURGLASS_CURSOR);
		
		param_t* ed_pos = new param_t(positionrole->get(pos)->get_id(), form->get_res());
		for (int i=0; i<form->get_size(); i++)	ed_pos->add(form->get_role(i));
		positionrole->edit_position(ed_pos);
		lb_position->SetString(pos, to_uc(positionrole->get(pos)->get_name()));
		wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_LISTBOX_SELECTED, pos);  
		OnClickPos(evt);
		lb_position->Select(pos);
	}
	catch(exception_t &e)
	{
		if (e.code() == 20701) wxMessageBox(local->get("HR_f", "error_del_pers", "Persons with this position exist"));
		else wxMessageBox(to_uc(e.user_message()));
		spooler->send_bag(bag_record_t(1, app_id, version::version, e.code() + " " + e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
		//spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
	}
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
/*******************************************************************/
void HRFrame::OnPropertyClick(wxMouseEvent& event)
{
	try
	{
		std::auto_ptr<HRproperty_f> prop_f (new HRproperty_f((wxWindow*) this));
		if (prop_f->Showdlg() != 1) return;
		load_lang();
		//if ((config->get("locale", "exist") == "1") && (config->get("locale", "name") != local->get_locale_name()) && (config->get("locale", "file") != ""))  
			//load_lang();
		//if (config->get("locale", "exist") != "1") load_lang();
	}
	catch (exception_t& ex) 
	{
		wxMessageBox(to_uc(ex.user_message()));
		//*flog << ex.comp_message();
	}
	
}

void HRFrame::OnPopupMenuClick( wxGridEvent& event )
{
	sel_row = event.GetRow();

	gr_pers_a->ClearSelection();
	gr_pers_a->SelectRow(event.GetRow());
	gr_pers_a->SetGridCursor(event.GetRow(), event.GetCol());

	gr_pers_f->ClearSelection();
	gr_pers_f->SelectRow(event.GetRow());
	gr_pers_f->SetGridCursor(event.GetRow(), event.GetCol());
	
	popup_menu->UpdateUI();
	PopupMenu(popup_menu);
}

/*******************************************************************/
/*  HRNotebook                                                     */
/*******************************************************************/

/*HRNotebook::HRNotebook(wxWindow *parent) //: base_HRNotebook(parent)
: wxNotebook( parent, wxID_ANY, wxDefaultPosition, wxSize(parent->GetSize().GetWidth()- wxFULL_REPAINT_ON_RESIZE)
{
  try
  {
	AddPage(CreatePersonPage(), local->get("HR_f", "person", "Personnel"), false, -1);
	AddPage(CreatePositionPage(), local->get("HR_f", "position", "Positions"), false, -1);
	
	std::string ver_str = version::version;
	st_bar = new wxStaticText(this, 1234, to_uc(version::copyright + ", " + version::product + " ver. " + ver_str), wxPoint(2, 385), wxDefaultSize/*, wxNO_BORDE
    st_bar->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( HRNotebook::OnPropertyClick ), NULL, this );
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
	exception_t e(5003, "unexpected error", "HRNotebook::HRNotebook()");
	wxMessageBox(to_uc(e.message()));
  }

}
/*******************************************************************/
/*HRNotebook::~HRNotebook()
{
	st_bar->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( HRNotebook::OnPropertyClick ), NULL, this );
}
/*******************************************************************/
/*void HRNotebook::reload_lang()
{
	tb_person->reload_lang(); //HRPersToolBar
	tb_position->reload_lang(); //HRPosToolBar
	SetPageText(0, local->get("HR_f", "person", "Personnel"));
	SetPageText(1, local->get("HR_f", "position", "Positions"));
}
	
	
/*******************************************************************/

/*******************************************************************/
/*wxWindow *HRNotebook::CreatePersonPage()
{
  try
  {
	wnd_pers = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxSize(GetSize()),
		wxFULL_REPAINT_ON_RESIZE |
		wxCLIP_CHILDREN |
		wxTAB_TRAVERSAL );

	//nb_person = new HRGridNotebook(wnd_pers);
	tb_person = new HRPersToolBar(wnd_pers);
	return wnd_pers;
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
	exception_t e(5006, "unexpected error", "*HRNotebook::CreatePersonPage()");
	wxMessageBox(to_uc(e.message()));
  }
}
/*******************************************************************/
/*wxWindow *HRNotebook::CreatePositionPage()
{
  try
  {
    wnd_pos = new wxWindow(this, wxID_ANY, wxDefaultPosition, wxSize(GetSize()) ,
		wxFULL_REPAINT_ON_RESIZE |
		wxCLIP_CHILDREN |
		wxTAB_TRAVERSAL );

	tb_position = new HRPosToolBar(wnd_pos);

	return wnd_pos; 
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
	exception_t e(5007, "unexpected error", "*HRNotebook::CreatePositionPage");
	wxMessageBox(to_uc(e.message()));
  }
}
/*******************************************************************/
/*void HRNotebook::OnResize(wxSizeEvent &event)
{
  try
  {
	wnd_pos->SetSize(event.GetSize().GetWidth()-8, event.GetSize().GetHeight()-45);
	wnd_pers->SetSize(event.GetSize().GetWidth()-8, event.GetSize().GetHeight()-45);//-50

	tb_person->SetSize(event.GetSize().GetWidth(), 50);
	tb_person->OnResize(event);
	tb_position->OnResize(event);
	if (event.GetSize().GetWidth() > 40) st_bar->Move(3, event.GetSize().GetHeight()-20);
	else st_bar->Move(3, 20);
	
	Update();
	
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
	exception_t e(5008, "unexpected error", "HRNotebook::OnResize");
	wxMessageBox(to_uc(e.message()));
  }
}





/*BEGIN_EVENT_TABLE( HRGridNotebook, wxGrid )
	//EVT_GRID_EDITOR_HIDDEN( HRFrame::OnEditorHidden )
	EVT_SIZE(HRGridNotebook::OnResize)
END_EVENT_TABLE()*/
/*******************************************************************/
/*BEGIN_EVENT_TABLE( HRNotebook, wxNotebook )
	EVT_SIZE(HRNotebook::OnResize)
	//EVT_BUTTON(1234, HRNotebook::OnApplyClick)
	//EVT_BUTTON(wxID_ANY, HRNotebook::OnApplyClick)
END_EVENT_TABLE()
/*******************************************************************/
