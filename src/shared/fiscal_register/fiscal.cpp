
#include "fiscal.hpp"
#include <boost/thread/thread.hpp>
#include <iostream>
#include "strerror.hpp"
#include <ksi_include/algo_functor.hpp>

#define __DEBUG__

#ifdef __DEBUG__
#include <fstream>
std::ofstream of("fiscal.txt");
#endif

void print_data(const std::deque<fiscal::dataT>& data) {
#ifdef __DEBUG__	
	for (size_t i=0; i<data.size(); ++i) of << std::hex << std::uppercase << "0x" << short(data[i]) << " ";
	of << std::endl;
	of << std::dec;
#endif
}

void print_byte(fiscal::dataT byte) {
#ifdef __DEBUG__
	of << std::hex << std::uppercase << "0x" << short(byte) << " ";
	of << std::endl;
	of << std::dec;
#endif
}

namespace fiscal {

size_t communicator_t::sendByte(const dataT& byte) {
	return com_.writeByte<dataT>(byte);
}

size_t communicator_t::recvByte(dataT& response, size_t timeout) {
	com_.set_timeouts(timeout, proto::timeout::MAX, proto::timeout::MAX);
	dataT buff[2];
	size_t res = com_.read<dataT>(buff, 1);
	response = buff[0];
	return res;
}

size_t communicator_t::sendVector(const std::deque<dataT>& data) {
	size_t writed = 0;
	for(size_t i=0; i<data.size(); ++i) {
		if (sendByte(data[i])) ++writed;
		else return writed;
	}
	return writed;
}

std::deque<dataT> communicator_t::recvVector(size_t timeout) {
	std::deque<dataT> res;
	dataT byte;
	while (recvByte(byte, timeout)) {
		res.push_back(byte);
	}
	return res;
}

void communicator_t::send(const std::deque<dataT>& data) {
	for (size_t i=0; i<2; ++i) {
		if (sendByte(proto::segment::ENQ)) {
			dataT response;			
			if (recvByte(response, proto::timeout::T1)) {
				if (i == 0 && response == proto::segment::ENQ) {
					boost::this_thread::sleep(boost::posix_time::milliseconds(fiscal::proto::timeout::T7));
					continue;
				}
				if (response != proto::segment::ACK) {
					throw exception(1, "send. Invalid response, expected proto::segment::ACK");
				}
				size_t cnt = sendVector(data);
				if (cnt != data.size()) {
					boost::this_thread::sleep(boost::posix_time::milliseconds(proto::timeout::T2));
					throw exception(2, "send. Failed send data");
				}
				if (recvByte(response, proto::timeout::T3)) {
					if (response == proto::segment::ACK) {
						sendByte(proto::segment::EOT);
						return;
					}
					else throw exception(3, "send. Invalid response, expected proto::segment::ACK");
				}
				else throw exception(4, "send. No data found, expected proto::segment::ACK");
			}
		}
		throw exception(5, "send. Failed send proto::segment::ENQ");
	}
}

std::deque<dataT> communicator_t::receive(size_t timeout) {
	std::deque<dataT> response_vec;
	dataT byte;
	if (recvByte(byte, timeout)) {
		if (byte == proto::segment::ENQ) {
			if (sendByte(proto::segment::ACK)) {
				for (size_t i=0; i<10; ++i) {		
					std::deque<dataT> data;
					if (recvByte(byte, proto::timeout::T2)) {
						if (byte == proto::segment::STX) {							
							data.push_back(byte);	
						}						
						while (recvByte(byte, proto::timeout::T6)) {
							data.push_back(byte);
							if (data.size() > 2 && data[data.size()-3] != proto::segment::DLE && data[data.size()-2] == proto::segment::ETX) {
								try {									
									response_vec = unpack(data);
									if (sendByte(proto::segment::ACK)) {
										recvByte(byte, proto::timeout::T6);
									}
									else throw exception(6, "receive. Failed send finally proto::segment::ACK");
									return response_vec;
								}
								catch (exception& ex) {
									if (ex.msg_id() != 6) throw;
								}
								if (!sendByte(proto::segment::NAK)) {
									throw exception(7, "receive. Failed send proto::segment::NAK");
								}
							}
						}
					}
					else throw exception(8, "receive. Failed receive proto::segment::STX");
				}
				throw exception(9, "receive. Failed receive message");
			}
			else throw exception(10, "receive. Failed send proto::segment::ACK");
		}
	}
}

dataT CRC(const std::deque<dataT>& data) {
	if (data.size() < 2) return 0;
	dataT crc = *(data.begin());
	for (std::deque<dataT>::const_iterator i=data.begin()+1; i != data.end(); ++i) {
		crc ^=*i;
	}
	return crc;
}

std::deque<dataT> mask_data(const std::deque<dataT>& data) {
	std::deque<dataT> masked;
	for (std::deque<dataT>::const_iterator i=data.begin(); i != data.end(); ++i) {
		if (*i == proto::segment::DLE || *i == proto::segment::ETX) masked.push_back(proto::segment::DLE);
		masked.push_back(*i);
	}
	return masked;
}

std::deque<dataT> unmask_data(const std::deque<dataT>& masked) {
	std::deque<dataT> data;
	for (std::deque<dataT>::const_iterator i=masked.begin(); i != masked.end(); ++i) {
		if (*i == proto::segment::DLE) ++i;
		if (i != masked.end()) data.push_back(*i);
	}
	return data;
}

std::deque<dataT> pack(const std::deque<dataT>& data) {
	std::deque<dataT> packet;
	std::deque<dataT> masked = mask_data(data);
	std::copy(masked.begin(), masked.end(), std::back_inserter(packet));
	packet.push_back(proto::segment::ETX);
	dataT crc = CRC(packet);
	packet.push_back(crc);
	packet.push_front(0x0);
	packet.push_front(0x0);
	packet.push_front(proto::segment::STX);
	//packet.insert(packet.begin(), proto::segment::STX);
	return packet;
}

std::deque<dataT> unpack(const std::deque<dataT>& packet) {
	std::deque<dataT> data;
	if (packet.size() < 3) 
		throw exception(1, "unpack. Invalid packet.size() = " + boost::lexical_cast<std::string>(packet.size()));
	dataT crc = packet[packet.size()-1];
	if (packet[0] != proto::segment::STX) 
		throw exception(2, "unpack. Invaled start (STX) bit");		
	std::deque<dataT> masked;
	std::copy(packet.begin()+1, packet.end()-1, std::back_inserter(masked));
	dataT calc_crc = CRC(masked);
	if (calc_crc != crc) 
		throw exception(3, "unpack. Invaled crc");	
	if (masked[masked.size()-1] != proto::segment::ETX) 
		throw exception(4, "unpack. Invalid end (ETX) bit");	
	masked.erase(masked.end()-1);
	data = unmask_data(masked);
	return data;
}

double fromBCD(const std::deque<dataT>& data) {
	std::deque<size_t> digits;
	for (size_t i=0; i<data.size(); ++i) {
		dataT item = data[i];
		dataT low = item & 0x0F;
		dataT hi = item >> 4;
		digits.push_front(hi);
		digits.push_front(low);
	}
	double res = 0;
	size_t m = 1;
	for (size_t i=0; i<digits.size(); ++i) {
		res += digits[i]*m;
		m *= 10;
	}
	return res;
}

std::deque<dataT> make_money(double val) {
	std::deque<dataT> res;
	size_t rval = size_t(val*100+0.5);
	std::string bcd = boost::lexical_cast<std::string>(rval);
	bcd = std::string(10-bcd.length(), '0') + bcd;
	for(size_t i=0; i<5; ++i) {
		size_t hi = boost::lexical_cast<size_t>(bcd.substr(i*2,1));
		size_t low = boost::lexical_cast<size_t>(bcd.substr(i*2+1, 1));
		size_t item = (hi << 4) + low;
		res.push_back(item);
	}
	return res;
}

std::deque<dataT> make_weight(double val) {
	std::deque<dataT> res;
	size_t rval = size_t(val*1000+0.5);
	std::string bcd = boost::lexical_cast<std::string>(rval);
	bcd = std::string(10-bcd.length(), '0') + bcd;
	for(size_t i=0; i<5; ++i) {
		size_t hi = boost::lexical_cast<size_t>(bcd.substr(i*2,1));
		size_t low = boost::lexical_cast<size_t>(bcd.substr(i*2+1, 1));
		size_t item = (hi << 4) + low;
		res.push_back(item);
	}
	return res;
}

int haveError(const std::deque<dataT>& response, std::string& err) {
	err = "";
	if (response.size() >=2 && response[0] == 0x55) {
		if (response[1] == 0x0) return 0;
		err = strerr::err.get(response[1]);
		return response[1];
	}
	return 0;
}

bool toKKM(char ch, dataT& data) {
	typedef unsigned char uchar_t;
	if (ch == 'ё') ch = 'е';
	if (ch == 'Ё') ch = 'Е';
	if (ch == '№') data = 0x24;
	else if (ch == '-') data = 0xFA;
	else if (ch == '$') data = 0xFC;
	else if (ch >= ' ' && ch <= '~') data = uchar_t(ch);
	else if (ch >= 'А' && ch <= 'Я') data = 0x80 + (uchar_t(ch) - uchar_t('А'));
	else if (ch >= 'а' && ch <= 'п') data = 0xA0 + (uchar_t(ch) - uchar_t('а'));
	else if (ch >= 'р' && ch <= 'я') data = 0xE0 + (uchar_t(ch) - uchar_t('р'));
	else {
		ch = 0x20;
		return false;
	}
	return true;
}

bool fromKKM(dataT data, char& ch) {
	if (data >= 0 && data <= 31) ch = 0xC0 + data;
	else if (data >= 32 && data <= 35) ch = 0x20 + (data - 32);
	else if (data == 36) ch = '№';
	else if (data >= 37 && data <= 126) ch =0x25 + (data - 37);
	else if (data == 127) ch = 0x20;
	else if (data >= 128 && data <= 159) ch = 0xE0 + (data - 128);
	else if (data == 160) ch = '$';
	else if (data == 0xFE) ch = 0;
	else return false;
	return true;
/*	
	cp866
	if (data >= 0 && data <= 31) ch = 0x80 + data;
	else if (data >= 32 && data <= 35) ch = 0x20 + (data - 32);
	else if (data == 36) ch = '№';
	else if (data >= 37 && data <= 126) ch = 0x25 + (data - 37);
	else if (data == 127) ch = 0x20;
	else if (data >= 128 && data <= 143) ch = 0xA0 + (data - 128);
	else if (data >= 144 && data <= 159) ch = 0xE0 + (data - 144);
	else if (data == 160) ch = '$';
	else if (data == 0xFE) ch = 0;
	else return false;
	return true;
*/
}

bool toKKM(const std::string& str, std::deque<dataT>& data, bool wide) {
	data.clear();
	bool success = true;
	dataT item;
	for (std::string::const_iterator i=str.begin(); i!=str.end(); ++i) {
		bool cur_success = toKKM(*i, item);
		if (!cur_success) success = false;
		if (wide && *i != ' ') data.push_back(0x09);
		data.push_back(item);
	}
	return success;
}

bool fromKKM(const std::deque<dataT>& data, std::string& str) {
	str = "";
	bool success = true;
	char ch;
	for (std::deque<dataT>::const_iterator i=data.begin(); i!=data.end(); ++i) {
		bool cur_success = fromKKM(*i, ch);
		if (!cur_success) success = false;
		if (ch != 0) str += ch;
	}
	return success;
}

mode::mode_t modeFromCode(size_t code) {
	if (code == 0) return mode::SEL;
	else if (code == 1) return mode::REG;
	else if (code == 2) return mode::REP1;
	else if (code == 3) return mode::REP2;
	else if (code == 4) return mode::PRO;
	else if (code == 5) return mode::AFM;
	else if (code == 6) return mode::APL;
	return mode::INV;
}

std::string modeName(mode::mode_t mode) {
	if (mode == mode::SEL) return "Режим выбора";
	else if (mode == mode::REG) return "Режим регистрации";
	else if (mode == mode::REP1) return "Режим отчетов без гашения";
	else if (mode == mode::REP2) return "Режим отчетов с гашением";
	else if (mode == mode::PRO) return "Режим программирования";
	else if (mode == mode::AFM) return "Режим доступа к ФП";
	else if (mode == mode::APL) return "Режим доступа к ЭКЛЗ";
	return "Режим не существует";
}

void registrator_t::Zreport() {
	std::deque<dataT> cmd;
	cmd.push_back(0x5A);
	try {
		changeMode(fiscal::mode::REP2);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "Zreport. Device error: " + err);
		std::pair<size_t, size_t> stat;
		while (true) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			stat = getRawMode();
			if (stat.first == 3 && stat.second == 2) {
				continue;
			}
			if (stat.first == 7 && stat.second == 1) {
				break;
			}
			throw exception(333, "Zreport. Invalid stat. first = " + boost::lexical_cast<std::string>(stat.first) + ", second = " + boost::lexical_cast<std::string>(stat.second));
		}
		size_t counter = 0;
		while (true) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			stat = getRawMode();
			counter++;
			if (stat.first == 7 && stat.second == 1) {
				if (counter == 20) throw exception_t(111, "Zreport. Operation TimeOut");
				continue;
			}
			return;
		}
	}
	catch (exception& ex) {
		throw;
	}
}

bool registrator_t::open_shift(const std::string& name, bool check) {
	std::deque<dataT> cmd;
	std::string sname = name.substr(0, 32);
	if (!toKKM(sname, cmd, false)) throw exception(5, "open_shift. Failed toKKM");
	if (check) cmd.push_front(1);
	else cmd.push_front(0x0);
	cmd.push_front(0x9A);
	try {
		changeMode(fiscal::mode::REG);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "open_shift. Device error: " + err);
	}	
	catch (exception& ex) {
		if (check) return false;
		throw;// exception(1, "open_shift. " + ex.fmsg());
	}
	return true;
}

std::deque<dataT> registrator_t::readTable(size_t table, size_t row, size_t field) {
	std::deque<dataT> cmd;
	cmd.push_back(0x46);
	cmd.push_back(table);
	cmd.push_back(0x0);
	cmd.push_back(row);
	cmd.push_back(field);
	try {
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "readTable. Device error: " + err);
		std::deque<dataT> res;
		std::copy(recv.begin()+2, recv.end(), std::back_inserter(res));
		return res;
	}
	catch (exception& ex) {
		throw;// exception(2, "readTable. " + ex.fmsg());
	}
}

void registrator_t::setTable(size_t table, size_t row, size_t field, const std::deque<dataT>& data) {
	std::deque<dataT> cmd;
	cmd.push_back(0x50);
	cmd.push_back(table);
	cmd.push_back(0x0);
	cmd.push_back(row);
	cmd.push_back(field);
	std::copy(data.begin(), data.end(), std::back_inserter(cmd));
	try {
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "readTable. Device error: " + err);
	}
	catch (exception& ex) {
		throw;// exception(3, "readTable. " + ex.fmsg());
	}	
}

void registrator_t::setFooter(const std::string& str) {
	size_t ret_pos = str.find('\n');
	std::string line1 = str.substr(0, ret_pos);
	std::string line2 = "";
	if (ret_pos != std::string::npos) line2 = str.substr(ret_pos+1, std::string::npos);
	line1 = fixFormat(line1, format::LEFT, size_);
	line2 = fixFormat(line2, format::LEFT, size_);
	changeMode(fiscal::mode::PRO);
	std::deque<dataT> res;
	if (!fiscal::toKKM(line1, res, false)) throw exception(6, "setFooter. Failed toKKM");
	setTable(6, 1, 1, res);
	res.clear();
	if (!fiscal::toKKM(line2, res, false)) throw exception(6, "setFooter. Failed toKKM");
	setTable(6, 2, 1, res);
	changeMode(fiscal::mode::SEL);
}

bool registrator_t::open_receipt(receipt_type::receipt_type_t type, bool check) {
	std::deque<dataT> cmd;
	cmd.push_back(0x92);
	if (check) cmd.push_back(0x1);
	else cmd.push_back(0x0);
	cmd.push_back(type);
	try {
		changeMode(fiscal::mode::REG);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "open_receipt. Device error: " + err);
	}
	catch (exception& ex) {
		if (check) return false;
		throw;// exception(4, "open_receipt. " + ex.fmsg());
	}
	return true;
}

void registrator_t::cancel_receipt() {
	std::deque<dataT> cmd;
	cmd.push_back(0x59);
	try {
		changeMode(fiscal::mode::REG);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "cancel_receipt. Device error: " + err);
	}
	catch (exception& ex) {
		throw;// exception(5, "cancel_receipt. " + ex.fmsg());
	}
}

bool registrator_t::cash_in(double value, bool check) {
	std::deque<dataT> cmd;
	cmd.push_back(0x49);
	if (check) cmd.push_back(0x1);
	else cmd.push_back(0x0);
	std::deque<dataT> money = make_money(value);
	std::copy(money.begin(), money.end(), std::back_inserter(cmd));
	try {
		changeMode(fiscal::mode::REG);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "cash_in. Device error: " + err);
	}
	catch (exception& ex) {
		if (check) return false;
		throw;// exception(6, "cash_in. " + ex.fmsg());
	}
	return true;
}

bool registrator_t::cash_out(double value, bool check) {
	std::deque<dataT> cmd;
	cmd.push_back(0x4F);
	if (check) cmd.push_back(0x1);
	else cmd.push_back(0x0);
	std::deque<dataT> money = make_money(value);
	std::copy(money.begin(), money.end(), std::back_inserter(cmd));
	try {
		changeMode(fiscal::mode::REG);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "cash_out. Device error: " + err);
	}
	catch (exception& ex) {
		if (check) return false;
		throw;// exception(7, "cash_out. " + ex.fmsg());
	}
	return true;
}

bool registrator_t::sale(double price, size_t cnt, bool check) {
	std::deque<dataT> cmd;
	cmd.push_back(0x52);
	if (check) cmd.push_back(0x1);
	else cmd.push_back(0x0);
	std::deque<dataT> data = make_money(price);
	std::copy(data.begin(), data.end(), std::back_inserter(cmd));
	data = make_weight(cnt);
	std::copy(data.begin(), data.end(), std::back_inserter(cmd));
	cmd.push_back(0x0);
	try {
		changeMode(fiscal::mode::REG);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "sale. Device error: " + err);
	}
	catch (exception& ex) {
		if (check) return false;
		throw;// exception(8, "sale. " + ex.fmsg());
	}
	return true;
}

bool registrator_t::cancel(double price, size_t cnt, bool check) {
	std::deque<dataT> cmd;
	cmd.push_back(0x41);
	if (check) cmd.push_back(0x1);
	else cmd.push_back(0x0);
	std::deque<dataT> data = make_money(price);
	std::copy(data.begin(), data.end(), std::back_inserter(cmd));
	data = make_weight(cnt);
	std::copy(data.begin(), data.end(), std::back_inserter(cmd));
	try {
		changeMode(fiscal::mode::REG);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "cancel. Device error: " + err);
	}
	catch (exception& ex) {
		if (check) return false;
		throw;// exception(9, "cancel. " + ex.fmsg());
	}
	return true;
}

bool registrator_t::refund(double price, size_t cnt, bool check) {
	std::deque<dataT> cmd;
	cmd.push_back(0x57);
	if (check) cmd.push_back(0x1);
	else cmd.push_back(0x0);
	std::deque<dataT> data = make_money(price);
	std::copy(data.begin(), data.end(), std::back_inserter(cmd));
	data = make_weight(cnt);
	std::copy(data.begin(), data.end(), std::back_inserter(cmd));
	try {
		changeMode(fiscal::mode::REG);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "refund. Device error: " + err);
	}
	catch (exception& ex) {
		if (check) return false;
		throw;// exception(10, "refund. " + ex.fmsg());
	}
	return true;
}

double registrator_t::final_pay(payment_type::payment_type_t type, double price, bool check) {
	std::deque<dataT> cmd;
	cmd.push_back(0x99);
	if (check) cmd.push_back(0x1);
	else cmd.push_back(0x0);
	cmd.push_back(type);
	std::deque<dataT> money = make_money(price);
	std::copy(money.begin(), money.end(), std::back_inserter(cmd));
	try {
		changeMode(fiscal::mode::REG);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "final_pay. Device error: " + err);
		if (recv.size() != 12) throw exception(haveError(recv, err), "final pay. Invalid recv.size() = " + boost::lexical_cast<size_t>(recv.size()));
		std::deque<dataT> change;
		std::copy(recv.begin()+2, recv.begin()+7, std::back_inserter(change));
		return fromBCD(change)/100;
	}
	catch (exception& ex) {
		throw;// exception(11, "final_pay. " + ex.fmsg());
	}
}

void registrator_t::print_fl() {
	printText("--------------------------------");
}

bool registrator_t::close_receipt(payment_type::payment_type_t type, bool check) {
	std::deque<dataT> cmd;
	cmd.push_back(0x4A);
	if (check) cmd.push_back(0x1);
	else cmd.push_back(0x0);
	cmd.push_back(type);
	std::deque<dataT> money = make_money(0);
	std::copy(money.begin(), money.end(), std::back_inserter(cmd));
	try {
		changeMode(fiscal::mode::REG);
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "close_receipt. Device error: " + err);
	}
	catch (exception& ex) {
		if (check) return false;
		throw;// exception(12, "close_receipt. " + ex.fmsg());
	}
	return true;
}

std::pair<size_t, size_t> registrator_t::getRawMode() {
	std::deque<dataT> stat = getStat();
	if (stat.size() != 3) throw exception_t(1, "getMode. Invalid stat.size() = " + boost::lexical_cast<size_t>(stat.size()));
	if (stat[0] != 0x55) throw exception_t(2, "getMode. Invalid start (0x55) bit");
	dataT s = stat[1];
	size_t mode = 0x0F & s;
	size_t sub_mode = s >> 4;
	return std::pair<size_t, size_t>(mode, sub_mode);
}

std::pair<mode::mode_t, mode::mode_t> registrator_t::getMode() {
	std::deque<dataT> stat = getStat();
	if (stat.size() != 3) throw exception_t(1, "getMode. Invalid stat.size() = " + boost::lexical_cast<size_t>(stat.size()));
	if (stat[0] != 0x55) throw exception_t(2, "getMode. Invalid start (0x55) bit");
	dataT s = stat[1];
	size_t mode = 0x0F & s;
	size_t sub_mode = s >> 4;
	return std::pair<mode::mode_t, mode::mode_t>(modeFromCode(mode), modeFromCode(sub_mode));
}

std::deque<dataT> registrator_t::getStat() {
	std::deque<dataT> cmd;
	cmd.push_back(0x45);
	try {
		std::deque<dataT> recv = process(cmd);
		return recv;
	}
	catch (exception& ex) {
		throw;// exception(13, "getStat. " + ex.fmsg());
	}
}

void registrator_t::changeMode(mode::mode_t new_mode) {
	std::pair<mode::mode_t, mode::mode_t> stat = getMode();
	if (stat.first == new_mode) return;
	outMode();
	if (new_mode == mode::SEL) return;
	stat = getMode();
	if (stat.first == mode::SEL && stat.second == mode::SEL) {
		inMode(new_mode);
	}
	else throw exception(14, "changeMode. Failed out from " + modeName(stat.first) + "." + modeName(stat.second));
}

std::string registrator_t::identificationDevice() {
	std::deque<dataT> cmd;
	cmd.push_back(0xA5);
	try {
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "identificationDevice. Device error: " + err);
		if (recv.size() < 12) throw exception(haveError(recv, err), "identificationDevice. Invalid recv.size() = " + boost::lexical_cast<size_t>(recv.size()));
		std::deque<dataT> name;
		std::copy(recv.begin()+11, recv.end(), std::back_inserter(name));
		std::string name_str;
		print_data(name);
		if (!fromKKM(name, name_str)) throw exception(7, "identificationDevice. Failed fromKKM");
		return name_str;
	}
	catch (exception& ex) {
		throw;// exception(15, "identificationDevice. " + ex.fmsg());
	}	
}

void registrator_t::inMode(mode::mode_t mode) {
	std::deque<dataT> cmd;
	cmd.push_back(0x56);
	cmd.push_back(mode);
	cmd.push_back(0x0);
	cmd.push_back(0x0);
	cmd.push_back(0x0);
	if (mode == mode::AFM) cmd.push_back(0x0);
	else cmd.push_back(0x30);
	try {
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "inMode. Device error: " + err);
	}
	catch (exception& ex) {
		throw;// exception(16, "inMode. " + ex.fmsg());
	}	
}

void registrator_t::bell() {
	std::deque<dataT> cmd;
	cmd.push_back(0x88);
	cmd.push_back(0xFE);
	cmd.push_back(0x90);
	cmd.push_back(10);
	try {
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "bell. Device error: " + err);
	}
	catch (exception& ex) {
		throw;// exception(17, "bell. " + ex.fmsg());
	}
}

void registrator_t::outMode() {
	std::deque<dataT> cmd;
	cmd.push_back(0x48);
	try {
		std::deque<dataT> recv = process(cmd);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "outMode. Device error: " + err);
	}
	catch (exception& ex) {
		throw;// exception(18, "outMode. " + ex.fmsg());
	}
}

bool registrator_t::print(format::font_t font, format::align_t align, size_t px, const std::string& str, bool check) {
	std::string dest = fixFormat(str, align, size_);
	return printSmartText(font, px, dest, false, check);
}

bool registrator_t::printSmartText(format::font_t h, size_t i, const std::string& str, bool wide, bool check) {
	std::deque<dataT> dstr;
	if (!toKKM(str, dstr, wide)) throw exception(8, "printSmartText. Failed toKKM");
	dstr.push_front(0x0);
	dstr.push_front(0x0);
	dstr.push_front(0x0);
	dstr.push_front(0x1);
	dstr.push_front(0x1);
	dstr.push_front(0x0);
	dstr.push_front(i);	
	dstr.push_front(h);	
	dstr.push_front(0x0);
	dstr.push_front(0x1);
	if (check) dstr.push_front(0x1);
	else dstr.push_front(0x0);
	dstr.push_front(0x87);
	try {
		std::deque<dataT> recv = process(dstr);
		std::string err;
		if (haveError(recv, err)) throw exception(haveError(recv, err), "printSmartText. Device error: " + err);
	}
	catch (exception& ex) {
		throw;// exception_t(3, "printSmartText. " + ex.fmsg());
	}
}

void registrator_t::printText(const std::string& str) {
	std::deque<dataT> dstr;
	if (!toKKM(str, dstr, false)) throw exception_t(1, "printText. Failed toKKM");
	dstr.push_front(0x4C);
	try {
		std::deque<dataT> recv = process(dstr);
		std::string err;
		if (haveError(recv, err)) throw exception_t(2, "printText. Device error: " + err);
	}
	catch (exception& ex) {
		throw;// exception(19, "printText. " + ex.fmsg());
	}
}

std::string fixFormat(const std::string& str, format::align_t align, size_t max_len) {
	std::string dest = str.substr(0, max_len);
	if (align == format::CENTER) {
		size_t free_space = max_len - dest.length();
		while (free_space > 0) {
			dest += " ";
			if (--free_space > 0) {
				dest = " " + dest;
				--free_space;
			}
		}
	}
	else if (align == format::RIGHT) {
		size_t free_space = max_len - dest.length();
		while (free_space > 0) {
			dest = " " + dest;
			--free_space;
		}
	}
	// else if (align == format::LEFT) {
	while (dest.length() < max_len) dest += " ";
	return dest;
	//}
}

std::deque<dataT> registrator_t::process(const std::deque<dataT>& data, bool response) {
	std::deque<dataT> packet = fiscal::pack(data);
#ifdef __DEBUG__
	of << "send: ";
	print_data(packet);
#endif
	std::deque<dataT> recv;
	if (response) {
		comm_.send(packet);
#ifdef __DEBUG__
	of << "sended" << std::endl;
#endif
		recv = comm_.receive();
#ifdef __DEBUG__
		of << "receive: ";
		print_data(recv);
#endif
	}
	return recv;
}



};

