// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc
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
    void readMemory(unsigned addr, unsigned count, unsigned char* it) override;
    void writeMemory(unsigned addr, unsigned count, unsigned char* it) override;
    std::string readVersion() override;
    std::string readVersion2() override;
    void clearFlash1() override;
    void clearFlash2(unsigned int segment) override;
    void setEpoEol(unsigned char, unsigned char, unsigned char) override {}

  private:
    std::ifstream from_watch;
    std::ofstream to_watch;
};
