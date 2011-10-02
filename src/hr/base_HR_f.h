///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_HR_f__
#define __base_HR_f__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/toolbar.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/listbox.h>
#include <wx/checklst.h>
#include <wx/stattext.h>
#include <wx/frame.h>

#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>

///////////////////////////////////////////////////////////////////////////

#define ADDPERS_ID 1000
#define EDITPERS_ID 1001
#define FIREPERS_ID 1002
#define REHIREPERS_ID 1003
#define UNLOCKPERS_ID 1004
#define DELPERS_ID 1005
#define ADDPOS_ID 1006
#define EDITPOS_ID 1007
#define DELPOS_ID 1008

///////////////////////////////////////////////////////////////////////////////
/// Class base_HR_f
///////////////////////////////////////////////////////////////////////////////
class base_HR_f : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel5;
		wxNotebook* nb_HR;
		wxPanel* wnd_pers;
		wxToolBar* tb_person;
		wxNotebook* nb_person;
		wxPanel* t_active;
		wxGrid* gr_pers_a;
		wxPanel* t_fired;
		wxGrid* gr_pers_f;
		wxPanel* wnd_pos;
		wxToolBar* tb_position;
		wxListBox* lb_position;
		wxCheckListBox* cl_role;
		wxStaticText* st_bar;
		
		wxTextCtrl *ed_cafefilter;
		wxTextCtrl *ed_rolefilter;
		wxCheckBox *ck_cafefilter;
		wxCheckBox *ck_rolefilter;
		wxButton *btn_cafe;
		wxButton *btn_role;
		wxPanel *ppp;
		
		virtual void OnCheckCafeFilter( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckRoleFilter( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextCafeFilter( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextRoleFilter( wxCommandEvent& event ){ event.Skip(); }
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnResize( wxSizeEvent& event ){ event.Skip(); }
		virtual void OnAdd( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnEdit( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnFire( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRehire( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnUnlock( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDelete( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnChangeActive( wxNotebookEvent& event ){ event.Skip(); }
		virtual void OnGridClick( wxGridEvent& event ){ event.Skip(); }
		virtual void OnGridDblClick( wxGridEvent& event ){ event.Skip(); }
		virtual void OnPopupMenuClick( wxGridEvent& event ){ event.Skip(); }
		virtual void OnSort( wxGridEvent& event ){ event.Skip(); }
		virtual void OnAddpos( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnEditpos( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDelpos( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnClickPos( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPropertyClick( wxMouseEvent& event ){ event.Skip(); }
		
	
	public:
		base_HR_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("HRManager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,472 ), long style = wxDEFAULT_FRAME_STYLE|wxALWAYS_SHOW_SB|wxTAB_TRAVERSAL );
		~base_HR_f();
	
};

#endif //__base_HR_f__
