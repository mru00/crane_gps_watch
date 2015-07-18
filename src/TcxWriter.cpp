// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//



#include <stack>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <sstream>
#include <algorithm>


#include "DataTypes.hpp"
#include "Callback.hpp"
#include "TcxWriter.hpp"


TcxWriter::TcxWriter(std::string filename, bool split_by_track) 
    : writer()
       , filename(filename)
       , current_wo()
       , split_by_track(split_by_track)
       , distance_acc(0) {
}

TcxWriter::~TcxWriter() {
    if (writer.isOpen()) {
        // XXX shouldn't happen
        writer.endDocument();
        writer.close();
    }
}

void TcxWriter::onWatch(const WatchInfo &) {
}

void TcxWriter::onWatchEnd(const WatchInfo &) {
    // only close when at least one workout was written... if the writer is open
    if (!split_by_track && writer.isOpen()) {
        writer.endElement("Activities");
        writer.endElement("TrainingCenterDatabase");
        writer.endDocument();
        writer.close();
    } 
}

void TcxWriter::onWorkout(const WorkoutInfo &i)  {
    if (split_by_track) {
        std::string fn = i.start_time.format();
        fn += ".tcx";
        std::replace(fn.begin(), fn.end(), ':', '_');
        writer.open(fn);
        writer.startDocument();
        writer.startElement("TrainingCenterDatabase");
        writer.writeAttribute("xmlns", "http://www.garmin.com/xmlschemas/TrainingCenterDatabase/v2");
        writer.startElement("Activities");
    }
    else {
        // open document on the first workout for non-split mode

        if (!writer.isOpen()) {
            writer.open(filename);
            writer.startDocument();
            writer.startElement("TrainingCenterDatabase");
            writer.writeAttribute("xmlns", "http://www.garmin.com/xmlschemas/TrainingCenterDatabase/v2");
            writer.startElement("Activities");
        }
    }

    std::string sport;

    // small translation profile -> tcx activity
    // terrible but necessary: language dependent!
    if (i.profile == "Running" || i.profile == "Laufen") sport = "Running";
    else if (i.profile == "Cycling" || i.profile == "Radfahren") sport = "Biking";
    else sport = "Other";

    current_wo = i;
    writer.startElement("Activity");
    writer. writeAttribute("Sport", sport);
    writer. writeElement("Id", i.start_time.format());
    writer. startElement("Lap");
    writer. writeAttribute("StartTime", i.start_time.format());

    writer. writeElement("TotalTimeSeconds", fmt() << (i.workout_time.time.tm_hour*60*60 + i.workout_time.time.tm_min*60 + i.workout_time.time.tm_sec));
    writer. writeElement("DistanceMeters", fmt() << (double)(i.total_km)/10);
    writer. writeElement("MaximumSpeed", fmt() << (double)(i.speed_max)/10);
    writer. writeElement("Calories", fmt() << (int)(i.calories)/100);

    if (i.hr_avg > 0 ) {
        writer. startElement("AverageHeartRateBpm");
        writer.  writeElement("Value", fmt() << i.hr_avg);
        writer. endElement("AverageHeartRateBpm");
    }
    if (i.hr_max > 0) {
        writer. startElement("MaximumHeartRateBpm");
        writer.  writeElement("Value", fmt() << i.hr_max);
        writer. endElement("MaximumHeartRateBpm");
    }
    writer. writeElement("Intensity", "Active");
    writer. writeElement("TriggerMethod", "Manual");
}


struct formatHMS {
    const GpsTime& t;
    formatHMS(const GpsTime& t) : t(t) {}
};

static std::ostream& operator << (std::ostream &os, const formatHMS& t) {
    os << std::setw(2) << std::setfill('0') << t.t.time.tm_hour << ":"
        << std::setw(2) << std::setfill('0') << t.t.time.tm_min << ":"
        << std::setw(2) << std::setfill('0') << t.t.time.tm_sec;
    return os;
}

void TcxWriter::onWorkoutEnd(const WorkoutInfo & i)  { 

    distance_acc = 0;

    std::ostringstream ss;
    ss << "Start time: " << current_wo.start_time.format() <<std::endl
        << "Activity: " << current_wo.profile << std::endl
        << "Avg speed (km/h): " << (double)(i.speed_avg)/10 << std::endl
        << "Max speed (km/h): " << (double)(i.speed_max)/10 << std::endl
        << "Calories: " << (i.calories/100) << std::endl
        << "Min heart rate: " << (i.hr_min) << std::endl
        << "Avg heart rate: " << (i.hr_avg) << std::endl
        << "Max heart rate: " << (i.hr_max) << std::endl
        << "Time below training zone: " << formatHMS(i.below_zone_time) << std::endl
        << "Time in training zone: "  << formatHMS(i.in_zone_time) << std::endl
        << "Time above training zone: " << formatHMS(i.above_zone_time)
        ;

    writer.  writeElement("Notes", ss.str());
    writer. endElement("Lap");
    writer.endElement("Activity");

    if (split_by_track) {
        writer. endElement("Activities");
        writer.endElement("TrainingCenterDatabase");
        writer.endDocument();
    }
}

void TcxWriter::onTrack(const TrackInfo&) {
    writer.startElement("Track");
}

void TcxWriter::onTrackEnd(const TrackInfo&) {
    writer.endElement("Track");
}

void TcxWriter::onSample(const SampleInfo &i) { 
    writer.startElement("Trackpoint");
    writer. writeElement("Time", i.time.format());
    if (i.fix != 0) {
        writer. startElement("Position");
        writer.  writeElement("LatitudeDegrees", i.lat.format());
        writer.  writeElement("LongitudeDegrees", i.lon.format());
        writer. endElement("Position");
        writer. writeElement("AltitudeMeters", i.ele.format());

        writer. writeElement("DistanceMeters", fmt() << distance_acc);
        distance_acc += (double)i.distance/10;

        //writer. writeElement("Speed", fmt() << (double)i.speed/100);
        //writer. writeElement("Orientation", fmt() << (int)i.orientation);
    }
    if (i.hr != 0) {
        writer. startElement("HeartRateBpm");
        writer.  writeElement("Value", fmt() << (int)i.hr);
        writer. endElement("HeartRateBpm");
    }
    writer.endElement("Trackpoint");
}

