// Copyright (C) 2014 mru@sisyphus.teil.cc

#include <memory>
#include <sstream>
#include <iostream>
#include <list>


#include <cstring>
#include <cassert>

#include "SerialPort.hpp"

#ifdef  __MINGW32__

#include <windows.h>
#include <tchar.h>

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
        ct.ReadTotalTimeoutConstant = 2000;
        ct.ReadTotalTimeoutMultiplier = 2000;
        ct.WriteTotalTimeoutMultiplier = 2000;
        ct.WriteTotalTimeoutConstant = 2000;
        if (!::SetCommTimeouts(fd, &ct)) {
            close();
            throw std::runtime_error(formatLastError("Failed to set COM timeouts"));
        }
    }

    std::list<std::string> enumeratePorts() {
        std::list<std::string> ports;

        const size_t bufsize = 100000;
        LPTSTR buf = new TCHAR[bufsize];
        ::QueryDosDevice(NULL, buf, bufsize);

        while (*buf != '\0') {
            if (_tcsncmp(buf, _T("COM"), 3) == 0 || _tcsncmp(buf, _T("\\\\.\\COM"), 7) == 0) {
                ports.push_back(buf);
            }
            buf += _tcslen(buf) + 1;
        }
        return ports;
    }
    HANDLE fd;
};

#else


#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


class SerialPort::SerialPortD {
  public:
    ssize_t read(std::vector<unsigned char> & buf) {
        errno = 0;
        return ::read(fd, &buf[0], buf.size());
    }
    ssize_t write(const std::vector<unsigned char> & buf) {
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
    std::list<std::string> enumeratePorts() {
        // http://stackoverflow.com/questions/2530096/how-to-find-all-serial-devices-ttys-ttyusb-on-linux-without-opening-them
        std::list<std::string> retval;
        const std::string sys_tty = "/sys/class/tty";
        struct dirent* de;
        DIR *ds = opendir (sys_tty.c_str());
        if (ds == nullptr) {
            throw std::runtime_error(formatLastError("Failed to enumerate " + sys_tty));
        }
        while((de=readdir(ds)) != nullptr) {
            if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) {
              continue;
            }

            std::string fn = sys_tty + "/" + de->d_name;
            struct stat st;
            if (::stat(fn.c_str(), &st) == -1) {
                throw std::runtime_error(formatLastError("stat " + fn));
            }
            if(!S_ISDIR(st.st_mode)) {
                continue;
            }

            std::string link(1024, '\0');
            if (readlink((fn + "/device/driver").c_str(), &link[0], 1024) == -1) {
                continue;
            }

            std::string driver = basename(link.c_str());
            if (driver != "cp210x") {
                continue;
            }

            std::string devfn = std::string("/dev/") + de->d_name;
            retval.push_back(devfn);
        }
        ::closedir(ds);
        return retval;
    }
  public:
    int fd;
};


#endif


SerialPort::SerialPort() : d(new SerialPortD()) {
}

void SerialPort::open(const std::string& filename) {
    std::cerr << "Opening serial port '" << filename << "'" << std::endl;
    d->open(filename);
}

void SerialPort::close() {
    d->close();
}

void SerialPort::read(std::vector<unsigned char>& buf) {
    if (!d->isopen()) {
        throw std::runtime_error("trying to read from closed serial port");
    }
    ssize_t nread = d->read(buf);
    if ( nread != (ssize_t)buf.size() ) {
        std::stringstream ss;
        ss << "Serial link: failed to read expected number of bytes"
          << " expected: " << (int) buf.size()
          << " actual: " << (int) nread;
        throw std::runtime_error(formatLastError(ss.str()));
    }
}

void SerialPort::write(const std::vector<unsigned char>& buf) {
    if (!d->isopen()) {
        throw std::runtime_error("trying to write on closed serial port");
    }
    ssize_t nwrite = d->write(buf);
    if ( nwrite != (ssize_t)buf.size() ) {
        throw std::runtime_error(formatLastError("Serial link: failed to write expected number of bytes"));
    }
}

std::list<std::string> SerialPort::enumeratePorts() {
    std::shared_ptr<SerialPortD> p(new SerialPortD());
    return p->enumeratePorts();
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

