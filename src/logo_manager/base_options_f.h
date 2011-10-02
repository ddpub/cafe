///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_options_f__
#define __base_options_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_options_f
///////////////////////////////////////////////////////////////////////////////
class base_options_f : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_printer_port_lb;
		wxChoice* m_printer_port_cb;
		wxStaticText* m_command_set_lb;
		wxChoice* m_command_set_cb;
		wxStaticText* m_baud_rate_lb;
		wxChoice* m_baud_rate_cb;
		
		wxButton* ok_btn;
		
		wxButton* cancel_btn;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void m_printer_portChoice( wxCommandEvent& event ){ event.Skip(); }
		virtual void m_command_setChoice( wxCommandEvent& event ){ event.Skip(); }
		virtual void m_baud_rateChoice( wxCommandEvent& event ){ event.Skip(); }
		virtual void ok_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		base_options_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Printer settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 222,148 ), long style = wxDEFAULT_DIALOG_STYLE );
		~base_options_f();
	
};

#endif //__base_options_f__
