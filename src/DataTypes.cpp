// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// Data Types
//


#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

#include <ctime>
#include <cstring>

#include "DataTypes.hpp"


struct fmt {
    std::ostringstream s;
    operator std::string() {return s.str();}
    template<class T> fmt& operator<<(T const& v) {return s<<v, *this;}
};

put_time::put_time(const tm* time, const std::string& format) : str() {
    const size_t max_size = 800;
    char buf[max_size];
    size_t n = std::strftime(buf, max_size, format.c_str(), time);
    if (n == 0) {
        throw std::runtime_error (fmt() << "failed to format time string: " << strerror(errno) << " with format " << format);
    }
    str = buf;
}

std::ostream& operator << (std::ostream& o, const put_time& time) {
    return o << time.str;
}

GpsLocation& GpsLocation::operator += (const GpsLocation& other) {
    loc += other.loc;
    return *this;
}

std::string GpsLocation::format() const {
    return fmt() << std::setprecision(15) << (double)(*this);
}

GpsLocation::operator double() const {
    return (loc / 10000000.0);
}

GpsEle& GpsEle::operator += (const GpsEle& other) {
    ele += other.ele;
    return *this;
}
std::string GpsEle::format() const {
    return fmt() << ele;
}

GpsTime::GpsTime() : time () {
    // explicit for documentation purposes:
    // see 'man mktime' for details.
    /*
       tm_isdst  A  flag  that  indicates  whether  daylight saving time is in$
                 effect at the time described.  The value is positive if  day‐$
                 light  saving time is in effect, zero if it is not, and nega‐$
                 tive if the information is not available.$
    */
    time.tm_isdst = 0;
}

GpsTime& GpsTime::operator=(const GpsTimeUpd& other) {
    time.tm_min = other.mm;
    time.tm_sec = other.ss;
    mktime();
    return *this;
}

time_t GpsTime::mktime() {
    time_t t = ::mktime(&time);
    if (t == (time_t) -1) {
        throw std::runtime_error(fmt() << "failed to mktime: " << strerror(errno));
    }
    return t;
}

std::string GpsTime::format() const {
    return fmt() << put_time(&time, "%Y-%m-%dT%H:%M:%SZ");
    // does not work with windows:
    //return fmt() << put_time(&time, "%FT%TZ");
}

