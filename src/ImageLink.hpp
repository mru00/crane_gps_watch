// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// 
//

#pragma once


#include <string>
#include <fstream>

#include "DeviceInterface.hpp"

class ImageLink : public DeviceInterface {
  public:
    ImageLink(const std::string& filename);
    virtual ~ImageLink() {};
    virtual void readMemory(unsigned addr, unsigned count, unsigned char* it);
    virtual std::string readVersion();

  private:
    std::ifstream f;
};
