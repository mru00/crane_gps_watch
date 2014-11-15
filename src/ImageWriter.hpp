// Copyright (C) 2014 mru@sisyphus.teil.cc
//
//
//

#pragma once

#include <fstream>


#include "Callback.hpp"

class ImageWriter : public Callback {

  public:
    ImageWriter(const std::string&);
    virtual void onWatch(const WatchInfo &i);
    virtual void onWatchEnd(const WatchInfo &);
    virtual void onWorkout(const WorkoutInfo &i);
    virtual void onWorkoutEnd(const WorkoutInfo &);
    virtual void onTrack(const TrackInfo &i);
    virtual void onTrackEnd(const TrackInfo &);
    virtual void onSample(const SampleInfo &i);
    virtual void onReadBlocks(int id, int count);
    virtual void onReadBlock(int id, int addr, unsigned char* data);

  private:
    std::ofstream f;
};
