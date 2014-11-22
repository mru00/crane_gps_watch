// Copyright (C) 2014 mru@sisyphus.teil.cc
//
//
//


#include <iostream>
#include <stdexcept>

#include <cassert>
#include <cmath>

#include "DataTypes.hpp"
#include "GpsLint.hpp"

//GpsLint::GpsLint() : lon(14.2), lat(48.3) {
GpsLint::GpsLint() : lon(19.2), lat(50.3) {
}

void GpsLint::onSample(const SampleInfo &i) {
    if (i.fix != 0) {
        // general range for gps values
        // also covered by xmllint
        assert ( fabs( (double)i.lon ) < 180.0 );
        assert ( fabs( (double)i.lat ) < 90.0 );

        // somewhere in europe?
        assert ( fabs( lon - (double)i.lon ) < 30 );
        assert ( fabs( lat - (double)i.lat ) < 30 );

        // happens with 0x80 fix=0 entries:
        assert ( (double)i.lon != 200);
        assert ( (double)i.lat != 200);
    }

    // around this year
    assert ( std::abs(i.time.time.tm_year-114) < 5);

    static SampleInfo lastI;
    if (i.idx_track > 1) {
        tm begin = lastI.time.time;
        tm end = i.time.time;
        
        double diff = difftime(mktime(&end), mktime(&begin));
        if ( diff < 0 ) {
            std::cerr << "   LINT ERROR: sample #" << i.idx_track << " jumps back in time : " << diff << " seconds (time not monotonic) (dst?)" << std::endl;
            //throw std::runtime_error("lint error");
        }
    }
    lastI = i;


}

