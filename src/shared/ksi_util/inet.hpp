#ifndef DMITRY_DERGACHEV_10_04_2008_INET_H
#define DMITRY_DERGACHEV_10_04_2008_INET_H

#if !defined(_UNIX_BUILD_) && !defined(_WIN32_BUILD_)
#error define _UNIX_BUILD_ or _WIN32_BUILD_
#endif

#include <string>
#include <vector>

std::vector<std::string> get_mac();

#endif
