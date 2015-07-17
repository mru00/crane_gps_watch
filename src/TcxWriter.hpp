// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc
//
//
//


#pragma once


#include <stack>

#include "XmlFileWriter.hpp"



class TcxWriter : public Callback {
  public:
    TcxWriter(std::string filename, bool split_by_track);
    virtual ~TcxWriter();
    void onWatch(const WatchInfo &) override;
    void onWatchEnd(const WatchInfo &) override;
    void onWorkout(const WorkoutInfo &i)  override;
    void onWorkoutEnd(const WorkoutInfo &) override;
    void onTrack(const TrackInfo&) override;
    void onTrackEnd(const TrackInfo&) override;
    void onSample(const SampleInfo &i) override;

  private:
    XmlFileWriter writer;
    std::string filename;
    WorkoutInfo current_wo;
    bool split_by_track;
    double distance_acc;
};
