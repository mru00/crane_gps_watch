// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc


#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <libgen.h>


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
        std::list<std::string> retval;
 #warning serial port enumeration not yet implemented for Mac OS X
        return retval;
    }

    std::string formatLastError(const std::string& msg) {
        std::ostringstream ss;
        ss << msg << ": " << strerror(errno);
        return ss.str();
    }

  public:
    int fd;
};


