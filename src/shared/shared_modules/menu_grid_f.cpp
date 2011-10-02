
#include "menu_grid_f.h"
#include <ksi_include/ksi_exceptions.hpp>
#include <wx_util/wx_util.hpp>
#include <wx/msgdlg.h>

menu_grid_f::menu_grid_f( wxWindow* parent )
:
base_menu_grid_f( parent )
{

}

void menu_grid_f::menu_click( wxCommandEvent& event )
{
	EndModal(wxID_OK);
	// TODO: Implement menu_click
}

void menu_grid_f::add(ksi_cafe::menu_t* menu)
{
  kv_menu_[menu_list->Append(to_uc(menu->name()))] = menu;
  menu_list->Fit();
}

void menu_grid_f::select(ksi_cafe::menu_t* menu)
{
  for (size_t i=0; i<menu_list->GetCount(); ++i)
  {
    if (kv_menu_[i] == menu)
    {
      menu_list->Select(i);
      return;
    }
  }
  THROW_LOCATION(0, "void menu_grid_f::select(ksi_cafe::menu_t* menu)");
}

ksi_cafe::menu_t* menu_grid_f::selected()
{
  if (menu_list->GetSelection() == wxNOT_FOUND) THROW_LOCATION(0, "ksi_cafe::menu_t* menu_grid_f::selected()");
  return kv_menu_[menu_list->GetSelection()];
}
