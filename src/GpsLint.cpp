// Copyright (C) 2014 mru@sisyphus.teil.cc
//
//
//


#include <iostream>

#include <cassert>
#include <cmath>

#include "DataTypes.hpp"
#include "GpsLint.hpp"

//GpsLint::GpsLint() : lon(14.2), lat(48.3) {
GpsLint::GpsLint() : lon(19.2), lat(50.3) {
}

void GpsLint::onSample(const SampleInfo &i) {
    if (i.fix != 0) {
        assert ( fabs( (double)i.lon ) < 180.0 );
        assert ( fabs( (double)i.lat ) < 90.0 );

        // somewhere in europe?
        assert ( fabs( lon - (double)i.lon ) < 30 );
        assert ( fabs( lat - (double)i.lat ) < 30 );
    }
    assert (i.time.YY == 14);
}

