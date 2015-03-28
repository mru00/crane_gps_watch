// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc
//
//
//

#pragma once

#include <fstream>


#include "Callback.hpp"

class ImageWriter : public Callback {
  public:
    ImageWriter(const std::string&);
    virtual ~ImageWriter() {}
    void onReadBlock(int id, int addr, unsigned char* data, size_t size) override;

  private:
    std::ofstream f;
};
