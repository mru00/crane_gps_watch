// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// Data Types
//


#include <sstream>
#include <iomanip>
#include <vector>


#include "DataTypes.hpp"


class GpsLocation;
std::ostream& operator<<(std::ostream&, const GpsLocation&);

class GpsEle;
std::ostream& operator<<(std::ostream&, const GpsEle&);

class GpsTime;
std::ostream& operator<<(std::ostream&, const GpsTime&);

GpsLocation& GpsLocation::operator += (const GpsLocation& other) {
    loc += other.loc;
    return *this;
}

GpsLocation::operator const std::string() const {
    std::ostringstream ss;
    ss << (*this);
    return ss.str();
}


GpsEle& GpsEle::operator += (const GpsEle& other) {
    ele += other.ele;
    return *this;
}
GpsEle::operator std::string() const {
    std::ostringstream ss;
    ss << (*this);
    return ss.str();
}

GpsTime& GpsTime::operator=(const GpsTimeUpd& other) {
    mm = other.mm;
    ss = other.ss;
    return *this;
}
GpsTime::operator std::string() const {
    std::ostringstream ss;
    ss << (*this);
    return ss.str();
}

std::ostream& operator<< (std::ostream& s, const GpsLocation& l) {
    s << std::setprecision(15) << l.loc / 10000000.0;
    return s;
}

std::ostream& operator<< (std::ostream& s, const GpsEle& l) {
    s << l.ele;
    return s;
}

std::ostream& operator<< (std::ostream& s, const GpsTime& t) {
    s << std::dec
      << "20" << (int)t.YY << "-"  
      << std::setfill('0') << std::setw(2) << (int)t.MM << "-" 
      << std::setfill('0') << std::setw(2) << (int)t.DD << "T" 
      << std::setfill('0') << std::setw(2) << (int)t.hh << ":" 
      << std::setfill('0') << std::setw(2) << (int)t.mm << ":" 
      << std::setfill('0') << std::setw(2) << (int)t.ss << "Z";
    return s;
}

