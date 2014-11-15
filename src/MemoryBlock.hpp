// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//

#pragma once


#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <algorithm>



class Watch;

class WatchMemoryBlock {
  private:
    WatchMemoryBlock(const unsigned id, const unsigned count) 
      : id(id)
        , count(count)
        , memory(count*blockSize, 0) 
    {
    };

    WatchMemoryBlock(const WatchMemoryBlock&) = delete;

    void dump() {
        std::cerr << "memory:" << memory.size()  <<std::endl;
        mem_it_t it = memory.begin();
        while (it != memory.end()) {
            std::cerr << std::hex << (int)*it++ << ",";
        }
        std::cerr << std::endl;
    }
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
