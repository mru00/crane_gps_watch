// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#pragma once

#include <ostream>
#include <iostream>
#include <vector>
#include <iomanip>


#include "SerialLink.hpp"
#include "DataTypes.hpp"
#include "MemoryBlock.hpp"
#include "Callback.hpp"


class Watch {

  public:
    Watch(){
    }

    void parse() {
        parseBlock0();
    }

    void addRecipient(Callback* c) {
        br.addRecipient(c);
    }
  private:

    void parseGpsEle(GpsEle& l, WatchMemoryBlock::mem_it_t it) {
        l.ele = *it++;
        l.ele+= *it++ << 8;
    }
    void parseGpsLocation(GpsLocation& l, WatchMemoryBlock::mem_it_t it) {
        l.loc = *it++;
        l.loc+= *it++ << 8;
        l.loc+= *it++ << 16;
        l.loc+= *it++ << 24;
    }
    void parseGpsTimeUpd(GpsTimeUpd& t, WatchMemoryBlock::mem_it_t it) {
        t.mm = *it++;
        t.ss = *it++;
    }
    void parseGpsTime(GpsTime& t, WatchMemoryBlock::mem_it_t it) {
        t.YY = *it++;
        t.MM = *it++;
        t.DD = *it++;
        t.hh = *it++;
        t.mm = *it++;
        t.ss = *it++;
        //assert (t.YY == 14);
    }
    void parseSample(SampleInfo& si, WatchMemoryBlock::mem_it_t& it) {

        auto type = *it;
        si.fb = type;
        switch (type) {
          case 0x00: 
              {
                si.type = SampleInfo::Full;
                si.hr = *(it+24);
                parseGpsTime(si.time, it+2);
                parseGpsLocation(si.lon, it+8);
                parseGpsLocation(si.lat, it+12);
                parseGpsEle(si.ele, it+16);
                it += 25;
                break;
              }
          case 0x01: 
              {
                si.type = SampleInfo::Diff;
                si.hr = *(it+20);
                parseGpsTimeUpd(si.time_upd, it+2);
                parseGpsLocation(si.lon, it+4);
                parseGpsLocation(si.lat, it+8);
                parseGpsEle(si.ele, it+12);
                it += 21;
                break;
              }
          case 0x02: 
              {
                si.type = SampleInfo::TimeOnly;
                si.hr = 0;
                parseGpsTimeUpd(si.time_upd, it+1);
                it += 3;
                break;
              }
          case 0x03: 
              {
                si.type = SampleInfo::HrOnly;
                si.hr = *(it+7);
                parseGpsTime(si.time, it+1);
                it += 8;
                break;
              }
          case 0x80: 
              {
                si.type = SampleInfo::None;
                si.hr = *(it+24);
                parseGpsTime(si.time, it+2);
                parseGpsLocation(si.lon, it+8);
                parseGpsLocation(si.lat, it+12);
                parseGpsEle(si.ele, it+16);
                it += 25;
                break;
              }
          case 0xff: 
              {
                si.type = SampleInfo::End;
                it += 0;
                break;
              }
          default: 
              {
                assert(0 && type);
              }
        }
    }
    void parseWO(WorkoutInfo& wo, int first, int count) {
        
        WatchMemoryBlock cb(first, count);
        readBlock(cb);

        WatchMemoryBlock::mem_it_t it = cb.memory.begin();

        wo.nsamples = *it++;  // [0..1]
        wo.nsamples+= *it++ << 8;

        wo.lapcount = *it++;  // [2]

        // [3..8]
        std::vector<unsigned char> reverse_time(6);
        std::reverse_copy(it+3, it +3+6, reverse_time.begin());
        parseGpsTime(wo.start_time, reverse_time.begin());
        it += 6;

        it += 3; // total workout time [9..12]
        it += 1; // profile [15]

        it = cb.memory.begin() + 16;
        it += 4; // km [16..19]
        it += 2; // speed avg [20..21]
        it += 2; // speed max [22..23]
        it += 8; // ? [24..32]
        
        it = cb.memory.begin() + 32;
        wo.calories = *it++; // [32..33]
        wo.calories+= *it++ << 8;

        br.onWorkout(wo);

        it = cb.memory.begin() + 0x1000;
        GpsTime t;
        GpsLocation lon;
        GpsLocation lat;
        GpsEle ele;
        for (unsigned i =0; i< wo.nsamples;i++) {
            SampleInfo si;
            parseSample(si, it);
            if (si.type == SampleInfo::Full || si.type == SampleInfo::HrOnly) {
                t = si.time;
            }
            else if (si.type == SampleInfo::Diff || si.type == SampleInfo::TimeOnly) {
                si.time = (t = si.time_upd);
            }
            else if (si.type == SampleInfo::End) {
                break;
            }

            if (si.type == SampleInfo::Full) {
                lon = si.lon;
                lat = si.lat;
                ele = si.ele;
            }
            else if (si.type == SampleInfo::Diff) {
                si.lon = (lon += si.lon);
                si.lat = (lat += si.lat);
                si.ele = (ele += si.ele);
            }
            else {
                si.lon.loc = 0;
                si.lat.loc = 0;
                si.ele.ele = 0;
            }

            br.onSample(si);

        }
    }
    void parseBlock0() {
        sl.open("/dev/ttyUSB0");

        std::string version = sl.readVersion();
        WatchMemoryBlock mb(0, 1);
        readBlock(mb);
        WatchInfo wi;

        wi.version = version;
        br.onWatch(wi);

        WatchMemoryBlock::mem_it_t mem_it = mb.memory.begin();
        unsigned char cur;
        unsigned char first;
        mem_it += 0x100;
        for (;;) {
            first = *mem_it++;
            if (first == 0xff) {
                break;
            }
            
            while (*mem_it != 0xff) {
                cur = *mem_it++;
            }
            WorkoutInfo wo;
            parseWO(wo, first, 1+cur-first);
            br.onWorkoutEnd(wo);
            mem_it ++;
        }
        
        br.onWatchEnd(wi);
    }

    void readBlock(WatchMemoryBlock& b) {
        // read b.count bytes from block# b.id
        const unsigned readSize = 0x80;
        const unsigned rcount = b.blockSize / readSize;
        
        br.onReadBlocks(b.id, b.count);

        WatchMemoryBlock::mem_it_t mem_it = b.memory.begin();
        for (unsigned block = 0; block < b.count; block++) {
            unsigned block_start = (b.id+block)*b.blockSize;
            br.onReadBlock(b.id+block, block_start);

            for (unsigned nbyte = 0; nbyte < rcount; nbyte++) {
                sl.readMemory(block_start + nbyte*readSize, readSize, &*mem_it);
                mem_it += readSize;
            }
        }
        //b.dump();
    }

  private:
    SerialLink sl;
    Broadcaster br;
};
