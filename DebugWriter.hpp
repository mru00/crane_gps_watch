// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//

#pragma once

#include <iostream>

#include "Callback.hpp"

class DebugWriter : public Callback {

  public:
    virtual void onWatch(const WatchInfo &i) {
        std::cout << "watch version: " << i.version << std::endl;
    }
    virtual void onWatchEnd(const WatchInfo &) {
        std::cout << "watch end" << std::endl;
    }
    virtual void onWorkout(const WorkoutInfo &i)  {
        std::cout << " workout info t=" << i.start_time << std::endl;
    }
    virtual void onWorkoutEnd(const WorkoutInfo &)  {
        std::cout << " workout end" << std::endl;
    }
    virtual void onTrack(const TrackInfo &i)  {
        std::cout << "  track info " << std::endl;
    }
    virtual void onTrackEnd(const TrackInfo &)  {
        std::cout << "  track end" << std::endl;
    }
    virtual void onSample(const SampleInfo &i) {
        std::cout << "   sample info: " 
          << " idx_wo: " << (int)i.idx_wo
          << " idx_track: " << (int)i.idx_track
          << " fb: " << (int)i.fb
          << " fix: " << (int)i.fix
          << " time: " << i.time 
          << " hr: " << (int)i.hr 
          << " lon: " << i.lon
          << " lat: " << i.lat
          << " ele: " << i.ele
          << std::endl;
         
    }
    virtual void onReadBlocks(int id, int count) {
        std::cout << "reading block #" << id << " + " << count << std::endl;
    }
    virtual void onReadBlock(int id, int addr) {
        std::cout << "reading block #" << std::dec << id << " from 0x"<< std::hex << addr << std::endl;
    }
};
