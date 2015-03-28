// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc
//
//
//

#pragma once

#include "Callback.hpp"

class GpsLint : public Callback {

  public:
    GpsLint();
    virtual ~GpsLint() {}
    void onSample(const SampleInfo &i) override;
  private:
    double lon, lat;
};
