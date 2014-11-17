// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#include <iostream>

#include "DataTypes.hpp"
#include "DebugWriter.hpp"

DebugWriter::DebugWriter(int level) : debug_level(level) , wo_with_coord(0) {
}

void DebugWriter::onWatch(const WatchInfo &i) {
    std::cout << "watch begin"
      << " version='" << i.version << "'"
      << " firmware='" << i.firmware << "'"
      << std::endl;
}
void DebugWriter::onWatchEnd(const WatchInfo &) {
    std::cout << "watch end" << std::endl;
}
void DebugWriter::onWorkout(const WorkoutInfo &i)  {
    std::cout << " workout info"
      << " t=" << i.start_time
      << " nsamples=" << i.nsamples
      << std::endl;
    wo_with_coord = 0;
}
void DebugWriter::onWorkoutEnd(const WorkoutInfo &)  {
    std::cout << " workout end"
      << " samples with coord=" << wo_with_coord
      << std::endl;
}
void DebugWriter::onTrack(const TrackInfo &)  {
    std::cout << "  track begin" << std::endl;
}
void DebugWriter::onTrackEnd(const TrackInfo &)  {
    std::cout << "  track end" << std::endl;
}
void DebugWriter::onSample(const SampleInfo &i) {
    if (debug_level > 1) {
        std::cout << "   sample info: " 
          << " idx_wo=" << (int)i.idx_wo
          << " idx_track=" << (int)i.idx_track
          << " type=" << (int)i.type
          << " fix=" << (int)i.fix
          << " time=" << i.time 
          << " hr=" << (int)i.hr 
          << " lon=" << i.lon
          << " lat=" << i.lat
          << " ele=" << i.ele
          << std::endl;
    }
    if (i.fix != 0) {
        wo_with_coord ++;
    }

}
void DebugWriter::onReadBlocks(int id, int count) {
    std::cout << "memory block #" << id << " + " << count << std::endl;
}
void DebugWriter::onReadBlock(int id, int addr, unsigned char*, size_t) {
    if (debug_level > 1) {
        std::cout << "reading block #" << std::dec << id << " from 0x"<< std::hex << addr << std::endl;
    }
}

