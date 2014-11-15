// Copyright (C) 2014 mru@sisyphus.teil.cc
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

    virtual void readMemory(unsigned addr, unsigned count, unsigned char* it) = 0;
    virtual std::string readVersion() = 0;
};
