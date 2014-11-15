// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//

#pragma once

#include "Callback.hpp"

class DebugWriter : public Callback {

  public:
    virtual void onWatch(const WatchInfo &i);
    virtual void onWatchEnd(const WatchInfo &);
    virtual void onWorkout(const WorkoutInfo &i);
    virtual void onWorkoutEnd(const WorkoutInfo &);
    virtual void onTrack(const TrackInfo &i);
    virtual void onTrackEnd(const TrackInfo &);
    virtual void onSample(const SampleInfo &i);
    virtual void onReadBlocks(int id, int count);
    virtual void onReadBlock(int id, int addr);
};
