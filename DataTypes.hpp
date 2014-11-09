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
    enum { Full, None, Diff, HrOnly, TimeOnly, End } type;
    GpsTime time;
    GpsTimeUpd time_upd;
    GpsLocation lon, lat;
    GpsEle ele;
    unsigned char hr;
    unsigned char fb;
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

struct WatchInfo {
    unsigned timezone;
    unsigned sample_interval;
    unsigned selected_profile;
    unsigned nblocks;
    std::vector<std::string> path_names;
    std::vector<std::string> profile_names;
    std::string version;
};
