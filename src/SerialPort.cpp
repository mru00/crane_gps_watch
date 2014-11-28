// Copyirght (C) 2014 mru@sisyphus.teil.cc

#include <memory>
#include <sstream>
#include <iostream>
#include <cstring>
#include "SerialPort.hpp"

#ifdef  __MINGW32__

#include <windows.h>

class SerialPort::SerialPortD {
  public:
    HANDLE fd;
};

SerialPort::SerialPort()
{

  d->fd = nullptr;
}

void SerialPort::open(const std::string& filename) {
    std::cerr << "opening com port at " << filename << std::endl;
    d->fd = ::CreateFile(filename.c_str(), GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

    if (d->fd == INVALID_HANDLE_VALUE) {
        throw std::runtime_error(formatLastError("Open COM port"));
    }

    DCB dcb;
    dcb.DCBlength = sizeof(DCB);

    if (!::GetCommState(d->fd, &dcb)) {
        throw std::runtime_error(formatLastError("Failed to get COM state"));
    }

    dcb.BaudRate = 115200;
    dcb.ByteSize = 8;
    dcb.Parity = 0;
    dcb.StopBits = ONESTOPBIT;

    if (!::SetCommState(d->fd, &dcb)) {
        throw std::runtime_error(formatLastError("Failed to set COM state"));
    }


    COMMTIMEOUTS ct;
    if (!::GetCommTimeouts(d->fd, &ct)) {
        throw std::runtime_error(formatLastError("Failed to get COM timeouts"));
    }
    ct.ReadTotalTimeoutConstant = 20000;
    ct.ReadTotalTimeoutMultiplier = 20000;
    ct.WriteTotalTimeoutMultiplier = 20000;
    ct.WriteTotalTimeoutConstant = 20000;
    if (!::SetCommTimeouts(d->fd, &ct)) {
        throw std::runtime_error(formatLastError("Failed to set COM timeouts"));
    }
}

void SerialPort::close() {
    ::CloseHandle(d->fd);
    d->fd = nullptr;
}

void SerialPort::read(std::vector<unsigned char>& buf) {
    DWORD n;
    ReadFile(d->fd, &buf[0], buf.size(), &n, 0);
    if ( (size_t)n != buf.size() ) {
        throw std::runtime_error(formatLastError("Serial link: failed to read expected number of bytes"));
    }
}

void SerialPort::write(const std::vector<unsigned char>& buf) {
    DWORD n;
    WriteFile(d->fd, &buf[0], buf.size(), &n, 0);
    if ( (size_t)n != buf.size() ) {
        throw std::runtime_error(formatLastError("Serial link: failed to write expected number of bytes"));
    }
}

std::vector<std::string> SerialPort::enumeratePorts() {
    std::vector<std::string> ports;

    /*
    const size_t bufsize = 100000;
    LPTSTR buf = new TCHAR[bufsize];
    ::QueryDosDevice(NULL, buf, bufsize);

    while (*buf != '\0') {
        if (_tcsncmp(buf, L"COM", 3) == 0) {
            ports.push_back(buf);
        }
        buf += _tcslen(buf) + 1;
    }
*/
    return ports;
}

#else


#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


class SerialPort::SerialPortD {
  public:
    int fd;
};

SerialPort::SerialPort()
{
  d->fd = -1;
}

void SerialPort::open(const std::string& filename) {

    d->fd = ::open (filename.c_str(), O_RDWR);
    if (d->fd == -1) {
        throw std::runtime_error(formatLastError("failed to open serial port '" + filename + "'"));
    }

    struct termios options;

    tcgetattr(d->fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    cfmakeraw(&options);
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 50;

    if(tcsetattr(d->fd, TCSANOW, &options) != 0) {
        throw std::runtime_error(formatLastError("failed to configure serial port '" + filename + "'"));
    }
}

void SerialPort::close() {
    ::close(d->fd);
    d->fd = -1;
}

void SerialPort::read(std::vector<unsigned char>& buf) {
    ssize_t n = ::read(d->fd, &buf[0], buf.size());
    if ( (size_t)n != buf.size() ) {
        throw std::runtime_error(formatLastError("Serial link: failed to read expected number of bytes"));
    }
}

void SerialPort::write(const std::vector<unsigned char>& buf) {
    ssize_t n;
    n = ::write(d->fd, &buf[0], buf.size());
    if ( (size_t)n != buf.size() ) {
        throw std::runtime_error(formatLastError("Serial link: failed to write expected number of bytes"));
    }
}
std::vector<std::string> SerialPort::enumeratePorts() {
    std::vector<std::string> ports;

    return ports;
}
#endif



std::string formatLastError(const std::string& msg) {

    std::ostringstream ss;
#ifdef __MINGW32__
    LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        ::GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    ss << msg << ": " << (char*)lpMsgBuf;
    LocalFree(lpMsgBuf);
#else
    ss << msg << ": " << strerror(errno);
#endif
    return ss.str();
}

