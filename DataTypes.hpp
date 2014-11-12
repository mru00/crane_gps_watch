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
    unsigned mm, ss;
};

struct GpsEle {
    unsigned short ele;
    GpsEle& operator += (const GpsEle& other);
    operator std::string() const;
};

struct GpsTime {
    unsigned char YY, MM, DD, hh, mm, ss;
    GpsTime& operator=(const GpsTimeUpd& other);
    operator std::string() const;
};

std::ostream& operator<< (std::ostream& s, const GpsLocation& l);

std::ostream& operator<< (std::ostream& s, const GpsEle& l);

std::ostream& operator<< (std::ostream& s, const GpsTime& t);

struct SampleInfo {
    SampleInfo() {}
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
    unsigned timezone;
    unsigned sample_interval;
    unsigned selected_profile;
    unsigned nblocks;
    std::vector<std::string> path_names;
    std::vector<std::string> profile_names;
    std::string version;
};
