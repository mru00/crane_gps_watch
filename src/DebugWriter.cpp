// Copyright (C) 2014 mru@sisyphus.teil.cc
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
      << " p=" << i.selected_profile.format()
      << " l=" << i.language.format()
      << " version='" << i.version << "'"
      << " version2='" << i.version2 << "'"
      << " firmware='" << i.firmware << "'"
      << " timezone=" << (int)i.timezone << "=0x" << std::hex << (int)i.timezone << std::dec
      << std::endl;
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
          << " p=" << i.profile.format()
          << " t=" << i.start_time.format()
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
void DebugWriter::onLap(const LapInfo &i)  {
    if (debug_level > 0) {
        std::cout << "  lap begin" << std::endl;

        std::cout << "   Absolute Time             WorkoutTime LapTime     HR  Dist  Spd       Pace" << std::endl;
        std::cout << "   ------------------------- ----------- ----------- --- ----- --------- --------" << std::endl;

        std::cout << "   " << i.abs_split.format() << " ";
        std::cout << std::setw(2) << std::setfill('0') << i.split.tm_hour << ":"
            << std::setw(2) << i.split.tm_min << ":"
            << std::setw(2) << i.split.tm_sec << "."
            << std::setw(2) << i.split_milli << " ";

        std::cout << std::setw(2) << std::setfill('0') << i.lap.tm_hour << ":"
            << std::setw(2) << i.lap.tm_min << ":"
            << std::setw(2) << i.lap.tm_sec << "."
            << std::setw(2) << i.lap_milli << " ";

        std::cout << std::setw(3) << std::setfill(' ') << i.avg_hr << " ";

        std::cout << std::setw(5) << std::setprecision(2) << std::fixed << i.distance / 10 / 1000.0 << " ";

        std::cout << std::setw(5) << std::setprecision(1) << std::fixed << i.speed / 10.0 << "km/h ";

        std::cout << std::setw(2) << i.pace.tm_min << ":" << std::setw(2) << std::setfill('0') << i.pace.tm_sec << "/km ";
        std::cout << std::endl;
    }
}
void DebugWriter::onLapEnd(const LapInfo &)  {
    if (debug_level > 0) {
        std::cout << "  lap end" << std::endl;
    }
}
void DebugWriter::onTrack(const TrackInfo &)  {
    if (debug_level > 0) {
        std::cout << "   track begin" << std::endl;
    }
}
void DebugWriter::onTrackEnd(const TrackInfo &)  {
    if (debug_level > 0) {
        std::cout << "   track end" << std::endl;
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

