// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc

#include <memory>
#include <sstream>
#include <iostream>
#include <list>
#include <stdexcept>


#include <cstring>
#include <cassert>

#include "SerialPort.hpp"

#ifdef  __MINGW32__
#include "SerialPortWindows.inc"
#elif TARGET_MAC_OS
#include "SerialPortMacOSX.inc"
#elif __linux__
#include "SerialPortLinux.inc"
#else
#error Unsupported Platform. Please file a bug report with details about your compiler and platform
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
        throw std::runtime_error(d->formatLastError(ss.str()));
    }
}

void SerialPort::write(const std::vector<unsigned char>& buf) {
    if (!d->isopen()) {
        throw std::runtime_error("trying to write on closed serial port");
    }
    ssize_t nwrite = d->write(buf);
    if ( nwrite != (ssize_t)buf.size() ) {
        throw std::runtime_error(d->formatLastError("Serial link: failed to write expected number of bytes"));
    }
}

std::list<std::string> SerialPort::enumeratePorts() {
    std::shared_ptr<SerialPortD> p(new SerialPortD());
    return p->enumeratePorts();
}


