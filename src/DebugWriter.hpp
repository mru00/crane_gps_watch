// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//

#pragma once

#include "Callback.hpp"

class DebugWriter : public Callback {

  public:
    DebugWriter(int debug_level);
    virtual ~DebugWriter() {}
    void onWatch(const WatchInfo &i) override;
    void onWatchEnd(const WatchInfo &) override;
    void onWorkout(const WorkoutInfo &i) override;
    void onWorkoutEnd(const WorkoutInfo &) override;
    void onLap(const LapInfo &i) override;
    void onLapEnd(const LapInfo &) override;
    void onTrack(const TrackInfo &i) override;
    void onTrackEnd(const TrackInfo &) override;
    void onSample(const SampleInfo &i) override;
    void onReadBlocks(int id, int count) override;
    void onReadBlock(int id, int addr, unsigned char*, size_t) override;
  private:
    int debug_level;
    int wo_samples_with_coord, wo_samples, num_workouts;
    double diff_acc;
};
