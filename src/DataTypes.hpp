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
    GpsTime();
    GpsTime& operator=(const GpsTimeUpd& other);
    time_t mktime();
    std::string format() const;
};

enum DisplayItems {
    Altitude,
    Calories,
    Distance,
    Heading,
    HR_Avg, HR_Max, HR_Min, HR,
    HRZ_Abv, HRZ_Blw, HRZ_In,
    LapDist, LapNo, LapTime,
    PaceAvg, PaceMax, Pace,
    SpeedAvg, SpeedMax, Speed,
    TimeOfDay,
    WkoutTime,
    WP
};

struct Language {
    enum language_t {
        English,
        Italiano,
        German,
        French,
        Spanish,
    } language;
    Language& operator= (unsigned char);
    std::string format() const;
};

struct Profile {
    enum profile_e {
        Running, 
        Cycling, 
        Hiking,
        Sailing,
        User
    } profile;

    Profile& operator= (unsigned char);
    std::string format() const;
};

// Toc is a list of numbers;
// each entry designates one workout record.
// the number is the number of memory blocks for that record.
struct Toc : public std::vector<int> {
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

struct LapInfo {
    GpsTime abs_split;
    GpsTime start_time;

    tm split;
    int split_milli; // GpsTime doesn't have millisplit

    tm lap;
    int lap_milli; // GpsTime doesn't have millisplit
    int lap_seconds;

    tm pace;

    long distance; // in m
    double speed; // in km/hr
    int avg_hr;
};

struct WorkoutInfo {

    // i doubt nsamples:
    unsigned nsamples;

    unsigned lapcount;
    GpsTime start_time;
    GpsTime workout_time;
    Profile profile;
    double total_km;
    double speed_avg;
    double speed_max;
    double calories;
    LapInfo *lapinfo;
};

struct TrackInfo {
};

struct WatchInfo {
    unsigned timezone;
    unsigned sample_interval;
    Profile selected_profile;
    Language language;
    unsigned nblocks;
    std::vector<std::string> path_names;
    std::vector<std::string> profile_names;
    std::string version;
    std::string version2;
    std::string firmware;
    Toc toc;
};



