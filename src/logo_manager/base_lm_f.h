///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_lm_f__
#define __base_lm_f__

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_lm_f
///////////////////////////////////////////////////////////////////////////////
class base_lm_f : public wxFrame 
{
	private:
	
	protected:
		wxBoxSizer* bmpSizer_;
		wxStaticBitmap* bmp_;
		wxMenuBar* m_menubar;
		wxMenu* m_file;
		wxMenuItem* m_save_as;
		wxMenu* m_tools;
		wxMenuItem* m_register_logo;
		wxMenuItem* m_center;
		wxMenu* m_help;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void bmpOnPaint( wxPaintEvent& event ){ event.Skip(); }
		virtual void bmpOnSize( wxSizeEvent& event ){ event.Skip(); }
		virtual void m_OpenClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void m_save_asClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void m_save_asOnPopUp( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void m_exitClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void m_register_logoClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void m_register_logoOnPopUp( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void m_centerClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void m_centerOnPopUp( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void m_optionsClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void m_aboutClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void statusBarDClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void statusBarUpdateUI( wxUpdateUIEvent& event ){ event.Skip(); }
		
	
	public:
		base_lm_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Logo Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,200 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		~base_lm_f();
	
};

#endif //__base_lm_f__
