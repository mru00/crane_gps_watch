// Copyright (C) 2014 mru@sisyphus.teil.cc
//
//
//

#include <iostream>
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
    while (it != memory.end()) {
        std::cerr << std::hex << (int)*it++ << ",";
    }
    std::cerr << std::endl;
}
