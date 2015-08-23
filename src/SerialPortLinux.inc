// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc


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

    std::string formatLastError(const std::string& msg) {
        std::ostringstream ss;
        ss << msg << ": " << strerror(errno);
        return ss.str();
    }

  public:
    int fd;
};

