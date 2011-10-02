#ifndef DMITRY_DERGACHEV_02_07_2008_LOGO_MAKER_H
#define DMITRY_DERGACHEV_02_07_2008_LOGO_MAKER_H

#include <vector>

std::vector<char> from_bmp(const std::vector<unsigned char>&, size_t width, size_t height);
std::vector<unsigned char> to_bmp(const std::vector<char>&, size_t& width, size_t& height);

#endif
