
#include "util.hpp"
#include <map>
//#include <sstream>
#include <cmath>

namespace ksi_cafe {


const std::string full_trim(const std::string& source) {
	std::string res = "";
	std::string tmp = source;
	bool insert_space = false;
	for (size_t i=0; i<tmp.length(); ++i) {
		if (tmp[i] != ' ') {
			insert_space = false;
			res = res + tmp[i];
		}
		else if (!insert_space) {
			res = res + ' ';
			insert_space = true;
		}
	}

	return res;
}

int red(int color) {
	// X1*16^5+X2*16^4+X3*16^3+X4*16^2+X5*16+X6
	int X1 = color/(16*16*16*16*16);
	int X2 = (color - X1*16*16*16*16*16)/(16*16*16*16); // int X2 = color
	return X1*16+X2;
}

int green(int color) {
	// X1*16^5+X2*16^4+X3*16^3+X4*16^2+X5*16+X6
	int X1 = color/(16*16*16*16*16);
	int X2 = (color - X1*16*16*16*16*16)/(16*16*16*16);
	int X3 = (color - X1*16*16*16*16*16 - X2*16*16*16*16)/(16*16*16);
	int X4 = (color - X1*16*16*16*16*16 - X2*16*16*16*16 - X3*16*16*16)/(16*16);
	return X3*16+X4;
}

int blue(int color) {
  // X1*16^5+X2*16^4+X3*16^3+X4*16^2+X5*16+X6
  int X1 = color/(16*16*16*16*16);
  int X2 = (color - X1*16*16*16*16*16)/(16*16*16*16);
  int X3 = (color - X1*16*16*16*16*16 - X2*16*16*16*16)/(16*16*16);
  int X4 = (color - X1*16*16*16*16*16 - X2*16*16*16*16 - X3*16*16*16)/(16*16);
  int X5 = (color - X1*16*16*16*16*16 - X2*16*16*16*16 - X3*16*16*16 - X4*16*16)/16;
  int X6 = (color - X1*16*16*16*16*16 - X2*16*16*16*16 - X3*16*16*16 - X4*16*16 - X5*16);
  return X5*16+X6;
}

int color_from_str(const std::string& color_str) {
	std::map<char, int> hexDigit;

	hexDigit['0'] = 0;
	hexDigit['1'] = 1;
	hexDigit['2'] = 2;
	hexDigit['3'] = 3;
	hexDigit['4'] = 4;
	hexDigit['5'] = 5;
	hexDigit['6'] = 6;
	hexDigit['7'] = 7;
	hexDigit['8'] = 8;
	hexDigit['9'] = 9;
	hexDigit['a'] = 10;
	hexDigit['A'] = 10;
	hexDigit['b'] = 11;
	hexDigit['B'] = 11;
	hexDigit['c'] = 12;
	hexDigit['C'] = 12;
	hexDigit['d'] = 13;
	hexDigit['D'] = 13;
	hexDigit['e'] = 14;
	hexDigit['E'] = 14;
	hexDigit['f'] = 15;
	hexDigit['F'] = 15;

	int color = 0;

	for (size_t i=0; i<color_str.length(); ++i) {
		color = color*16 + hexDigit[color_str[i]];
	}

	return color;
}

double round(double a) {
	double b = fabs(a)*100;
	b += 0.5;
	b = int(b);
	b /= 100;
	if (a < 0) b *= -1;
	return b;
}

const std::string last4digits(const std::string& msk_cc) {
	if (msk_cc.length() < 5) return msk_cc;
  
	std::string l4d = msk_cc.substr(msk_cc.length()-4, 4);
	std::string res = "";
	for (int i=0; i<msk_cc.length()-4; i++) res += "*";
	return res+l4d;
}

const std::string first6andlast4digits(const std::string& msk_cc) {
	if (msk_cc.length() < 6) return msk_cc;
	std::string l4d = msk_cc.substr(msk_cc.length()-4, 4);
	std::string res = msk_cc.substr(0, 6);
	for (int i=5; i<msk_cc.length()-4; i++) res += "*";
	return res+l4d;
}

const std::string masked_buff(const std::string& buff) {
	std::string num = buff.substr(buff.find(";")+1, buff.find("=")-buff.find(";")-1);
	return first6andlast4digits(num);
}

}
