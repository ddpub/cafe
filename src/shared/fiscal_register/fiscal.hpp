#ifndef DMITRY_DERGACHEV_23_03_2009_FISCAL_H
#define DMITRY_DERGACHEV_23_03_2009_FISCAL_H

#include <deque>
#include <algorithm>
#include <numeric>
#include <ksi_include/ksi_exceptions.hpp>
#include <boost/lexical_cast.hpp>
#include <serial/com.hpp>
#include <WinDef.h>
#include <string>
#include "fiscal_exception.hpp"

namespace fiscal {

namespace proto {

	namespace segment {
		enum control_character_t {
			ENQ = 0x05, // Запрос
			ACK = 0x06, // Подтверждение
			STX = 0x02, // Начало текста
			ETX = 0x03, // Конец текста
			EOT = 0x04, // Конец передачи
			NAK = 0x15, // Отрицание
			DLE = 0x10 // Экранирование управляющих символов
		};
	};
	
	namespace timeout {
		enum time_constant_t {
			T1 = 500,
			T2 = 2000,
			T3 = 500,
			T4 = 500,
			T5 = 10000,
			T6 = 500,
			T7 = 500,
			T8 = 1000,
			MAX = MAXDWORD
		};
	};	
};

namespace mode {
	enum mode_t {
		SEL	 = 0, // Выбор
		REG  = 1, // Регистрации
		REP1 = 2, // Отчетов без гашения
		REP2 = 3, // Отчетов с гашением
		PRO  = 4, // Программирования
		AFM  = 5, // Доступ к ФП
		APL  = 6, // Доступ к ЭКЛЗ
		INV
	};
};

namespace receipt_type {
	enum receipt_type_t {
		SALE = 1, //  Продажа
		REFUND = 2, // Возврат
		CANCEL = 3 // Аннулирование			
	};
};
	
namespace payment_type {
	enum payment_type_t {
		CASH = 0x1 // Наличные
	};
};
	
namespace sum_type {
	enum sum_type_t {
		SALE = 1 // Итог по продажам
	};
};

namespace device {
	enum constant_t {
		MAX_DATA_CNT = 66
	};
};

namespace format {
	enum align_t {
		LEFT,
		CENTER,
		RIGHT
	};
	enum font_t {
		NORMAL = 0,
		Hx2 = 1
	};
};

typedef unsigned char dataT;

class communicator_t {
public:
	explicit communicator_t(com_t& com_): com_(com_) {}
public:
	void send(const std::deque<dataT>& data);
	std::deque<dataT> receive(size_t cmd_timeout=proto::timeout::T5);
private:
	size_t sendByte(const dataT&);
	size_t recvByte(dataT& response, size_t timeout);
	size_t sendVector(const std::deque<dataT>&);
	std::deque<dataT> recvVector(size_t);
private:
	com_t& com_;
private:
	communicator_t(const communicator_t&);
	communicator_t& operator=(const communicator_t&);
};

class registrator_t {
public:
	explicit registrator_t(com_t& com_, size_t size_=32): comm_(com_), size_(size_) {
		std::string dev = identificationDevice();
		if (dev != "FPrint-03K") throw exception(666, "Invalid Registrator");
	}
	~registrator_t() {}
public:
public:
	std::string identificationDevice();
	void bell();
public:
public:
	void print_fl();
	void setFooter(const std::string& str);
	bool open_shift(const std::string& name, bool);
	void Zreport();
	bool open_receipt(receipt_type::receipt_type_t, bool);
	void cancel_receipt();
	bool sale(double, size_t, bool);
	bool refund(double, size_t, bool);
	bool cancel(double, size_t, bool);
	double final_pay(payment_type::payment_type_t, double, bool);
	bool close_receipt(payment_type::payment_type_t, bool);
	bool print(format::font_t, format::align_t, size_t, const std::string&, bool);
private:
	void setTable(size_t, size_t, size_t, const std::deque<dataT>&);
	std::deque<dataT> readTable(size_t, size_t, size_t);
	bool cash_in(double, bool);
	bool cash_out(double, bool);	
	bool printSmartText(format::font_t, size_t, const std::string&, bool, bool);
	void changeMode(mode::mode_t);
	void printText(const std::string& str);
	void outMode();
	void inMode(mode::mode_t);
	std::pair<mode::mode_t, mode::mode_t> getMode();
	std::pair<size_t, size_t> getRawMode();
	std::deque<dataT> getStat();
private:
	std::deque<dataT> process(const std::deque<dataT>&, bool response=true);
private:
	communicator_t comm_;
	size_t size_;
private:
	registrator_t(const registrator_t&);
	registrator_t& operator=(const registrator_t&);
};

// STX<data[N]>ETX<CRC>

dataT CRC(const std::deque<dataT>& data);
std::deque<dataT> mask_data(const std::deque<dataT>& data);
std::deque<dataT> unmask_data(const std::deque<dataT>& masked);
std::deque<dataT> pack(const std::deque<dataT>& data);
std::deque<dataT> unpack(const std::deque<dataT>& packet);
int haveError(const std::deque<dataT>& response, std::string& err);

mode::mode_t modeFromCode(size_t);
std::string modeName(mode::mode_t);

std::deque<dataT> make_money(double val);
std::deque<dataT> make_weight(double val);
double fromBCD(const std::deque<dataT>&);

bool fromKKM(dataT data, char& ch);
bool toKKM(char ch, dataT& data);
bool fromKKM(const std::deque<dataT>& data, std::string& str);
bool toKKM(const std::string& str, std::deque<dataT>& data, bool wide);

std::string fixFormat(const std::string& str, format::align_t align=format::LEFT, size_t max_len=32);


};

void print_byte(fiscal::dataT byte);
void print_data(const std::deque<fiscal::dataT>& data);

#endif
