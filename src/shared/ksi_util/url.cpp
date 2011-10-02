
#include "url.hpp"

#include <algorithm>
#include <cmath>


char hexVals[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
std::string url_encode_t::UnsafeString = "\"<>%\\^[]`+$,@:;/!#?=&";

std::string url_encode_t::convert(char val)  {
	std::string  res;
	res += "%";
	res += decToHex(val, 16);	
	return  res;
}

std::string url_encode_t::decToHex(char num, int radix) {	
	int temp = 0;	
	std::string tmp_str;
	int num_char;
	num_char = (int) num;
	
	if (num_char < 0) num_char = 256 + num_char;

	while (num_char >= radix) {
		temp = num_char % radix;
		num_char = (int)floor(num_char / radix);
		tmp_str = hexVals[temp];
    }
	
	tmp_str += hexVals[num_char];

	if(tmp_str.length() < 2)
	{
		tmp_str += '0';
	}
	
	//std::string strdecToHex(tmp_str);
	std::reverse(tmp_str.begin(), tmp_str.end());
	
	return tmp_str;
}

bool url_encode_t::is_un_safe(char compareChar) {
	bool bcharfound = false;
	char tmpsafeChar;

	for(int i = 0; i < UnsafeString.length(); ++i) {
		tmpsafeChar = UnsafeString[i]; 
		if(tmpsafeChar == compareChar) { 
			bcharfound = true;
			break;
		} 
	}
	int char_ascii_value = 0;
	
	char_ascii_value = (int) compareChar;

	if(bcharfound == false &&  char_ascii_value > 32 && char_ascii_value < 123) {
		return false;
	}
	else {
		return true;
	}
	
	return true;
}

std::string url_encode_t::URLEncode(std::string source) {	
	std::string res;	

	for(int i = 0; i < source.length(); ++i) {
		char ch = source[i];
		if (ch < ' ')  {
			ch = ch;
		}		
		if(!is_un_safe(ch)) {
			// Safe Character				
			res += ch;
		}
		else {
			// get Hex Value of the Character
			res += convert(ch);
		}
	}

	return res;
}
