// Copyright (C) 2015 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//

#pragma once

#include "Callback.hpp"

class LapWriter : public Callback {

  public:
    LapWriter();
    virtual ~LapWriter() {}
    void onLap(const LapInfo &i) override;
};
