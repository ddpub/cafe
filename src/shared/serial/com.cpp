
#include "com.hpp"
#include <ksi_include/ksi_exceptions.hpp>
#include <boost/lexical_cast.hpp>


#ifdef _UNIX_BUILD_
#include <fcntl.h>
#include <cstring>

com_t::com_t(const std::string& port, int rate)
{
	std::string com_str;
	if (port[0] != '/')
  		com_str = "/dev/ttyS" + boost::lexical_cast<std::string>(boost::lexical_cast<int>(port[3])-1);
	else
		com_str = port;

  if((fd = open(com_str.c_str(), O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK)) == -1)
	THROW_MESSAGE(0, com_str + " serial port could not be initialized.", "com_t::com_(const std::string&, int)");

  tcgetattr(fd, &oldios);

  termios t;
  memset(&t, 0, sizeof(t));

  t.c_cflag |= CS8;

  t.c_iflag = IGNPAR;

  t.c_cflag |= (CLOCAL | CREAD | HUPCL);

  cfsetispeed(&t, rate);
  cfsetospeed(&t, rate);

  tcflush(fd, TCIOFLUSH);

  if(tcsetattr(fd, TCSANOW, &t) < 0) {
    close(fd);
    THROW_MESSAGE(1, com_str + " serial port could not be initialized.", "com_t::com_(const std::string&, int)");
  }
}

com_t::com_t(int comnum, int speed, bool parity) {
  std::string com_str = "/dev/ttyS" + boost::lexical_cast<std::string>(comnum -1);

  if((fd = open(com_str.c_str(), O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK)) == -1)
    THROW_MESSAGE(0, com_str + " serial port could not be initialized.", "com_t::com_(const std::string&, int, bool)");

  tcgetattr(fd, &oldios);

  termios t;
  memset(&t, 0, sizeof(t));

  t.c_cflag |= CS8;

  if(parity)
    t.c_cflag |= PARENB;
  else
    t.c_iflag = IGNPAR;

  t.c_cflag |= (CLOCAL | CREAD | HUPCL);

  cfsetispeed(&t, speed);
  cfsetospeed(&t, speed);

  tcflush(fd, TCIOFLUSH);

  if(tcsetattr(fd, TCSANOW, &t) < 0) {
    close(fd);
    THROW_MESSAGE(1, com_str + " serial port could not be initialized.", "com_t::com_(const std::string&, int, bool)");
  }
}

com_t::~com_t() {
  tcsetattr(fd, TCSANOW, &oldios);
  close(fd);
}

/*
void com_t::write(const char *ptr, int size) {
  for(int i = 0; i < size; i++) {
    int res = ::write(fd, ptr + i, 1);
    if(res == -1)
    	THROW_MESSAGE(0, "write failed", "void com_t::write(const void*, int)");
  }
}

void com_t::read(char *ptr, int size) {
	for(int i = 0; i < size; ++i) {
		int res = ::read(fd, ptr + i, 1);
		if(res == -1)
			THROW_MESSAGE(0, "write failed", "void com_t::read(const void*, int)");
	}
}
*/

void com_t::purge() {
}

#endif // _UNIX_BUILD_

#ifdef _WIN32_BUILD_
#include <windows.h>

com_t::com_t(const std::string& port, int rate_, int mode): hCom(0), rate_(rate_)
{
	if (mode != 3) {
	hCom = CreateFile(port.c_str(), GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

	if (hCom == INVALID_HANDLE_VALUE|| hCom == 0) 
		THROW_MESSAGE(0, "serial device "+port+" open error", "com_t::com_t(const std::string&, int)");

	DCB dcb;
	if (!GetCommState(hCom, &dcb))
		THROW_MESSAGE(0, "serial device "+port+" get state error", "com_t::com_t(const std::string&, int)");
	//if (mode == 2) GetCommState(hCom, &dcb);	
		
	dcb.BaudRate = rate_;

	dcb.fBinary = TRUE;

	dcb.fParity = FALSE;
	
	if (mode != 2) 
	{
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = TRUE;
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
		dcb.fDsrSensitivity = FALSE;
		dcb.fTXContinueOnXoff = FALSE;
	}
	
	dcb.fOutX = FALSE;
	if (mode == 2) dcb.fInX = FALSE;	
	dcb.fErrorChar = FALSE;
	dcb.fNull = FALSE;
	if (mode != 2) dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fAbortOnError = FALSE;

	dcb.ByteSize = 8;
	dcb.Parity   = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	
	if (!SetCommState(hCom, &dcb))
		THROW_MESSAGE(0, "serial device "+port+" set state error", "com_t::com_t(const std::string&, int)");
	
	if (mode == 2) if (!SetupComm(hCom, 16384, 16384)) return;


	COMMTIMEOUTS  ct;
	ZeroMemory(&ct, sizeof(COMMTIMEOUTS));
	ct.ReadIntervalTimeout = MAXDWORD;
	ct.ReadTotalTimeoutMultiplier = ct.ReadTotalTimeoutConstant = 0;
	ct.WriteTotalTimeoutMultiplier = 10;
	ct.WriteTotalTimeoutConstant = 10;
	SetCommTimeouts(hCom, &ct);
	return;
	}

	hCom = CreateFile(port.c_str(), GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

	if (hCom == INVALID_HANDLE_VALUE|| hCom == 0) 
		THROW_MESSAGE(0, "serial device "+port+" open error", "com_t::com_t(const std::string&, int)");

	DCB dcb;
	if (!GetCommState(hCom, &dcb))
		THROW_MESSAGE(0, "serial device "+port+" get state error", "com_t::com_t(const std::string&, int)");
	
	dcb.BaudRate = rate_;	
	
	if (mode == 2) {
		dcb.ByteSize = 8;
		dcb.fBinary = TRUE;
		dcb.fParity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;		
		dcb.fOutxDsrFlow = TRUE;
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
		dcb.fOutxCtsFlow = FALSE;
		dcb.fDsrSensitivity = FALSE;
		dcb.fTXContinueOnXoff = FALSE;
		dcb.fOutX = FALSE;
		dcb.fErrorChar = FALSE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fAbortOnError = FALSE;
		dcb.fInX = FALSE;
		dcb.fNull = FALSE;
		dcb.Parity   = NOPARITY;
	}
	else if (mode == 3) {
		dcb.fBinary = TRUE;
		dcb.fParity = FALSE;
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fDtrControl = DTR_CONTROL_DISABLE;
		dcb.fDsrSensitivity = FALSE;
		dcb.fTXContinueOnXoff = FALSE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		dcb.fErrorChar = FALSE;
		dcb.fNull = FALSE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fAbortOnError = FALSE;
		dcb.XonLim = 2048;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
	}
	else {	
		dcb.ByteSize = 8;
		dcb.fBinary = TRUE;
		dcb.fParity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;		
		dcb.fOutxDsrFlow = TRUE;
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
		dcb.fOutxCtsFlow = FALSE;
		dcb.fDsrSensitivity = FALSE;
		dcb.fTXContinueOnXoff = FALSE;
		dcb.fOutX = FALSE;
		dcb.fErrorChar = FALSE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fAbortOnError = FALSE;
		dcb.fNull = FALSE;
		dcb.Parity   = NOPARITY;
	}

	if (!SetCommState(hCom, &dcb))
		THROW_MESSAGE(0, "serial device "+port+" set state error", "com_t::com_t(const std::string&, int)");
	
	if (mode == 2) if (!SetupComm(hCom, 16384, 16384)) return;

	COMMTIMEOUTS  ct;
	ZeroMemory(&ct, sizeof(COMMTIMEOUTS));
	ct.ReadIntervalTimeout = MAXDWORD;
	ct.ReadTotalTimeoutMultiplier = ct.ReadTotalTimeoutConstant = 0;
	ct.WriteTotalTimeoutMultiplier = 100;//MAXDWORD;//100;
	ct.WriteTotalTimeoutConstant = 100;//MAXDWORD;//100;
	SetCommTimeouts(hCom, &ct);
}

void com_t::set_timeouts(size_t ReadIntervalTimeout, size_t ReadTotalTimeoutMultiplier, size_t ReadTotalTimeoutConstant) {
	COMMTIMEOUTS ct;
	ZeroMemory(&ct, sizeof(COMMTIMEOUTS));
	ct.ReadIntervalTimeout = ReadIntervalTimeout;
	ct.ReadTotalTimeoutMultiplier = ReadTotalTimeoutMultiplier;
	ct.ReadTotalTimeoutConstant = ReadTotalTimeoutConstant;
	ct.WriteTotalTimeoutMultiplier = 100;//MAXDWORD;
	ct.WriteTotalTimeoutConstant = 100;//MAXDWORD;
	SetCommTimeouts(hCom, &ct);
}

com_t::~com_t()
{
	try {
		CloseHandle(hCom);
	}
	catch (...) {
	}
}

void com_t::purge()
{
	PurgeComm(hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
}

#endif //_WIN32_BUILD_

