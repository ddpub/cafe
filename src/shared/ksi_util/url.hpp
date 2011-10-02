#ifndef URL_H_DERGACHEV_06_11_2008
#define URL_H_DERGACHEV_06_11_2008

#include <string>

class url_encode_t {
public:
	url_encode_t() {}
	~url_encode_t() {}
	std::string URLEncode(std::string);
private:
	static std::string UnsafeString;
	std::string decToHex(char num, int radix);
	bool is_un_safe(char);
	std::string convert(char);
};

#endif
