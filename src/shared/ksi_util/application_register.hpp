#ifndef APPLICATION_REGISTER_H_DERGACHEV_17_05_2008
#define APPLICATION_REGISTER_H_DERGACHEV_17_05_2008

#include <string>

void try_register(int app_id);
void try_cashier_register(const std::string& mac_addr);
void try_cashierW_register(const std::string& mac_addr);

#endif
