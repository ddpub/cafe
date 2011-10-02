#ifndef UTIL_H_DERGACHEV_24_12_2007
#define UTIL_H_DERGACHEV_24_12_2007

#include <string>

namespace ksi_cafe {

const std::string full_trim(const std::string&);
int color_from_str(const std::string& color_str);
int red(int color);
int green(int color);
int blue(int color);
double round(double);
const std::string last4digits(const std::string&);
const std::string first6andlast4digits(const std::string&);
const std::string masked_buff(const std::string&);
};

#endif
