// Copyright (C) 2014 mru@sisyphus.teil.cc

#include <memory>
#include <sstream>
#include <iostream>


#include <cstring>
#include <cassert>

#include "SerialPort.hpp"

#ifdef  __MINGW32__

#include <windows.h>

class SerialPort::SerialPortD {
  public:
    size_t read(std::vector<unsigned char> & buf) {
        DWORD n;
        ReadFile(fd, &buf[0], buf.size(), &n, 0);
        return (size_t)n;
    }
    size_t write(const std::vector<unsigned char> & buf) {
        DWORD n;
        WriteFile(fd, &buf[0], buf.size(), &n, 0);
        return n;
    }
    bool isopen() {
        return fd != nullptr;
    }
    void close() {
        ::CloseHandle(fd);
        fd = nullptr;
    }
    void open(const std::string& filename) {
        fd = ::CreateFile(filename.c_str(), GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

        if (fd == INVALID_HANDLE_VALUE) {
            fd = nullptr;
            throw std::runtime_error(formatLastError("Open COM port"));
        }

        DCB dcb;
        dcb.DCBlength = sizeof(DCB);

        if (!::GetCommState(fd, &dcb)) {
            close();
            throw std::runtime_error(formatLastError("Failed to get COM state"));
        }

        dcb.BaudRate = 115200;
        dcb.ByteSize = 8;
        dcb.Parity = 0;
        dcb.StopBits = ONESTOPBIT;

        if (!::SetCommState(fd, &dcb)) {
            close();
            throw std::runtime_error(formatLastError("Failed to set COM state"));
        }


        COMMTIMEOUTS ct;
        if (!::GetCommTimeouts(fd, &ct)) {
            close();
            throw std::runtime_error(formatLastError("Failed to get COM timeouts"));
        }
        ct.ReadTotalTimeoutConstant = 20000;
        ct.ReadTotalTimeoutMultiplier = 20000;
        ct.WriteTotalTimeoutMultiplier = 20000;
        ct.WriteTotalTimeoutConstant = 20000;
        if (!::SetCommTimeouts(fd, &ct)) {
            close();
            throw std::runtime_error(formatLastError("Failed to set COM timeouts"));
        }
    }
    HANDLE fd;
};

#else


#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


class SerialPort::SerialPortD {
  public:
    size_t read(std::vector<unsigned char> & buf) {
        return ::read(fd, &buf[0], buf.size());
    }
    size_t write(const std::vector<unsigned char> & buf) {
        return ::write(fd, &buf[0], buf.size());
    }
    bool isopen() {
        return fd != -1;
    }
    void close() {
        ::close(fd);
        fd = -1;
    }
    void open(const std::string& filename) {
    fd = ::open (filename.c_str(), O_RDWR);
    if (fd == -1) {
        throw std::runtime_error(formatLastError("failed to open serial port '" + filename + "'"));
    }

    struct termios options;

    if (tcgetattr(fd, &options) != 0) {
        close();
        throw std::runtime_error(formatLastError("failed to configure serial port '" + filename + "'"));
    }

    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    cfmakeraw(&options);
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 50;

    if(tcsetattr(fd, TCSANOW, &options) != 0) {
        close();
        throw std::runtime_error(formatLastError("failed to configure serial port '" + filename + "'"));
    }
    }
  public:
    int fd;
};


#endif


SerialPort::SerialPort() : d(new SerialPortD()) { }

void SerialPort::open(const std::string& filename) {
    std::cerr << "opening com port at " << filename << std::endl;
    d->open(filename);

}

void SerialPort::close() {
    d->close();
}

void SerialPort::read(std::vector<unsigned char>& buf) {
    if (!d->isopen()) {
        throw std::runtime_error("trying to read from closed serial port");
    }
    if ( d->read(buf) != buf.size() ) {
        throw std::runtime_error(formatLastError("Serial link: failed to read expected number of bytes"));
    }
}

void SerialPort::write(const std::vector<unsigned char>& buf) {
    if (!d->isopen()) {
        throw std::runtime_error("trying to write on closed serial port");
    }
    if ( d->write(buf) != buf.size() ) {
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

