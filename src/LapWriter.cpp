// Copyright (C) 2015 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#include <iostream>
#include <iomanip>
#include <ctime>

#include "DataTypes.hpp"
#include "LapWriter.hpp"

LapWriter::LapWriter() {
}

void LapWriter::onLap(const LapInfo &i)  {
    if(i.lap_number == 1) {
        std::cout 
            << std::endl
            << "   Lap Absolute Time             WorkoutTime LapTime     HR  Dist  Spd       Pace" << std::endl
            << "   --- ------------------------- ----------- ----------- --- ----- --------- --------" << std::endl;
    }

    std::cout
        << "   " 
        << std::setw(3) << std::setfill(' ') << i.lap_number << " "
        << i.abs_split.format() << " "
        << std::setw(2) << std::setfill('0') << i.split.tm_hour << ":"
        << std::setw(2) << i.split.tm_min << ":"
        << std::setw(2) << i.split.tm_sec << "."
        << std::setw(2) << i.split_milli << " ";

    std::cout 
        << std::setw(2) << std::setfill('0') << i.lap.tm_hour << ":"
        << std::setw(2) << i.lap.tm_min << ":"
        << std::setw(2) << i.lap.tm_sec << "."
        << std::setw(2) << i.lap_milli << " ";

    std::cout 
        << std::setw(3) << std::setfill(' ') << i.avg_hr << " "
        << std::setw(5) << std::setprecision(2) << std::fixed << i.distance / 10 / 1000.0 << " "
        << std::setw(5) << std::setprecision(1) << std::fixed << i.speed  << "km/h "
        << std::setw(2) << i.pace.tm_min << ":" << std::setw(2) << std::setfill('0') << i.pace.tm_sec << "/km "
        << std::endl;
}
