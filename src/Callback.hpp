// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//

#pragma once

#include <vector>
#include <memory>


class WatchInfo;
class WorkoutInfo;
class SampleInfo;



class Callback {
  public:
    virtual ~Callback() {}
    virtual void onWatch(const WatchInfo &) {}
    virtual void onWatchEnd(const WatchInfo &) {}
    virtual void onWorkout(const WorkoutInfo &) {}
    virtual void onWorkoutEnd(const WorkoutInfo &) {}
    virtual void onTrack(const TrackInfo&) {}
    virtual void onTrackEnd(const TrackInfo&) {}
    virtual void onSample(const SampleInfo &) {}
    virtual void onReadBlocks(int /*id*/, int /*count*/) {}
    virtual void onReadBlock(int /*id*/, int /*addr*/, unsigned char* /*data*/, size_t /*size*/) {}
};

class Broadcaster : public Callback {

  public:
    Broadcaster() : recipients() {}
    virtual ~Broadcaster() {}

    void addRecipient(std::shared_ptr<Callback> c) { recipients.push_back(c); }

    void onWatch(const WatchInfo &i) override { 
        for (auto c : recipients) c->onWatch(i);
    }
    void onWatchEnd(const WatchInfo &i) override { 
        for (auto c : recipients) c->onWatchEnd(i);
    }
    void onWorkout(const WorkoutInfo &i) override {
        for (auto c : recipients) c->onWorkout(i);
    }
    void onWorkoutEnd(const WorkoutInfo &i) override {
        for (auto c : recipients) c->onWorkoutEnd(i);
    }
    void onTrack(const TrackInfo &i) override {
        for (auto c : recipients) c->onTrack(i);
    }
    void onTrackEnd(const TrackInfo &i) override {
        for (auto c : recipients) c->onTrackEnd(i);
    }
    void onSample(const SampleInfo &i) override {
        for (auto c : recipients) c->onSample(i);
    }
    void onReadBlocks(int id, int count) override {
        for (auto c : recipients) c->onReadBlocks(id, count);
    }
    void onReadBlock(int id, int addr, unsigned char* data, size_t size) override {
        for (auto c : recipients) c->onReadBlock(id, addr, data, size);
    }
  private:
    std::vector<std::shared_ptr<Callback> > recipients;
};



