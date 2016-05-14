// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc
//
// 
//

#pragma once


#include <string>


class DeviceInterface {
    DeviceInterface(const DeviceInterface&) = delete;
  public:
    DeviceInterface() {}
    virtual ~DeviceInterface() {}

    // opcode 0x12, 0x13
    virtual void readMemory(unsigned addr, unsigned count, unsigned char* it) = 0;

    // opcode 0x16, 0x17
    virtual void writeMemory(unsigned addr, unsigned count, unsigned char* it) = 0;

    // opcode 0x10, 0x11
    virtual std::string readVersion() = 0;

    // opcode 0x2c, 0x2d
    virtual std::string readVersion2() = 0;

    // opcode 0x24, 0x25
    virtual void clearFlash1() = 0;

    // opcode 0x14, 0x15
    // 3 byte segment address
    virtual void clearFlash2(unsigned int segment) = 0;

    virtual void setEpoEol(unsigned char, unsigned char, unsigned char) = 0;
};
