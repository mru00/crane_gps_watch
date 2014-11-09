// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#ifndef __WATCH_HPP
#define __WATCH_HPP

#include "SerialLink.hpp"
#include "DataTypes.hpp"
#include "MemoryBlock.hpp"
#include "Callback.hpp"


class Watch {

  public:
    Watch(const std::string& device);
    void parse();
    void addRecipient(Callback* c);
  private:

    void parseGpsEle(GpsEle& l, WatchMemoryBlock::mem_it_t it);
    void parseGpsLocation(GpsLocation& l, WatchMemoryBlock::mem_it_t it);
    void parseGpsTimeUpd(GpsTimeUpd& t, WatchMemoryBlock::mem_it_t it);
    void parseGpsTime(GpsTime& t, WatchMemoryBlock::mem_it_t it);
    void parseSample(SampleInfo& si, WatchMemoryBlock::mem_it_t& it);
    void parseWO(WorkoutInfo& wo, int first, int count);
    void parseBlock0();
    void readBlock(WatchMemoryBlock& b);

  private:
    SerialLink sl;
    Broadcaster br;
    std::string device;
};


#endif // __WATCH_HPP
