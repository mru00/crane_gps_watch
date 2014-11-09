// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//

#pragma once

#include <vector>


class WatchInfo;
class WorkoutInfo;
class SampleInfo;



class Callback {
  public:
    virtual void onWatch(const WatchInfo &) = 0;
    virtual void onWatchEnd(const WatchInfo &) = 0;
    virtual void onWorkout(const WorkoutInfo &) = 0;
    virtual void onWorkoutEnd(const WorkoutInfo &) = 0;
    virtual void onSample(const SampleInfo &) = 0;
    virtual void onReadBlocks(int id, int count) = 0;
    virtual void onReadBlock(int id, int addr) = 0;
};

class Broadcaster : public Callback {

  public:
    void addRecipient(Callback* c) { recipients.push_back(c); }

    virtual void onWatch(const WatchInfo &i) { 
        for (auto c : recipients) c->onWatch(i);
    }
    virtual void onWatchEnd(const WatchInfo &i) { 
        for (auto c : recipients) c->onWatchEnd(i);
    }
    virtual void onWorkout(const WorkoutInfo &i) {
        for (auto c : recipients) c->onWorkout(i);
    }
    virtual void onWorkoutEnd(const WorkoutInfo &i) {
        for (auto c : recipients) c->onWorkoutEnd(i);
    }
    virtual void onSample(const SampleInfo &i) {
        for (auto c : recipients) c->onSample(i);
    }
    virtual void onReadBlocks(int id, int count) {
        for (auto c : recipients) c->onReadBlocks(id, count);
    }
    virtual void onReadBlock(int id, int addr) {
        for (auto c : recipients) c->onReadBlock(id, addr);
    }
  private:
    std::vector<Callback*> recipients;
};

