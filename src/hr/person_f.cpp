
#include <wx/stream.h>
//#include <wx/sstream.h>
#include <wx/file.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/mstream.h>
#include "person_f.h"
#include <wx/filedlg.h>
#include <wx_util/wx_util.hpp>
#include <wx/txtstrm.h>
//#include <wx/event.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include "boost/date_time/special_defs.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "lang/xml_lang.h"
#include <ksi_util/bag_spooler.hpp>

extern std::auto_ptr<vecparam_t> position;
extern std::auto_ptr<vecparam_t> positionrole;
extern std::auto_ptr<vecparam_t> all_cafe;
extern std::auto_ptr<vecparam_t> all_role;
extern std::auto_ptr<vecparam_t> state;
extern std::auto_ptr<bag_spooler_t> spooler;

extern int app_id;

namespace version {
extern std::string version;
};

extern std::auto_ptr<ksi_cafe::xml_lang_t> local;

std::time_t to_time_tt(boost::posix_time::ptime t) 
{
	if( t == boost::date_time::neg_infin ) return 0;
	else if( t == boost::date_time::pos_infin ) return LONG_MAX; 
	boost::posix_time::ptime start(boost::gregorian::date(1970,1,1)); 
	return ((t-start).total_seconds()-86400); 
}

HRPerson_f::HRPerson_f(wxWindow *parent, HRuser_t& usr) 
: base_person_f( parent, wxID_ANY, local->get("person_f", "caption", "Person's info"), wxDefaultPosition, wxSize(660,570)/*, wxTAB_TRAVERSAL*/), uuser(usr)  
{
	is_edit = false;
	editt = false;
	card_w = false;
	exist = false;
	
	lb_login->SetLabel(local->get("person_f","login","Login"));
	lb_firstname->SetLabel(local->get("person_f","firstname","Firstname"));
	lb_lastname->SetLabel(local->get("person_f","lastname","Lastname"));
	lb_surname->SetLabel(local->get("person_f","surname","Surname"));
		
	lb_address1->SetLabel(local->get("person_f", "address1", "Address1"));
	lb_address2->SetLabel(local->get("person_f","address2", "Address2"));
	lb_private_mail->SetLabel(local->get("person_f","mail_private","E-mail private"));
	lb_business_mail->SetLabel(local->get("person_f","mail_business","E-mail business"));
	lb_phone_cell->SetLabel(local->get("person_f","phone_cell","Phone cell"));
	lb_phone_home->SetLabel(local->get("person_f","phone_home","Phone home"));
	lb_phone_business->SetLabel(local->get("person_f","phone_business","Phone business"));
	lb_city->SetLabel(local->get("person_f","city","City"));
	lb_ename->SetLabel(local->get("person_f", "ename","Emergency name"));
	lb_ephone->SetLabel(local->get("person_f", "ephone", "Emergency phone"));
	lb_descr->SetLabel(local->get("person_f","descr","Description"));
	lb_zip->SetLabel(local->get("person_f","zip","Zip"));
	
	lb_position->SetLabel(local->get("person_f","position","Position"));
	lb_state->SetLabel(local->get("person_f","state","State"));
	lb_birthday->SetLabel(local->get("person_f","birthday","Birthday"));
	lb_hire->SetLabel(local->get("person_f","hire","hire"));
	lb_ssn->SetLabel(local->get("person_f","ssn","SSN#"));
	
	lb_cafe->SetLabel(local->get("person_f","pers_cafe","Person's cafe"));
	lb_role->SetLabel(local->get("person_f","pers_role","Person's role"));
	
	lb_cafe->SetLabel(local->get("person_f","pers_cafe","Person's cafe"));
	lb_role->SetLabel(local->get("person_f","pers_role","Person's role"));
	
	lb_pwd->SetLabel(local->get("person_f","password","Password"));
	lb_confpwd->SetLabel(local->get("person_f","conf_password","Confirm password"));
	
	lb_photo->SetLabel(local->get("person_f","photo","Photo"));

	if (!positionrole->loaded()) positionrole->load_positionrole();
	for ( int i=0; i< positionrole->size(); i++) bx_position->Append(to_uc(positionrole->get_name(i)));
	
	if (!state->loaded()) state->load_state();
	for ( int i=0; i< state->size(); i++) bx_state->Append(to_uc(state->get_name(i))); //sstate[i] = to_uc(state->get_name(i));
	
	is_low = false;
	rb_hi->Disable();  rb_low->Disable();
	
	if (!all_cafe->loaded()) all_cafe->load_cafe();
	for ( int i=0; i< all_cafe->size(); i++) ck_cafe->Append(to_uc(all_cafe->get_name(i))); 
	
	if (!all_role->loaded()) all_role->load_role();
	for ( int i=0; i< all_role->size(); i++) ck_role->Append(to_uc(all_role->get_name(i))); 
	is_clear = false;
	
	t_photo->SetBitmap(wxBitmap(t_photo->GetSize().GetWidth(), t_photo->GetSize().GetHeight()));
	
	popbmp_menu =  new wxMenu();
	popbmp_menu->Append(4001, local->get("HR_f", "bmp_load", "Load image"));
	popbmp_menu->Append(4002, local->get("HR_f", "bmp_clear", "Clear image"));
	
	if (usr.user_id() != 0)
	{
		is_edit = true;
		t_login->SetValue(to_uc(usr.get_login()));
		t_firstname->SetValue(to_uc(usr.get_firstname()));
		t_lastname->SetValue(to_uc(usr.get_lastname()));
		t_surname->SetValue(to_uc(usr.get_surname()));
		
		t_address1->SetValue(to_uc(usr.get_address1()));
		t_address2->SetValue(to_uc(usr.get_address2()));
		t_mail_private->SetValue(to_uc(usr.get_emailprivate()));
		t_mail_business->SetValue(to_uc(usr.get_emailbusiness()));
		t_phone_cell->SetValue(to_uc(usr.get_phonecell()));
		t_phone_home->SetValue(to_uc(usr.get_phonehome()));
		t_phone_business->SetValue(to_uc(usr.get_phonebusiness()));
		t_city->SetValue(to_uc(usr.get_city()));
		t_ename->SetValue(to_uc(usr.get_ename()));
		t_ephone->SetValue(to_uc(usr.get_ephone()));
		t_descr->SetValue(to_uc(usr.get_description()));
		t_zip->SetValue(to_uc(boost::lexical_cast<std::string>(usr.get_zip())));

		try
		{
			if (usr.get_position() > 0) bx_position->SetValue(to_uc(positionrole->find(usr.get_position())->get_name()));
			if (usr.get_state() > 0) bx_state->SetValue(to_uc(state->find(usr.get_state())->get_name()));
		}
		catch(std::exception &e) 
		{
			std::string sss = e.what();
			int c = 0;
		}
		for (int i=0; i< all_cafe->size(); i++) if (usr.find_cafe(all_cafe->get(i)->get_id()) != -1) ck_cafe->Check(i, true); else ck_cafe->Check(i, false);
		for (int i=0; i< all_role->size(); i++) if (usr.find_role(all_role->get(i)->get_id()) != -1) ck_role->Check(i, true); else ck_role->Check(i, false);
        
		try
		{
			if (usr.get_birthdate() != boost::date_time::not_a_date_time) dt_birthday->SetValue(to_time_tt(usr.get_birthdate()));
			if (usr.get_hire() != boost::date_time::not_a_date_time) dt_hire->SetValue(to_time_tt(usr.get_hire()));
		}
		catch(std::exception &e) 
		{
			std::string sss = e.what();
			int c = 0;
		}
		
		try
		{
			std::vector<char> sss = usr.get_bmp(); 
    
			unsigned char sss_[sss.size()+1];
			
			for (size_t k=0; k<sss.size(); k++) //stream_out << (unsigned char) sss[k];
				sss_[k] = sss[k];

			
			WORD code = ((unsigned char)sss[1])*256 + ((unsigned char)sss_[0]);
			if (code == 0x4D42)
			{
				wxMemoryInputStream stream(sss_, (sss.size()+1));
				wxImage img(stream, wxBITMAP_TYPE_BMP);
				if (img.Ok()) t_photo->SetBitmap(wxBitmap(img));
			}
			else if (code == 0xD8FF)
			{
				wxMemoryInputStream stream(sss_, (sss.size()+1));
				wxImage img(stream, wxBITMAP_TYPE_JPEG);
				if (img.Ok()) t_photo->SetBitmap(wxBitmap(img));
			}
		}
		catch(...) {   }
	}
}
/*******************************************************************/
void HRPerson_f::OnClose(wxCommandEvent& event)
{ 

	for( int i=0; i<all_role->size(); i++) if (ck_role->IsChecked(i)) new_role.push_back(all_role->get(i)->get_id());
	for( int i=0; i<all_cafe->size(); i++) if (ck_cafe->IsChecked(i)) new_cafe.push_back(all_cafe->get(i)->get_id());

	if (t_login->GetValue() == _T("")) 
	{
		wxMessageBox(local->get("person_f", "null_login", "Login must be defined."), local->get("person_f", "caption", "HR manager"), wxOK);
		return;
	}
	if (to_mb(t_login->GetValue()) != uuser.get_login() && t_password->GetValue() == _T("")) 
	{
		wxMessageBox(local->get("person_f", "null_pwd", "Password must be defined."), local->get("person_f", "caption", "HR manager"), wxOK);
		return;
	}
	if ( (t_magncard->IsChecked() || uuser.user_id() == 0 || (uuser.get_login() != to_mb(t_login->GetValue()))) && (t_password->GetValue() == _T("") || t_confpassword->GetValue() == _T(""))) 
	{
		wxMessageBox(local->get("person_f", "null_pwd", "Password must be defined."), local->get("person_f", "caption", "HR manager"), wxOK);
		return;
	}
	
	if ( bx_position->GetValue() == _T("") ) 
	{
		wxMessageBox(local->get("person_f", "null_pos", "Position must be defined."), local->get("person_f", "caption", "HR manager"), wxOK);
		return;
	}
	if (t_password->GetValue() != t_confpassword->GetValue())
	{
		wxMessageBox(local->get("person_f", "err_pwd", "Uncorrect password"), local->get("person_f", "caption", "HR manager"), wxOK);
		return;
	}
	if ( !dt_hire->GetValue().IsValid() ) 
	{
		wxMessageBox(local->get("person_f", "null_hire", "Hire must be defined."), local->get("person_f", "caption", "HR manager"), wxOK);
		return;
	}
		{
		int zzip = 0;
		try
		{
			zzip = boost::lexical_cast<int>(to_mb(t_zip->GetValue()));
		}
		catch(...) {   }

		try
		{
			try
			{
				if (is_clear) uuser.clear_bmp();
				else
				{
					wxImage img = t_photo->GetBitmap().ConvertToImage();
				
					wxMemoryOutputStream stream;
					img.SaveFile(stream, wxBITMAP_TYPE_JPEG);
					char buffer[10000];
					int ccc = stream.CopyTo(buffer, 10000);
					std::vector<char> img_char(0);
					for (int i=0; i<ccc; i++) img_char.push_back(buffer[i]);
					uuser.save_bmp(img_char);
				}
			}
			catch(...) {  }
			
			if (t_magncard->IsChecked()) if (rb_low->GetValue()) is_low = true;
			std::string llogin = to_mb(t_login->GetValue());
			boost::posix_time::ptime bbirth = (dt_birthday->GetValue().IsValid() ? boost::posix_time::from_time_t(dt_birthday->GetValue().GetTicks()+86400) : boost::posix_time::ptime());
			for (int i = 0; i < llogin.length(); i++) llogin[i] = std::toupper(llogin[i], std::locale());
			uuser.upd(HRuser_t(uuser.user_id(), llogin, to_mb(t_firstname->GetValue()), to_mb(t_lastname->GetValue()),
        	to_mb(t_surname->GetValue()), /*boost::posix_time::from_time_t(dt_birthdate->GetValue().GetTicks())*/bbirth, 
			boost::posix_time::from_time_t(dt_hire->GetValue().GetTicks()+86400), uuser.get_fire(), to_mb(t_ssn->GetValue()), 
			to_mb(t_address1->GetValue()), to_mb(t_address2->GetValue()), zzip, 
			to_mb(t_city->GetValue()), state->find_id(to_mb(bx_state->GetValue())), to_mb(t_mail_business->GetValue()), to_mb(t_mail_private->GetValue()), 
			to_mb(t_phone_business->GetValue()), to_mb(t_phone_home->GetValue()), to_mb(t_phone_cell->GetValue()), 
			to_mb(t_ename->GetValue()), to_mb(t_ephone->GetValue()), positionrole->find_id(to_mb(bx_position->GetValue())), uuser.get_image_id(), to_mb(t_descr->GetValue())));
		}
		catch(exception_t &e)
		{
			wxMessageBox(to_uc(e.message()), local->get("person_f", "caption", "HR manager"), wxOK);
			spooler->send_bag(bag_record_t(1, app_id, version::version, e.code() + " " + e.message()));
		}
		catch(std::exception &e)
		{
			wxMessageBox(to_uc(e.what()), local->get("person_f", "caption", "HR manager"), wxOK);
			spooler->send_bag(bag_record_t(1, app_id, version::version, e.what()));
		}
		catch(...)
		{
			exception_t e(2001, "unexpected error", "HRPerson_f::OnClose - upd");
			wxMessageBox(to_uc(e.message()), local->get("person_f", "caption", "HR manager"), wxOK);
			spooler->send_bag(bag_record_t(1, app_id, version::version, e.code() + " " + e.message()));
		}
		 
		if (t_magncard->IsChecked()) card_w = true;			
		new_password = to_mb(t_password->GetValue());
	} 
	EndModal(1);
}
/*******************************************************************/
void HRPerson_f::OnCancel(wxCommandEvent& event)
{
	EndModal(2);
}

/*******************************************************************/
void HRPerson_f::OnCheckPos(wxCommandEvent& event)
{ 
	if (bx_position->GetValue() != _T(""))	
	{
		param_t* cur_pos = positionrole->find(positionrole->find_id(to_mb(bx_position->GetValue())));
		for (int i=0; i<all_role->size(); i++) 
			if (cur_pos->find(all_role->get(i)->get_id())) 
				ck_role->Check(i, true); 
			else ck_role->Check(i, false); 
	}
}
/*******************************************************************/
void HRPerson_f::OnLoadBmp(wxMouseEvent& event)
{
	wxFileDialog *dlg = new wxFileDialog(this);
	dlg->SetWildcard(_T("BMP files (*.bmp)|*.bmp|JPEG files (*.jpg)|*.jpg"));
	if (dlg->ShowModal() != wxID_OK) return;
	//ed_lang->SetValue(dlg->GetPath());
	if (dlg->GetFilterIndex() == 0)
		t_photo->SetBitmap(wxBitmap(dlg->GetPath(), wxBITMAP_TYPE_BMP));
	else if (dlg->GetFilterIndex() == 1)
		t_photo->SetBitmap(wxBitmap(dlg->GetPath(), wxBITMAP_TYPE_JPEG));
	
	t_photo->SetSize(wxSize(220, 220));
	is_clear = false;
}
/*******************************************************************/
void HRPerson_f::OnLoginCtrl(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_TAB || event.GetKeyCode() == WXK_BACK || event.GetKeyCode() == WXK_RETURN || 
	  event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_SHIFT ||
      event.GetKeyCode() ==  WXK_DELETE || event.GetKeyCode() ==  WXK_CLEAR) event.Skip();
	if (t_login->GetValue().Length() >= 11 ) return;
	char _70[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789-+~=()|";
	for (int i=0; i<70; i++) if (event.GetKeyCode() == _70[i]) event.Skip();
	//event.Skip(false);
	/*if (event.GetKeyCode() == WXK_TAB) event.Skip();
	char _44[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789-+~=()|";
	fors (int i=0; i<44; i++) if (event.GetKeyCode() == char[i]) event.Skip();
	event.Skip(false);*/
}
/*******************************************************************/
void HRPerson_f::OnPwdCtrl(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_TAB || event.GetKeyCode() == WXK_BACK || event.GetKeyCode() == WXK_RETURN || 
	  event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_SHIFT || 
	  event.GetKeyCode() == WXK_HOME || event.GetKeyCode() == WXK_END ||
      event.GetKeyCode() ==  WXK_DELETE || event.GetKeyCode() ==  WXK_CLEAR || 
	  event.GetKeyCode() == WXK_LEFT || event.GetKeyCode() == WXK_RIGHT) event.Skip();
	if (t_password->GetValue().Length() >= 11 ) return; //GetEventObject()
	char _70[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789-+~=()|";
	for (int i=0; i<70; i++) if (event.GetKeyCode() == _70[i]) event.Skip();
	//event.Skip(false);
}
void HRPerson_f::OnConfPwdCtrl(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_TAB || event.GetKeyCode() == WXK_BACK || event.GetKeyCode() == WXK_RETURN || 
	  event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_SHIFT ||
      event.GetKeyCode() ==  WXK_DELETE || event.GetKeyCode() ==  WXK_CLEAR) event.Skip();
	if (t_confpassword->GetValue().Length() >= 11 ) return;
	char _70[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789-+~=()|";
	for (int i=0; i<70; i++) if (event.GetKeyCode() == _70[i]) event.Skip();
	//event.Skip(false);
}
void HRPerson_f::OnZipCtrl(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_TAB || event.GetKeyCode() == WXK_BACK || event.GetKeyCode() == WXK_RETURN || 
	  event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_SHIFT ||
      event.GetKeyCode() ==  WXK_DELETE || event.GetKeyCode() ==  WXK_CLEAR) event.Skip();
	if (t_zip->GetValue().Length() >= 10 ) return;
	char _10[]= "0123456789";
	for (int i=0; i<10; i++) if (event.GetKeyCode() == _10[i]) event.Skip();
	//event.Skip(false);
}
/*******************************************************************/
void HRPerson_f::OnClearBmp(wxMouseEvent& event)
{
	popbmp_menu->UpdateUI();
	PopupMenu(popbmp_menu);
}
/*******************************************************************/
void HRPerson_f::OnBmp(wxCommandEvent& event)
{
	if (event.GetId() == 4002)
	{
		t_photo->SetBitmap(wxBitmap(t_photo->GetSize().GetWidth(), t_photo->GetSize().GetHeight()));
		is_clear = true;
		Update();
	}
	else if (event.GetId() == 4001)
	{
		wxFileDialog *dlg = new wxFileDialog(this);
		dlg->SetWildcard(_T("BMP files (*.bmp)|*.bmp|JPEG files (*.jpg)|*.jpg"));
		if (dlg->ShowModal() != wxID_OK) return;
		//ed_lang->SetValue(dlg->GetPath());
		
		if (dlg->GetFilterIndex() == 0)
			t_photo->SetBitmap(wxBitmap(dlg->GetPath(), wxBITMAP_TYPE_BMP));
		else if (dlg->GetFilterIndex() == 1)
			t_photo->SetBitmap(wxBitmap(dlg->GetPath(), wxBITMAP_TYPE_JPEG));
		//t_photo->SetBitmap(wxBitmap(dlg->GetPath(), wxBITMAP_TYPE_BMP));
	
		t_photo->SetSize(wxSize(220, 220));
		is_clear = false;
	}
	
}
/*******************************************************************/

