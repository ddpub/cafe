#include "HRgrid_f.h"
//#include <wx_util/wx_util.hpp>

#include "lang/xml_lang.h"
#include "HR_ico.h"
#include <wx_util/xml_config.hpp>
#include "person_f.h"
#include "mcard.h"
#include "dialog_f.h"

extern std::auto_ptr<xml_config_t> config;

extern std::auto_ptr<ksi_cafe::users_t> users;
extern std::auto_ptr<vecparam_t> all_cafe;
extern std::auto_ptr<vecparam_t> all_role;
extern std::auto_ptr<HRusers_t> all_user;
extern std::auto_ptr<ksi_cafe::xml_lang_t> local;

/*******************************************************************/
/*  HRGridNotebook                                                 */
/*******************************************************************/

HRGridNotebook::HRGridNotebook(wxWindow *parent)
: wxNotebook( parent, wxID_ANY, wxPoint( 0, 55), wxSize(parent->GetSize().GetWidth() + 35, parent->GetSize().GetHeight()/* - 40*/ ), wxNB_LEFT|wxTAB_TRAVERSAL)
{
  try
  {
	SetCursor(*wxHOURGLASS_CURSOR);
	sort_col = -1;
	actived_page = -1;
	is_busy = false;
	//wxForm* splash = new wxForm((wxFrame *)NULL, wxID_ANY, _T("I'm a book page"));
	//splash->Show();

	//all_user = new HRusers_t(0);
	for (int i=0; i<all_user->size(); i++)
		if (all_user->get_user(i)->fired()) user_f.push_back(all_user->get_user(i)->user_id());
		else user_a.push_back(all_user->get_user(i)->user_id());
	actived = true;
	AddPage(CreatePersonGrid(), local->get("HR_f", "active", "Active"), false, -1);
	actived = false;
	AddPage(CreatePersonGrid(), local->get("HR_f", "fired", "Fired"), false, -1);
	SetCursor(*wxSTANDARD_CURSOR);
	
	sort_bmp_a = new wxStaticBitmap( gr_pers_a, wxID_ANY, wxNullBitmap, wxPoint(5,5), wxDefaultSize, 0 );
	sort_bmp_f = new wxStaticBitmap( gr_pers_f, wxID_ANY, wxNullBitmap, wxPoint(5,5), wxDefaultSize, 0 );
	
	actived_page = 0;
	
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
	exception_t e(5009, "unexpected error", "HRGridNotebook::HRGridNotebook");
	wxMessageBox(to_uc(e.message()));
  }
  SetCursor(*wxSTANDARD_CURSOR);

}
/*******************************************************************/
void HRGridNotebook::reload_lang()
{
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
}
/*******************************************************************/
void HRGridNotebook::OnChangeActive(wxNotebookEvent& event)
{
  
  
  if (event.GetSelection() == 1) actived = false;
  else actived = true;
  try
  {
	wxGrid *gr_pers;
	if (actived) gr_pers = gr_pers_a; else gr_pers = gr_pers_f;
	gr_pers = new wxGrid(this, wxID_ANY, wxPoint(2,0), wxSize(GetSize().GetWidth(), GetSize().GetHeight()) );
	gr_pers->CreateGrid( 0, 0);
	gr_pers->SetSelectionMode(wxGrid::wxGridSelectRows);

	gr_pers->HideCellEditControl();

	gr_pers->AppendCols(5);
	gr_pers->SetRowLabelSize( 0 );
	gr_pers->SetColLabelSize( gr_pers->GetDefaultRowSize());

	gr_pers->EnableEditing(false);
	gr_pers->EnableDragRowSize(false);
	gr_pers->EnableDragColSize(true);
	gr_pers->EnableDragColMove(false);
	gr_pers->EnableGridLines(true);
	gr_pers->SetColLabelValue(0, local->get("HR_f", "login", " login "));
	gr_pers->SetColLabelValue(1, local->get("HR_f", "firstname", "firstname"));
	gr_pers->SetColLabelValue(2, local->get("HR_f", "lastname", "lastname"));
	gr_pers->SetColLabelValue(3, local->get("HR_f", "surname", "surname"));
	gr_pers->SetColLabelValue(4, local->get("HR_f", "birthday", "birthday"));

	if (actived) gr_pers_a = gr_pers; else gr_pers_f = gr_pers;

	load_persons();

	//return gr_pers/*(actived ? gr_pers_a : gr_pers_f)*///;
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
	exception_t e(5010, "unexpected error", "*HRGridNotebook::CreatePersonGrid()");
	wxMessageBox(to_uc(e.message()));
  }
	
	
	//((HRPersToolBar*) GetParent())->OnChangeActive(event.GetSelection());
	//actived_page = event.GetSelection();
	//event.Allow();
	////event.SetSelection(actived_page);
	//Update();
	//Show();
	//Show();
}
/*******************************************************************/
wxWindow *HRGridNotebook::CreatePersonGrid()
{
  try
  {
	wxGrid *gr_pers;
	if (actived) gr_pers = gr_pers_a; else gr_pers = gr_pers_f;
	gr_pers = new wxGrid(this, wxID_ANY, wxPoint(2,0), wxSize(GetSize().GetWidth(), GetSize().GetHeight()) );
	gr_pers->CreateGrid( 0, 0);
	gr_pers->SetSelectionMode(wxGrid::wxGridSelectRows);

	gr_pers->HideCellEditControl();

	gr_pers->AppendCols(5);
	gr_pers->SetRowLabelSize( 0 );
	gr_pers->SetColLabelSize( gr_pers->GetDefaultRowSize());

	gr_pers->EnableEditing(false);
	gr_pers->EnableDragRowSize(false);
	gr_pers->EnableDragColSize(true);
	gr_pers->EnableDragColMove(false);
	gr_pers->EnableGridLines(true);
	gr_pers->SetColLabelValue(0, local->get("HR_f", "login", " login "));
	gr_pers->SetColLabelValue(1, local->get("HR_f", "firstname", "firstname"));
	gr_pers->SetColLabelValue(2, local->get("HR_f", "lastname", "lastname"));
	gr_pers->SetColLabelValue(3, local->get("HR_f", "surname", "surname"));
	gr_pers->SetColLabelValue(4, local->get("HR_f", "birthday", "birthday"));

	if (actived) gr_pers_a = gr_pers; else gr_pers_f = gr_pers;

	load_persons();

	return gr_pers/*(actived ? gr_pers_a : gr_pers_f)*/;
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
	exception_t e(5010, "unexpected error", "*HRGridNotebook::CreatePersonGrid()");
	wxMessageBox(to_uc(e.message()));
  }
}
/*******************************************************************/
void HRGridNotebook::OnSelect(wxGridEvent& event)
{
  try
  {
	if (is_busy) return;
	sel_row = event.GetRow();

	gr_pers_a->ClearSelection();
	gr_pers_a->SelectRow(event.GetRow());
	gr_pers_a->SetGridCursor(event.GetRow(), event.GetCol());

	gr_pers_f->ClearSelection();
	gr_pers_f->SelectRow(event.GetRow());
	gr_pers_f->SetGridCursor(event.GetRow(), event.GetCol());
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
	exception_t e(5011, "unexpected error", "HRGridNotebook::OnSelect");
	wxMessageBox(to_uc(e.message()));
  }
}*/
/*******************************************************************/
void HRGridNotebook::Sort(int ssort = -1)
{
  try
  {
	//all_user.sort(event.GetCol, sort_order);
	sort_col = ssort;
	if (sort_col != 0 && sort_col != 1 && sort_col != 2 && sort_col != 3) return;
	if (sort_order) sort_order = false; else  sort_order = true;
	std::vector<std::string> new_user_a;
	std::vector<std::string> new_user_f;
	
	for (size_t i=0; i<user_a.size(); i++)
		new_user_a.push_back(to_mb(gr_pers_a->GetCellValue(i, sort_col)));

	for (size_t i=0; i<user_f.size(); i++)
		new_user_f.push_back(to_mb(gr_pers_f->GetCellValue(i, sort_col)));

	for (size_t i=0; i < user_a.size()-1; i++)
	{
		for (size_t j=i+1; j<user_a.size(); j++)
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

	for (size_t i=0; i<user_f.size()-1; i++)
	{
		for (size_t j=i+1; j<user_f.size(); j++)
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
	actived = true;   load_persons();
	actived = false;  load_persons();
	
	wxBitmap sort_map;
	if (sort_order) sort_map = HR_sort(1); else sort_map = HR_sort(0);
	
	wxPoint pos_a = gr_pers_a->CellToRect(1,sort_col+1).GetPosition();
	wxPoint pos_f = gr_pers_f->CellToRect(1,sort_col+1).GetPosition();
	
	sort_bmp_a->SetBitmap(sort_map);
	sort_bmp_f->SetBitmap(sort_map);
	
	sort_bmp_a->Move(pos_a.x-20, 5);
	sort_bmp_f->Move(pos_f.x-20, 5);
	
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
	exception_t e(5012, "unexpected error", "HRGridNotebook::OnSort");
	wxMessageBox(to_uc(e.message()));
  }
}
  */
/*******************************************************************/
void HRGridNotebook::OnSort(wxGridEvent& event)
{
	if (is_busy) return;
	Sort(event.GetCol());
}
/*******************************************************************/
void HRGridNotebook::Filter()
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
	Sort();
	set_actived(true); load_persons();
	set_actived(false); load_persons();
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
	exception_t e(5013, "unexpected error", "HRGridNotebook::Filter");
	wxMessageBox(to_uc(e.message()));
  }
}
/*******************************************************************/
void HRGridNotebook::OnResize(wxSizeEvent& event)
{
	try
	{
		//gr_pers_a->SetSize(event.GetSize());
		if (is_busy) return;
		int wight_new = event.GetSize().GetWidth()- 60;
		for (int i=0; i<5; i++)
		{
			gr_pers_a->SetColSize(i, wight_new/5);
			gr_pers_f->SetColSize(i, wight_new/5);
		}
		gr_pers_a->SetSize(event.GetSize().GetWidth()- 28, event.GetSize().GetHeight()-7 );//-40
		gr_pers_f->SetSize(event.GetSize().GetWidth()- 28, event.GetSize().GetHeight()-7 );//-28  -7	
		
		wxPoint pos_a = gr_pers_a->CellToRect(1, sort_col+1).GetPosition();
		wxPoint pos_f = gr_pers_f->CellToRect(1, sort_col+1).GetPosition();
	
		sort_bmp_a->Move(pos_a.x-20, 5);
		sort_bmp_f->Move(pos_f.x-20, 5);
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
		exception_t e(5014, "unexpected error", "HRGridNotebook::OnResize");
		wxMessageBox(to_uc(e.message()));
	}
}
/*******************************************************************/
HRuser_t *HRGridNotebook::get_usr()
{
	try
	{
		if (GetSelection() == 0) return all_user->find_user(user_a[sel_row]);
		else if (GetSelection() == 1) return all_user->find_user(user_f[sel_row]);
		return 0;
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
		exception_t e(5015, "unexpected error", "*HRGridNotebook::get_usr()");
		wxMessageBox(to_uc(e.message()));
	}
}
/*******************************************************************/
/*bool HRGridNotebook::IsSelected()
{
  try
  {
	if (GetSelection() == 0) { if (sel_row > int(user_a.size()) || sel_row < 0)  return false; else return true;}
	else if (GetSelection() == 1) { if (sel_row > int(user_f.size()) || sel_row < 0) return false; else return true; }
	return false;
  }
  catch(exception_t &e)
  {
	wxMessageBox(to_uc(e.message()));
  }
  catch(std::exception &e)
  {
	wxMessageBox(to_uc(e.what()));
  }
  /*catch(...)
  {
	exception_t e(5016, "unexpected error", "HRGridNotebook::IsSelected()");
	wxMessageBox(to_uc(e.message()));
  }*/
}
/*******************************************************************/
/*void HRGridNotebook::reload()
{
  try
  {
	user_a.clear(); user_f.clear();
	for (int i=0; i<all_user->size(); i++)
	{
		if (all_user->get_user(i)->fired()) user_f.push_back(all_user->get_user(i)->user_id());
		else user_a.push_back(all_user->get_user(i)->user_id());
	}
	Filter();
	//Sort();
	//set_actived(true); load_persons();
	//set_actived(false); load_persons();
	/*if (ssort != -1) 
	{
		wxGridEvent ee = wxGridEvent(wxID_ANY, wxEVT_GRID_LABEL_LEFT_CLICK, gr_pers_a, -1, ssort);
		OnSort(ee);
	}*/
/*  }
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
	exception_t e(5016, "unexpected error", "HRGridNotebook::IsSelected()");
	wxMessageBox(to_uc(e.message()));
  }
}

/*******************************************************************/
/*void HRGridNotebook::load_persons()
{
  try
  {	
	wxGrid *gr_pers;
	if (actived) gr_pers = gr_pers_a; else gr_pers = gr_pers_f;
	SetCursor(*wxHOURGLASS_CURSOR);
	//gr_pers->ClearGrid();
	if (gr_pers->GetNumberRows() > 0) gr_pers->DeleteRows(0, gr_pers->GetNumberRows());

	for (size_t i=0; i<(actived ? user_a.size(): user_f.size()); i++)
	{
		//if (f_cafe_on && f_role_on)

		gr_pers->AppendRows(1);
		HRuser_t *usr = all_user->find_user(actived ? user_a[i] :user_f[i]);
		std::string nnname = usr->get_firstname();
		gr_pers->SetCellValue(i, 0, to_uc(usr->get_login()));
		gr_pers->SetCellValue(i, 1, to_uc(usr->get_firstname()));
		gr_pers->SetCellValue(i, 2, to_uc(usr->get_lastname()));
		gr_pers->SetCellValue(i, 3, to_uc(usr->get_surname()));
		gr_pers->SetCellValue(i, 4, (usr->get_birthdate() == boost::date_time::not_a_date_time ? _T("") : local->get_date(usr->get_birthdate())));
	}
	if (actived) gr_pers_a = gr_pers; else gr_pers_f = gr_pers;

	SetCursor(*wxSTANDARD_CURSOR);
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
	wxMessageBox(_T("jjj"));
  }
  SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
/*void HRGridNotebook::reload_persons()
{
  try
  {	
	Filter();
	set_actived(true); load_persons();
	set_actived(false); load_persons();
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
	wxMessageBox(_T("jjj"));
  }
  this->SetCursor(*wxSTANDARD_CURSOR);
}*/

/*******************************************************************/
//void HRGridNotebook::OnModify(int id)
//{

/*void HRGridNotebook::OnAdd()
{
	try
	{  
		is_busy = true;
		SetCursor(*wxHOURGLASS_CURSOR);
		HRuser_t load_usr = HRuser_t(0);

		std::auto_ptr<HRPerson_f> p_frame (new HRPerson_f(GetParent(), load_usr));
		SetCursor(*wxSTANDARD_CURSOR);

		if (p_frame->Showdlg() != 1) return;
		SetCursor(*wxHOURGLASS_CURSOR);
		all_user->insert_user(load_usr, p_frame->get_role(), p_frame->get_cafe(), p_frame->get_password());			
		if (p_frame->Is_cardW())
		{
			SetCursor(*wxSTANDARD_CURSOR);
			WriteCard(p_frame->get_user()->get_login(), p_frame->get_password(), p_frame->get_is_low());
			SetCursor(*wxHOURGLASS_CURSOR);
		}
		reload();

	}
	catch(exception_t &e)
	{
		wxMessageBox(to_uc(boost::lexical_cast<std::string>(e.code()) + "  " + e.message()));
	}
	catch(std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
	}
	catch(...)
	{
		exception_t e(5018, "unexpected error", "HRPersToolBar::OnClickTool");
		wxMessageBox(to_uc(e.message()));
	}
	is_busy = false;
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
/*void HRGridNotebook::OnEdit()
	//else if (id == 2 || id == 3 || id == 4 || id == 5 || id == 6)
{
	try 
	{
		SetCursor(*wxHOURGLASS_CURSOR);
		is_busy = true;
		if (!IsSelected())
		{
			wxMessageBox(_T("Select user "), _T("HR manager"), wxOK);
			SetCursor(*wxSTANDARD_CURSOR);
			return;
		}

		HRuser_t& load_usr = *(get_usr());
	//Edit
		load_usr.reload();

		std::auto_ptr<HRPerson_f> p_frame(new HRPerson_f(GetParent(), load_usr));
		SetCursor(*wxSTANDARD_CURSOR);

		if (p_frame->Showdlg() != 1) return;

		SetCursor(*wxHOURGLASS_CURSOR);
			
		all_user->update_user(load_usr, load_usr.user_id(), p_frame->get_role(), p_frame->get_cafe(), p_frame->get_password()); 
		if (p_frame->Is_cardW())
		{
			SetCursor(*wxSTANDARD_CURSOR);
			WriteCard(p_frame->get_user()->get_login(), p_frame->get_password(), p_frame->get_is_low());
			SetCursor(*wxHOURGLASS_CURSOR);
		}
		set_actived(true); load_persons();
		set_actived(false); load_persons();
	}
	catch (exception_t &e)
	{
		wxMessageBox(to_uc(boost::lexical_cast<std::string>(e.code()) + "  " + e.message()));
	}
	catch (std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
	}
	is_busy = false;
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/	
/*void HRGridNotebook::OnFire()
//Fire
{
	try 
	{
		SetCursor(*wxHOURGLASS_CURSOR);
		is_busy = true;
		if (!IsSelected())
		{
			wxMessageBox(_T("Select user "), _T("HR manager"), wxOK);
			SetCursor(*wxSTANDARD_CURSOR);
			return;
		}

		HRuser_t& load_usr = *(get_usr());
		
		all_user->fire_user(load_usr);
		reload();
					//Filter();
	}
	catch (exception_t &e)
	{
		wxMessageBox(to_uc(boost::lexical_cast<std::string>(e.code()) + "  " + e.message()));
	}
	catch (std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
	}
	is_busy = false;
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
/*void HRGridNotebook::OnRehire()
//Rehire
{
	try 
	{
		SetCursor(*wxHOURGLASS_CURSOR);
		is_busy = true;
		if (!IsSelected())
		{
			wxMessageBox(_T("Select user "), _T("HR manager"), wxOK);
			SetCursor(*wxSTANDARD_CURSOR);
			return;
		}

		HRuser_t& load_usr = *(get_usr());

		all_user->rehire_user(load_usr);
		reload();
	}
	catch (exception_t &e)
	{
		wxMessageBox(to_uc(boost::lexical_cast<std::string>(e.code()) + "  " + e.message()));
	}
	catch (std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
	}
	is_busy = false;
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
v/*oid HRGridNotebook::OnUnlock()
//Unlock
{
	try 
	{
		SetCursor(*wxHOURGLASS_CURSOR);
		is_busy = true;
		if (!IsSelected())
		{
			wxMessageBox(_T("Select user "), _T("HR manager"), wxOK);
			SetCursor(*wxSTANDARD_CURSOR);
			return;
		}

		HRuser_t& load_usr = *(get_usr());
		all_user->unlock_user(load_usr);
	}
	catch (exception_t &e)
	{
		wxMessageBox(to_uc(boost::lexical_cast<std::string>(e.code()) + "  " + e.message()));
	}
	catch (std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
	}
	is_busy = false;
	SetCursor(*wxSTANDARD_CURSOR);
}
/*******************************************************************/
/*void HRGridNotebook::OnDelete()
//Delete
{
	try 
	{
		SetCursor(*wxHOURGLASS_CURSOR);
		is_busy = true;
		if (!IsSelected())
		{
			wxMessageBox(_T("Select user "), _T("HR manager"), wxOK);
			SetCursor(*wxSTANDARD_CURSOR);
			return;
		}

		HRuser_t& load_usr = *(get_usr());
		all_user->delete_user(load_usr.user_id());
		reload();
	}
	catch (exception_t &e)
	{
		wxMessageBox(to_uc(boost::lexical_cast<std::string>(e.code()) + "  " + e.message()));
	}
	catch (std::exception &e)
	{
		wxMessageBox(to_uc(e.what()));
	}
	is_busy = false;
	SetCursor(*wxSTANDARD_CURSOR);
}

/*******************************************************************/
/*void HRGridNotebook::OnDblClick(wxGridEvent& event)
{

	try
	{
		if (is_busy) return;
		sel_row = event.GetRow();
	
		gr_pers_a->ClearSelection();
		gr_pers_a->SelectRow(event.GetRow());
		gr_pers_a->SetGridCursor(event.GetRow(), event.GetCol());

		gr_pers_f->ClearSelection();
		gr_pers_f->SelectRow(event.GetRow());
		gr_pers_f->SetGridCursor(event.GetRow(), event.GetCol());
		
		OnEdit();
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
		exception_t e(5011, "unexpected error", "HRGridNotebook::OnSelect");
		wxMessageBox(to_uc(e.message()));
	}
}


/*******************************************************************/
/*  HRPersToolBar                                                 					      */
/*******************************************************************/
HRPersToolBar::HRPersToolBar(wxWindow *parent)
: wxToolBar(parent, wxID_ANY, wxPoint(20,20)/*DefaultPosition*/,
	wxSize(/*parent->GetSize().GetWidth()*/700, 60), wxBORDER_NONE|wxTB_HORIZONTAL|wxTB_NODIVIDER|wxTB_FLAT|wxTB_TEXT|wxTAB_TRAVERSAL )
{
  try
  {
	SetToolBitmapSize(wxSize(30, 30));
	AddTool(1, local->get("HR_f", "btn_add", "Hire"), HR_tool(4));
	AddTool(2, local->get("HR_f", "btn_edit", "Edit"), HR_tool(1)); 
	AddTool(3, local->get("HR_f", "btn_fire", "Fire"), HR_tool(5));
	AddTool(4, local->get("HR_f", "btn_rehire", "Rehire"), HR_tool(4));
	EnableTool(4, false);
	AddTool(5, local->get("HR_f", "btn_unlock", "Unlock"), HR_tool(3));
	AddTool(6, local->get("HR_f", "btn_del", "Delete"), HR_tool(0));
	
	AddSeparator();
	
	ppp = new wxPanel(this, wxID_ANY, /*_T(""),*/ wxDefaultPosition, wxSize(110, 50));
	ppp1 = new wxPanel(this, wxID_ANY, /*_T(""),*/ wxDefaultPosition, wxSize(100, 50));
	
	ck_cafefilter = new wxCheckBox(ppp, wxID_ANY, local->get("HR_f", "cafe_f", "Cafe filter"), wxPoint(0,5));
	ck_rolefilter = new wxCheckBox(ppp, wxID_ANY, local->get("HR_f", "role_f", "Role filter"), wxPoint(0,30));
	
	AddControl((wxControl*) ppp);
	
	ed_cafefilter = new wxTextCtrl(ppp1, wxID_ANY, _T(""), wxPoint(0,2), wxSize(150, 20), wxTE_READONLY );
	ed_rolefilter = new wxTextCtrl(ppp1, wxID_ANY, _T(""), wxPoint(0,27), wxSize(150, 20), wxTE_READONLY );

	btn_cafe = new wxButton(ed_cafefilter, wxID_ANY, _T("..."), wxPoint(130,0), wxSize(16,17));
	btn_role = new wxButton(ed_rolefilter, wxID_ANY, _T("..."), wxPoint(130,0), wxSize(16,17));
	

	AddControl((wxControl*) ppp1);
	Realize();

	
	if (!all_cafe->loaded()) all_cafe->load_cafe();
	if (!all_role->loaded()) all_role->load_role();

//	nb_person = new HRGridNotebook(parent);
	nb_person->Connect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler(HRPersToolBar::OnChangeActive), NULL, this);
	
	//nb_person->Connect();
	//EVT_NOTEBOOK_PAGE_CHANGING(wxID_ANY, HRGridNotebook::OnChangeActive)
	//cb_locale->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxMouseEventHandler( HRproperty_f::OnComChanged ), NULL, this );
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
	exception_t e(5017, "unexpected error", "HRPersToolBar::HRPersToolBar");
	wxMessageBox(to_uc(e.message()));
  }
}


HRPersToolBar::~HRPersToolBar()
{
	nb_person->Disconnect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler(HRPersToolBar::OnChangeActive), NULL, this);
}

void HRPersToolBar::reload_lang()
{
	ClearTools();
	AddTool(1, local->get("HR_f", "btn_add", "Hire"), HR_tool(3));
	AddTool(2, local->get("HR_f", "btn_edit", "Edit"), HR_tool(1)); 
	AddTool(3, local->get("HR_f", "btn_fire", "Fire"), HR_tool(0));
	AddTool(4, local->get("HR_f", "btn_rehire", "Rehire"), HR_tool(1));
	EnableTool(4, false);
	AddTool(5, local->get("HR_f", "btn_unlock", "Unlock"), HR_tool(2));
	AddTool(6, local->get("HR_f", "btn_del", "Delete"), HR_tool(0));
	
	AddSeparator();
	
	ck_cafefilter->SetLabel(local->get("HR_f", "cafe_f", "Cafe filter"));
	ck_rolefilter->SetLabel(local->get("HR_f", "role_f", "Role filter"));
	
	AddControl((wxControl*) ppp);
	AddControl((wxControl*) ppp1);

	Realize();
	
	nb_person->reload_lang();
}
/*******************************************************************/
void HRPersToolBar::OnChangeActive(wxNotebookEvent& event)
{
	try
	{
		if (GetToolsCount() < 5) return;
		
		if (event.GetSelection() == 0) 
		{
			EnableTool(4, false);
			EnableTool(1, true);
			EnableTool(3, true);
			EnableTool(5, true);
			nb_person->ChangeSelection(event.GetSelection());
		}
		else if (event.GetSelection() == 1) 
		{
			EnableTool(4, true);
			EnableTool(1, false);
			EnableTool(3, false);
			EnableTool(5, false);
			nb_person->ChangeSelection(event.GetSelection());
		}
		Update();
	}
	catch(...) {  }
}
/*******************************************************************/
void HRPersToolBar::OnClickTool(wxCommandEvent& event)
{
	if (event.GetId() == 1) nb_person->OnAdd();
	else if (event.GetId() == 2) nb_person->OnEdit();
	else if (event.GetId() == 3) nb_person->OnFire();
	else if (event.GetId() == 4) nb_person->OnRehire();
	else if (event.GetId() == 5) nb_person->OnUnlock();
	else if (event.GetId() == 6) nb_person->OnDelete();
	//nb_person->OnModify(event.GetId());
}
/*******************************************************************/
/*void HRPersToolBar::OnTextFilter(wxCommandEvent& event)
{
  try
  {
	if (event.GetId() == 11)
	{

		HRSelectFrame *dlg = new HRSelectFrame(this, all_cafe.get(), local->get("filter_f", "caption_cafe", "cafe filter"), nb_person->get_fcafe());
		if (dlg->Showdlg() != 1) 
		{
			if (nb_person->size_fcafe() == 0) ck_cafefilter->SetValue(false);	
			ed_cafefilter->SetValue(_T(""));
			return;
		}
		
		nb_person->clear_fcafe();
		
		if (dlg->get_res_size() == 0) 
		{
			ck_cafefilter->SetValue(false);
			ed_cafefilter->SetValue(_T(""));
			return;
		}
		
		ck_cafefilter->SetValue(true);
		
		for (int i=0; i< dlg->get_res_size(); i++)
			nb_person->add_fcafe(dlg->get_res(i));

		ed_cafefilter->SetValue(to_uc(dlg->res_str()));

		delete dlg;
		nb_person->Filter();
		//nb_person->set_actived(true); nb_person->load_persons();
		//nb_person->set_actived(false); nb_person->load_persons();
		//nb_person->Filter(f_role, f_cafe);
	}
	else if (event.GetId() == 12)
	{

		HRSelectFrame *dlg = new HRSelectFrame(this, all_role.get(), local->get("filter_f", "caption_role", "role filter"), nb_person->get_frole());
		if (dlg->Showdlg() != 1) 
		{
			if (nb_person->size_frole() == 0) ck_rolefilter->SetValue(false);	
			ed_rolefilter->SetValue(_T(""));			
			return;
		}
		
		nb_person->clear_frole();
		
		if (dlg->get_res_size() == 0) 
		{
			ck_rolefilter->SetValue(false);
			ed_rolefilter->SetValue(_T(""));
			return;
		}
		
		ck_rolefilter->SetValue(true);
		for (int i=0; i< dlg->get_res_size(); i++)
			nb_person->add_frole(dlg->get_res(i));

		ed_rolefilter->SetValue(to_uc(dlg->res_str()));

		delete dlg;
		nb_person->Filter();
		//nb_person->set_actived(true); nb_person->load_persons();
		//nb_person->set_actived(false); nb_person->load_persons();
		//this->Update();
		//nb_person->Filter(f_role, f_cafe);
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
	exception_t e(5019, "unexpected error", "HRPersToolBar::OnTextFilter");
	wxMessageBox(to_uc(e.message()));
  }
}
/*******************************************************************/
/*void HRPersToolBar::OnCheckFilter(wxCommandEvent& event)
{
  try
  {
	if (event.GetId() == 7)
	{
		if (ck_cafefilter->IsChecked())
		{
			  //ck_cafefilter->SetValue(true);
			wxCommandEvent ee = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, 11);
			OnTextFilter(ee);
		}
		else
		{
			//ck_cafefilter->SetValue(false);
			nb_person->clear_fcafe();
			ed_cafefilter->SetValue(_T(""));
			nb_person->Filter();
			//nb_person->set_actived(true); nb_person->load_persons();
			//nb_person->set_actived(false); nb_person->load_persons();
			//nb_person->load_persons();
		}

	}
	else if (event.GetId() == 8)
	{
		if (ck_rolefilter->IsChecked())
		{
			  //ck_cafefilter->SetValue(true);
			wxCommandEvent ee = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED , 12);
			OnTextFilter(ee);
		}
		else
		{
			//ck_cafefilter->SetValue(false);
			nb_person->clear_frole();
			ed_rolefilter->SetValue(_T(""));
			nb_person->Filter();
			//nb_person->set_actived(true); nb_person->load_persons();
			//nb_person->set_actived(false); nb_person->load_persons();
			//nb_person->load_persons();
		}
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
	exception_t e(5020, "unexpected error", "HRPersToolBar::OnCheckFilter");
	wxMessageBox(to_uc(e.message()));
  }
}
/*******************************************************************/
/*void HRPersToolBar::OnResize(wxSizeEvent& event)
{
   try
  {
	ppp->SetSize(wxSize(110, 50));
	ppp1->SetSize(wxSize(150, 50));
	nb_person->SetSize(event.GetSize().GetWidth()-10, event.GetSize().GetHeight()-100); //-113
	Update();
	//nb_person->OnResize(event);
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
	exception_t e(5021, "unexpected error", "HRPersToolBar::OnResize");
	wxMessageBox(to_uc(e.message()));
  }
}


/*******************************************************************/
/*BEGIN_EVENT_TABLE( HRGridNotebook, wxNotebook )
	//EVT_GRID_EDITOR_HIDDEN( HRFrame::OnEditorHidden )
	//EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, HRGridNotebook::OnChangeActive)
	EVT_SIZE(HRGridNotebook::OnResize)
	EVT_GRID_CMD_CELL_LEFT_CLICK(wxID_ANY, HRGridNotebook::OnSelect)
	EVT_GRID_CELL_LEFT_DCLICK(HRGridNotebook::OnDblClick)
	EVT_GRID_LABEL_LEFT_CLICK(HRGridNotebook::OnSort)
	//EVT_TOOL(wxID_ANY, HRGridNotebook::OnClickTool)
END_EVENT_TABLE()
/*******************************************************************/
/*BEGIN_EVENT_TABLE( HRPersToolBar, wxToolBar )
	EVT_TOOL(wxID_ANY, HRPersToolBar::OnClickTool)
	EVT_SIZE(HRPersToolBar::OnResize)
	EVT_CHECKBOX(wxID_ANY, HRPersToolBar::OnCheckFilter)
	EVT_BUTTON(wxID_ANY, HRPersToolBar::OnTextFilter)
	EVT_BUTTON(wxID_ANY, HRPersToolBar::OnTextFilter)
END_EVENT_TABLE()
/*******************************************************************/

