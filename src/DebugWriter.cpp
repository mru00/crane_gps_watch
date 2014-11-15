// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#include <iostream>

#include "DataTypes.hpp"
#include "DebugWriter.hpp"

void DebugWriter::onWatch(const WatchInfo &i) {
    std::cout << "watch version: " << i.version << std::endl;
}
void DebugWriter::onWatchEnd(const WatchInfo &) {
    std::cout << "watch end" << std::endl;
}
void DebugWriter::onWorkout(const WorkoutInfo &i)  {
    std::cout << " workout info"
      << " t=" << i.start_time
      << " nsamples=" << i.nsamples
      << std::endl;
}
void DebugWriter::onWorkoutEnd(const WorkoutInfo &)  {
    std::cout << " workout end" << std::endl;
}
void DebugWriter::onTrack(const TrackInfo &i)  {
    std::cout << "  track info " << std::endl;
}
void DebugWriter::onTrackEnd(const TrackInfo &)  {
    std::cout << "  track end" << std::endl;
}
void DebugWriter::onSample(const SampleInfo &i) {
    std::cout << "   sample info: " 
      << " idx_wo: " << (int)i.idx_wo
      << " idx_track: " << (int)i.idx_track
      << " type: " << (int)i.type
      << " fix: " << (int)i.fix
      << " time: " << i.time 
      << " hr: " << (int)i.hr 
      << " lon: " << i.lon
      << " lat: " << i.lat
      << " ele: " << i.ele
      << std::endl;

}
void DebugWriter::onReadBlocks(int id, int count) {
    std::cout << "reading block #" << id << " + " << count << std::endl;
}
void DebugWriter::onReadBlock(int id, int addr, unsigned char*) {
    std::cout << "reading block #" << std::dec << id << " from 0x"<< std::hex << addr << std::endl;
}
