// Copyright (C) 2014 mru@sisyphus.teil.cc

// os independent serial port abstraction

#pragma once

#include <memory>
#include <vector>
#include <list>
#include <string>


class SerialPort {
  public:
    SerialPort();
    bool isOpen();
    void open(const std::string& filename);
    void close();
    void read(std::vector<unsigned char>& buf);
    void write(const std::vector<unsigned char>& buf);

    static std::list<std::string> enumeratePorts();
  private:
    class SerialPortD;
    std::auto_ptr<SerialPortD> d;
};

std::string formatLastError(const std::string& msg);

