// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc


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
            //std::cerr << "testing dosdevice " << buf << std::endl;
            if (_tcsncmp(buf, _T("COM"), 3) == 0 || _tcsncmp(buf, _T("\\\\.\\COM"), 7) == 0) {
                ports.push_back(buf);
            }
            buf += _tcslen(buf) + 1;
        }
        return ports;
    }

    std::string formatLastError(const std::string& msg) {

        std::ostringstream ss;
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
        return ss.str();
    }

    HANDLE fd;
};

