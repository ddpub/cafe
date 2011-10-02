
#if !defined(_UNIX_BUILD_) && !defined(_WIN32_BUILD_)
#error define _UNIX_BUILD_ or _WIN32_BUILD_
#endif

#include "options_f.h"
#include <wx_util/wx_util.hpp>
#include <boost/lexical_cast.hpp>

options_f::options_f( wxWindow* parent, const wxString& port, const wxString& cset, const wxString& rate)
:
base_options_f( parent )
{
	for (size_t i=0; i<10; ++i)
	{		
		m_printer_port_cb->Append(_("COM")+to_uc(boost::lexical_cast<std::string>(i+1)));
	}
	m_command_set_cb->Append(_("Star"));
	m_command_set_cb->Append(_("Epson"));
	m_printer_port_cb->SetStringSelection(port);
	m_command_set_cb->SetStringSelection(cset);
	
	m_baud_rate_cb->Append(_("9600"));
	m_baud_rate_cb->Append(_("19200"));
	if (!m_baud_rate_cb->SetStringSelection(rate)) m_baud_rate_cb->Select(0);
	
	if (m_printer_port_cb->GetSelection() != wxNOT_FOUND && m_command_set_cb->GetSelection() != wxNOT_FOUND)
		ok_btn->Enable(true);
	else
		ok_btn->Enable(false);		
}

void options_f::m_printer_portChoice( wxCommandEvent& event )
{
	// TODO: Implement m_printer_portChoice
	if (m_printer_port_cb->GetSelection() != wxNOT_FOUND && m_command_set_cb->GetSelection() != wxNOT_FOUND)
		ok_btn->Enable(true);
	else
		ok_btn->Enable(false);
}

void options_f::m_command_setChoice( wxCommandEvent& event )
{
	// TODO: Implement m_command_setChoice
	if (m_printer_port_cb->GetSelection() != wxNOT_FOUND && m_command_set_cb->GetSelection() != wxNOT_FOUND)
		ok_btn->Enable(true);
	else
		ok_btn->Enable(false);	
}

void options_f::m_baud_rateChoice( wxCommandEvent& event )
{
	
}

void options_f::ok_btnClick( wxCommandEvent& event )
{
	// TODO: Implement ok_btnClick
	EndModal(wxID_OK);
}

void options_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

wxString options_f::get_printer_port() const
{
	return m_printer_port_cb->GetStringSelection();
}

wxString options_f::get_command_set() const
{
	return m_command_set_cb->GetStringSelection();
}

wxString options_f::get_baud_rate() const
{
	return m_baud_rate_cb->GetStringSelection();
}

