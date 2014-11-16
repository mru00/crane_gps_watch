// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// Data Types
//

#pragma once

#include <sstream>
#include <iomanip>
#include <vector>




class GpsLocation;
std::ostream& operator<<(std::ostream&, const GpsLocation&);

class GpsEle;
std::ostream& operator<<(std::ostream&, const GpsEle&);

class GpsTime;
std::ostream& operator<<(std::ostream&, const GpsTime&);

struct GpsLocation {
    int loc;
    GpsLocation& operator += (const GpsLocation& other);
    operator const std::string() const;
};

struct GpsTimeUpd {
    GpsTimeUpd() : mm(0), ss(0) {}
    unsigned mm, ss;
};

struct GpsEle {
    GpsEle() : ele(0) {}
    unsigned short ele;
    GpsEle& operator += (const GpsEle& other);
    operator std::string() const;
};

struct GpsTime {
    GpsTime() : YY(0), MM(0), DD(0), hh(0), mm(0), ss(0) {}
    unsigned char YY, MM, DD, hh, mm, ss;
    GpsTime& operator=(const GpsTimeUpd& other);
    operator std::string() const;
};

std::ostream& operator<< (std::ostream& s, const GpsLocation& l);

std::ostream& operator<< (std::ostream& s, const GpsEle& l);

std::ostream& operator<< (std::ostream& s, const GpsTime& t);

struct SampleInfo {
    SampleInfo() : type(SampleInfo::None), time(), time_upd(), lon(), lat(), ele(), hr(0), fix(0), fb(0), sb(0), idx_wo(0), idx_track(0) {}
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
    WorkoutInfo() : nsamples(0), lapcount(0), start_time(), workout_time(), profile(0), total_km(0), speed_avg(0), speed_max(0), calories(0) {}

    unsigned nsamples;
    unsigned lapcount;
    GpsTime start_time;
    GpsTime workout_time;
    unsigned profile;
    double total_km;
    double speed_avg;
    double speed_max;
    double calories;
};

struct TrackInfo {
};

struct WatchInfo {
    WatchInfo() : timezone(0), sample_interval(0), selected_profile(0), nblocks(0), path_names(), profile_names(), version() {}
    unsigned timezone;
    unsigned sample_interval;
    unsigned selected_profile;
    unsigned nblocks;
    std::vector<std::string> path_names;
    std::vector<std::string> profile_names;
    std::string version;
};



