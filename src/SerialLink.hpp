// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// simplistic interface to the gps watch
//

#pragma once


#include <vector>
#include <string>

#include "DeviceInterface.hpp"

class SerialLink : public DeviceInterface {
  public:
    SerialLink(const std::string& filename);
    virtual ~SerialLink() {};
    virtual void readMemory(unsigned addr, unsigned count, unsigned char* it);
    virtual std::string readVersion();

  private:

    void sendCommand(unsigned char opcode, std::vector<unsigned char> payload);
    void receiveReply(unsigned char& opcode, std::vector<unsigned char>& target);
    unsigned char expect(unsigned char val);
    void write(std::vector<unsigned char>& buf);
    unsigned char read();
    void read(std::vector<unsigned char>& buf);
    unsigned short checksum(unsigned char opcode, std::vector<unsigned char> payload);

  private:
    std::string filename;
    int fd;
};
