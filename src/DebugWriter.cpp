// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#include <iostream>
#include <iomanip>
#include <ctime>

#include "DataTypes.hpp"
#include "DebugWriter.hpp"

DebugWriter::DebugWriter(int level) : debug_level(level) , wo_samples_with_coord(0), wo_samples(0), num_workouts(0) {
}

void DebugWriter::onWatch(const WatchInfo &i) {
    std::cout << "watch begin"
      << " p=" << i.selected_profile
      << " l=" << i.language.format()
      << " version='" << i.version << "'"
      //<< " version2='" << i.version2 << "'"
      << " firmware='" << i.firmware << "'"
      << " timezone=" << (int)i.timezone << "=0x" << std::hex << (int)i.timezone << std::dec
      << " profiles: ";

    for (auto pn : i.profile_names) {
        std::cout << pn << " ";
    }
    std::cout << std::endl;
}
void DebugWriter::onWatchEnd(const WatchInfo &) {
    if (debug_level > 0) {
        std::cout << "watch end" << std::endl;
    }
    std::cout << num_workouts << " workouts read from watch" << std::endl;
}
void DebugWriter::onWorkout(const WorkoutInfo &i)  {
    if (debug_level > 0) {
        std::cout 
          << std::endl
          << " workout info"
          << " p=" << i.profile
          << " t=" << i.start_time.format() << " dst=" << i.start_time.time.tm_isdst
          << " d=" << put_time(&i.workout_time.time, "%H:%M:%S") << "=" << i.workout_time.time.tm_hour*60*60 + i.workout_time.time.tm_min*60 +i.workout_time.time.tm_sec
          << " nsamples=" << i.nsamples << "=0x" << std::hex << i.nsamples << std::dec
          //<< " toc=" << i.toc
          << std::endl;
    }
    wo_samples_with_coord = wo_samples = 0;
    num_workouts ++;
}
void DebugWriter::onWorkoutEnd(const WorkoutInfo &i) {
    if (debug_level > 0) {
        std::cout 
          << " workout end"
          << " samples with gps=" << wo_samples_with_coord
          << " samples=" << wo_samples
          << " avg_time_diff=" << diff_acc/(wo_samples-1)
          << " d=" << i.nsamples*diff_acc/(wo_samples-1)
          << std::endl
          << std::endl;
    }
}
void DebugWriter::onTrack(const TrackInfo &)  {
    if (debug_level > 0) {
        std::cout << "  track begin" << std::endl;
    }
}
void DebugWriter::onTrackEnd(const TrackInfo &)  {
    if (debug_level > 0) {
        std::cout << "  track end" << std::endl;
    }
}
void DebugWriter::onSample(const SampleInfo &i) {
    if (debug_level > 1) {
        std::cout << "   sample info: " 
          << " idx_wo=" << (int)i.idx_wo
          << " idx_track=" << (int)i.idx_track
          << " type=" << (int)i.type
          << " fix=" << (int)i.fix
          << " time=" << i.time.format()
          << " hr=" << (int)i.hr 
          << " lon=" << i.lon.format()
          << " lat=" << i.lat.format()
          << " ele=" << i.ele.format()
          << std::endl;
    }
    if (i.fix != 0) {
        wo_samples_with_coord ++;
    }
    wo_samples ++;


    static SampleInfo lastI;
    if (i.idx_track > 1) {
        tm begin = lastI.time.time;
        tm end = i.time.time;
        double diff = difftime(mktime(&end), mktime(&begin));
        if ( diff > 0 ) {
            diff_acc += diff;
        }
    }
    else {
        diff_acc =0;
    }
    lastI = i;

}
void DebugWriter::onReadBlocks(int id, int count) {
    if (debug_level > 0) {
        std::cout << "memory block #" << id << " + " << count-1 << std::endl;
    }
}
void DebugWriter::onReadBlock(int id, int addr, unsigned char*, size_t) {
    if (debug_level > 1) {
        std::cout << "reading block #" << std::dec << id << " from 0x"<< std::hex << addr << std::endl;
    }
}

