// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// Data Types
//

#pragma once

#include <sstream>
#include <iomanip>
#include <vector>

#include <ctime>



// while we are waiting for http://en.cppreference.com/w/cpp/io/manip/put_time
struct put_time {
    std::string str;
    put_time(const tm* time, const std::string& format);
};
std::ostream& operator<< (std::ostream&, const put_time&);


struct GpsLocation {
    int loc;
    GpsLocation& operator += (const GpsLocation& other);
    std::string format() const;
    operator double() const;
};

struct GpsTimeUpd {
    int mm, ss;
};

struct GpsEle {
    unsigned short ele;
    GpsEle& operator += (const GpsEle& other);
    std::string format() const;
};

struct GpsTime {
    tm time;
    GpsTime& operator=(const GpsTimeUpd& other);
    std::string format() const;
};


struct SampleInfo {
    enum {
        Full = 0x00,
        Diff = 0x01,
        TimeOnly = 0x02,
        HrOnly = 0x03,
        None = 0x80,
        End = 0xff
    } type;
    GpsTime time;
    GpsTimeUpd time_upd;
    GpsLocation lon, lat;
    GpsEle ele;
    unsigned char hr;
    unsigned char fix;
    unsigned char fb, sb;
    unsigned idx_wo, idx_track;
};

struct WorkoutInfo {

    // i doubt nsamples:
    unsigned nsamples;

    unsigned lapcount;
    GpsTime start_time;
    GpsTime workout_time;
    unsigned profile;
    double total_km;
    double speed_avg;
    double speed_max;
    double calories;
    std::string toc;
};

struct TrackInfo {
};

struct WatchInfo {
    unsigned timezone;
    unsigned sample_interval;
    unsigned selected_profile;
    unsigned nblocks;
    std::vector<std::string> path_names;
    std::vector<std::string> profile_names;
    std::string version;
    std::string firmware;
};



