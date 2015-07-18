// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#ifndef __WATCH_HPP
#define __WATCH_HPP

#include <memory>

#include "DeviceInterface.hpp"
#include "DataTypes.hpp"
#include "MemoryBlock.hpp"
#include "Callback.hpp"


class Watch {

  public:
    Watch(std::shared_ptr<DeviceInterface> device);
    void addRecipient(std::shared_ptr<Callback> c);
    void parse();
    void clearWorkouts();
    void downloadEPO(const std::string& epo_fn);
  private:

    void parseGpsEle(GpsEle& l, WatchMemoryBlock::mem_it_t it);
    void parseGpsLocation(GpsLocation& l, WatchMemoryBlock::mem_it_t it);
    void parseGpsTimeUpd(GpsTimeUpd& t, WatchMemoryBlock::mem_it_t it);
    void parseGpsTime(GpsTime& t, WatchMemoryBlock::mem_it_t it, unsigned timezone);
    void parseSample(WatchInfo& wi, SampleInfo& si, WatchMemoryBlock::mem_it_t& it);
    void parseWO(WatchInfo& wi, int first, int count);
    void parseToc(Toc& toc, WatchMemoryBlock::mem_it_t it);
    void parseProfileNames(std::vector<std::string>&, WatchMemoryBlock::mem_it_t);
    void parseBlock0();

    void readBlock(WatchMemoryBlock& b);
    void writeBlock(WatchMemoryBlock& b);

  private:
    Broadcaster br;
    std::shared_ptr<DeviceInterface> device;
};


#endif // __WATCH_HPP
