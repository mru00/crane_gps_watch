// Copyright (C) 2014 mru@sisyphus.teil.cc
//
//
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <algorithm>

#include "MemoryBlock.hpp"


WatchMemoryBlock::WatchMemoryBlock(const unsigned id, const unsigned count) 
: id(id)
, count(count)
  , memory(count*blockSize, 0) 
{
};


void WatchMemoryBlock::dump() {
    std::cerr << "memory:" << memory.size()  <<std::endl;
    mem_it_t it = memory.begin();
    int i = 0;
    while (it != memory.end()) {
        std::cerr << std::setw(2) << std::setfill('0') <<std::hex << (int)*it++ << " ";
        if ( ++i % 16 == 0 )
          std::cerr << std::endl;
    }
    std::cerr << std::endl;
}
