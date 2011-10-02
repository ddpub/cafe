#include "lm_f.h"
#include <wx/app.h>
#include <wx/dcclient.h>
#include <wx/brush.h>
#include <wx/colour.h>
#include <wx/event.h>
#include <wx/msgdlg.h>
#include <wx_util/wx_util.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <wx/filedlg.h>
#include <serial/com.hpp>
#include <printer/printer.hpp>
#include <wx/image.h>
#include <fstream>
#include <printer/logo_maker.hpp>
#include <wx/file.h>
#include <ksi_include/ksi_exceptions.hpp>
#include <algorithm>
#include <iterator>
#include "options_f.h"
#include <xml_config.hpp>
#include <wx/statusbr.h>


std::auto_ptr<xml_config_t> config;
namespace version {
extern std::string version;
extern std::string compamny;
extern std::string product;
extern std::string copyright;
};

lm_f::lm_f( wxWindow* parent )
:
base_lm_f( parent ), opened_(false), center_(false)
{	
	try
	{
		config.reset(new xml_config_t("logo_manager.xml"));
	}
	catch (exception_t& ex)
	{
		wxMessageBox(to_uc(ex.comp_message()));
		wxExit();
	}
	try
	{
		if (config->get("printer", "port") == "" || config->get("printer", "command_set") == "") return;
		com_.reset(new com_t(config->get("printer", "port"), 9600));
		if (config->get("printer", "command_set") == "Star") printer_.reset(new star_printer_t(*com_.get()));
		else if (config->get("printer", "command_set") == "Epson") printer_.reset(new epson_printer_t(*com_.get()));		
		else com_.reset();		
	}
	catch (exception_t& ex)
	{
		com_.reset();
		wxMessageBox(to_uc(ex.comp_message()));
	}
}

void lm_f::draw_image(bool OnPaint)
{
	std::auto_ptr<wxDC> dc;
	if (OnPaint) dc.reset(new wxPaintDC(bmp_));
	else dc.reset(new wxClientDC(bmp_));
	
	wxBrush brush(wxColour(255,255,255));
	dc->SetBrush(brush);		
	dc->DrawRectangle(0, 0, bmp_->GetClientSize().GetWidth(), bmp_->GetClientSize().GetHeight());		

	if (opened_)
	{
		dc->DrawBitmap(*pic_.get(), 0, 0, false);
	}	
}

void lm_f::correct_size()
{
	
	if (opened_ && printer_.get())
	{
		wxImage img(printer_->height(), pic_->GetHeight());
		
		wxImage oimg(pic_->ConvertToImage());
		int w = img.GetWidth() > oimg.GetWidth() ? oimg.GetWidth() : img.GetWidth();
		for (int i=0; i<w; ++i)
			for (int j=0; j<img.GetHeight(); ++j)
			{
				unsigned char r = oimg.GetRed(i,j);
				unsigned char g = oimg.GetGreen(i,j);
				unsigned char b = oimg.GetBlue(i,j);
				img.SetRGB(i, j, r, g, b);
			}
		for (int i=w; i<img.GetWidth(); ++i)
			for (int j=0; j<img.GetHeight(); ++j)
			{
				img.SetRGB(i, j, 255, 255, 255);
			}
		pic_.reset(new wxBitmap(img));
	}
	if (pic_.get()) bmp_->SetBitmap(*pic_.get());	
	draw_image(false);	
	if (opened_ && printer_.get()) Fit();
}

void lm_f::show_options()
{
	wx_window_ptr<options_f> form (new options_f(this, to_uc(config->get("printer", "port")), to_uc(config->get("printer", "command_set")), to_uc(config->get("printer", "baud_rate"))));
	if (form->ShowModal() != wxID_OK) return;
	config->set("printer", "port", to_mb(form->get_printer_port()));
	config->set("printer", "command_set", to_mb(form->get_command_set()));
	config->set("printer", "baud_rate", to_mb(form->get_baud_rate()));
	config->sync();
	try
	{
		printer_.reset();
		com_.reset();		
		com_.reset(new com_t(config->get("printer", "port"), 9600));
		if (config->get("printer", "command_set") == "Star") printer_.reset(new star_printer_t(*com_.get()));
		else if (config->get("printer", "command_set") == "Epson") printer_.reset(new epson_printer_t(*com_.get()));		
		else com_.reset();
	}
	catch (exception_t& ex)
	{
		com_.reset();
		wxMessageBox(to_uc(ex.comp_message()));
	}	
	correct_size();
}

void lm_f::show_about()
{
	wxMessageDialog dialog( this, _("Welcome to Logo Manager ")+to_uc(version::version)+_("\n       (C)opyright KSI Corp."), _("About Logo Manager"), wxOK|wxICON_INFORMATION );
    dialog.ShowModal();
}

void lm_f::bmpOnPaint( wxPaintEvent& event )
{
	// TODO: Implement bmpOnPaint
	draw_image(true);
}

void lm_f::bmpOnSize( wxSizeEvent& event )
{
	// TODO: Implement bmpOnSize
	draw_image(false);
}

void lm_f::m_OpenClick( wxCommandEvent& event )
{
	// TODO: Implement m_OpenClick
	wx_window_ptr<wxFileDialog> openDlg (new wxFileDialog(this, _("Open"), _(""), _(""), _("Image files (*.bmp)|*.bmp|PRN files (*.prn)|*.prn"), wxFD_OPEN));
	if (openDlg->ShowModal() != wxID_OK) return;
	std::auto_ptr<wxImage> tmp (new wxImage());
	std::vector<unsigned char> bmp_data;
	pic_file_ = openDlg->GetFilename();
	pic_dir_ = openDlg->GetDirectory();	
		
	if (openDlg->GetFilterIndex() == 0)
	{
		binary_ = false;
		if (!tmp->LoadFile(openDlg->GetPath(), wxBITMAP_TYPE_ANY))
		{
			wxMessageBox(_("can't open file: ")+pic_file_);
			return;
		}
		
	}
	else if (openDlg->GetFilterIndex() == 1)
	{
		binary_ = true;
		size_t width, height;
		wxFile file;
		if (!file.Open(openDlg->GetPath()))
		{
			wxMessageBox(_("can't open file: ")+pic_file_);
			return;
		}
		size_t len = file.Length();		
		std::vector<char> data(len);
		file.Read(&data[0], len);
		try
		{
			bmp_data = to_bmp(data, width, height);
			wxImage img(width, height, &bmp_data[0], true);
			tmp.reset(new wxImage(img));
		}
		catch (exception_t& ex)
		{
			wxMessageBox(to_uc(ex.comp_message()));
			return;
		}
	}
	else
	{
		return;
	}
	
	SetLabel(openDlg->GetFilename()+_(" - Logo Manager"));
	pic_.reset(new wxBitmap(*tmp.get()));	
	backUp_pic_.reset(new wxBitmap(*tmp.get()));	
	opened_ = true;
	center_ = false;
	correct_size();	
}

void lm_f::m_save_asClick( wxCommandEvent& event )
{
	// TODO: Implement m_save_asClick
	wx_window_ptr<wxFileDialog> saveDlg (new wxFileDialog(this, _("Save as..."), pic_dir_, _(""), _("Image files (*.bmp)|*.bmp|PRN files (*.prn)|*.prn"), wxFD_SAVE));
	if (!binary_) saveDlg->SetFilterIndex(1);
	if (saveDlg->ShowModal() != wxID_OK) return;
	std::string out_file = to_mb(saveDlg->GetPath());
	wxImage img_ = pic_->ConvertToImage();	
		
	std::vector<char> data;
	try
	{
		std::vector<unsigned char> source;
		std::copy(img_.GetData(), img_.GetData()+img_.GetWidth()*img_.GetHeight()*3, std::back_inserter(source));
		data = from_bmp(source, img_.GetWidth(), img_.GetHeight());
	}
	catch (exception_t& ex)
	{
		wxMessageBox(to_uc(ex.comp_message()));
		return;	
	}
	std::ofstream f(out_file.c_str(), std::ios::out | std::ios::binary);
		
	f.write(&data[0], data.size());
	f.close();
}

void lm_f::m_save_asOnPopUp( wxUpdateUIEvent& event )
{
	// TODO: Implement m_save_asOnPopUp
	m_save_as->Enable(opened_);
}

void lm_f::m_exitClick( wxCommandEvent& event )
{
	// TODO: Implement m_exitClick
	wxExit();
}

void lm_f::m_register_logoClick( wxCommandEvent& event )
{
	// TODO: Implement m_register_logoClick
	wxImage img_ = pic_->ConvertToImage();	
	std::vector<char> data;
	try
	{
		std::vector<unsigned char> source;
		std::copy(img_.GetData(), img_.GetData()+img_.GetWidth()*img_.GetHeight()*3, std::back_inserter(source));
		data = from_bmp(source, img_.GetWidth(), img_.GetHeight());
	}
	catch (exception_t& ex)
	{
		wxMessageBox(to_uc(ex.comp_message()));
		return;	
	}
	
	wx_change_cursor_t change_cursor(this,  wxCursor(wxCURSOR_ARROW),  wxCursor(wxCURSOR_WAIT));
	printer_->init();
	printer_->set_logo(data);
	printer_->init();
	printer_->print_logo();
	printer_->cut();	
}

void lm_f::m_register_logoOnPopUp( wxUpdateUIEvent& event )
{
	// TODO: Implement m_saveOnPopUp
	m_register_logo->Enable(printer_.get() && opened_);	
}

void lm_f::m_centerClick( wxCommandEvent& event )
{
	if (!center_)
	{
		wxImage img(pic_->GetWidth(), pic_->GetHeight());
		wxImage oimg = backUp_pic_->ConvertToImage();
		//for (int i=0; i<img.GetWidth(); ++i)
			//for (int j=0; j<img.GetHeight(); ++j)
				//img.SetRGB(i,j, 255,255,255);
		
		int start = img.GetWidth()/2 - oimg.GetWidth()/2;
		if (start < 0) return;
		int stop = oimg.GetWidth();
		
		
		for (int i=0; i<start; ++i)
			for (int j=0; j<img.GetHeight(); ++j)
			{
				img.SetRGB(i, j, 255, 255, 255);
			}
		
		for (int i=start; i<stop; ++i)
			for (int j=0; j<img.GetHeight(); ++j)
			{
				unsigned char r = oimg.GetRed(i-start,j);
				unsigned char g = oimg.GetGreen(i-start,j);
				unsigned char b = oimg.GetBlue(i-start,j);
				img.SetRGB(i, j, r, g, b);
			}
		
		for (int i=stop; i<img.GetWidth(); ++i)
			for (int j=0; j<img.GetHeight(); ++j)
			{
				img.SetRGB(i, j, 255, 255, 255);
			}
				
		
		for (int i=0; i<oimg.GetWidth(); ++i)
		{
			if (i>=img.GetWidth()) break;
			for (int j=0; j<oimg.GetHeight(); ++j)
			{
				unsigned char r = oimg.GetRed(i,j);
				unsigned char g = oimg.GetGreen(i,j);
				unsigned char b = oimg.GetBlue(i,j);
				img.SetRGB(i+(img.GetWidth()-oimg.GetWidth())/2, j, r, g, b);				
			}
		}
		
		pic_.reset(new wxBitmap(img));
	}
	else
	{
		pic_.reset(new wxBitmap(*backUp_pic_.get()));		
	}
	center_ = !center_;
	correct_size();
}

void lm_f::m_centerOnPopUp( wxUpdateUIEvent& event )
{
	m_center->Enable(printer_.get() && opened_);
	if (center_) m_center->SetItemLabel(_("UnCenter image"));
	else m_center->SetItemLabel(_("Center image"));
}

void lm_f::m_optionsClick( wxCommandEvent& event )
{
	// TODO: Implement m_optionsClick
	show_options();
}

void lm_f::m_aboutClick( wxCommandEvent& event )
{
	// TODO: Implement m_aboutClick
	show_about();
}

void lm_f::statusBarDClick( wxMouseEvent& event )
{
	wxRect rect1, rect2;
	m_statusBar->GetFieldRect(1, rect1);
	m_statusBar->GetFieldRect(2, rect2);
	int x = event.GetPosition().x;
	int y = event.GetPosition().y;
	if (rect1.GetLeft() <= x && x <= rect1.GetRight() && rect1.GetTop() <= y && y <= rect1.GetBottom())
	{
		show_options();
	}
	else if (rect2.GetLeft() <= x && x <= rect2.GetRight() && rect2.GetTop() <= y && y <= rect2.GetBottom())
	{
		show_about();
	}	
}

void lm_f::statusBarUpdateUI( wxUpdateUIEvent& event )
{
	m_statusBar->SetStatusText(_(""), 0);
	wxString new_1 = to_uc(config->get("printer", "port"))+_("/")+to_uc(config->get("printer", "command_set"))+_("/")+to_uc(config->get("printer", "baud_rate"));
	wxString new_2 = _("v")+to_uc(version::version);
	bool change = false;
	
	if (m_statusBar->GetStatusText(1) != new_1) { m_statusBar->SetStatusText(new_1, 1); change = true; }
	if (m_statusBar->GetStatusText(2) != new_2) { m_statusBar->SetStatusText(new_2, 2); change = true; }
	
	if (change)
	{
		std::vector<int> widths;
		widths.push_back(-1);
		std::auto_ptr<wxClientDC> dc (new wxClientDC(m_statusBar));
	
		widths.push_back(dc->GetTextExtent(m_statusBar->GetStatusText(1)).GetWidth());
		widths.push_back(dc->GetTextExtent(m_statusBar->GetStatusText(2)).GetWidth());
		m_statusBar->SetStatusWidths(3,&widths[0]);
	}
}
