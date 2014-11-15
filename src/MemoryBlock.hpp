// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//

#pragma once

#include <vector>

class Watch;

class WatchMemoryBlock {
  private:
    WatchMemoryBlock(const unsigned id, const unsigned count);
    WatchMemoryBlock(const WatchMemoryBlock&) = delete;
    void dump();

  public:
    typedef unsigned char byte_t;
    typedef std::vector<byte_t> mem_t;
    typedef mem_t::iterator mem_it_t;
    static const unsigned blockSize = 0x1000;

  private:
    const unsigned id; 
    const unsigned count;
    mem_t memory;
    friend class Watch;
};
