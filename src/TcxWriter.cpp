// Copyright (C) 2014 mru@sisyphus.teil.cc
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


TcxWriter::TcxWriter(std::string filename, bool split_by_track) :  writer(), filename(filename), current_wo(), split_by_track(split_by_track) {
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
    switch (i.profile.profile) {
      case Profile::Running:
        sport = "Running";
        break;
      case Profile::Cycling:
        sport = "Biking";
        break;
      default:
        sport = "Other";
    }

    current_wo = i;
    writer.startElement("Activity");
    writer. writeAttribute("Sport", sport);
    writer. writeElement("Id", i.start_time.format());
}
void TcxWriter::onWorkoutEnd(const WorkoutInfo &)  { 
    writer.endElement("Activity");

    if (split_by_track) {
        writer. endElement("Activities");
        writer.endElement("TrainingCenterDatabase");
        writer.endDocument();
    }
}
void TcxWriter::onLap(const LapInfo &i) {
    writer.startElement("Lap");
    writer. writeAttribute("StartTime", i.start_time.format());
    writer.writeElement("TotalTimeSeconds", std::to_string(i.lap_seconds));
    writer.writeElement("TriggerMethod", "Manual");
    writer.writeElement("DistanceMeters", std::to_string(i.distance / 10));
    writer.writeElement("AverageHeartRateBpm", std::to_string(i.avg_hr));
}
void TcxWriter::onLapEnd(const LapInfo &i) {
    writer.writeElement("Notes", i.abs_split.format());
    writer.endElement("Lap");
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

        // required for turtle sports
        writer. writeElement("DistanceMeters", "0");
    }
    if (i.hr != 0) {
        std::ostringstream ss;
        ss << (int)i.hr;
        writer. startElement("HeartRateBpm");
        writer.  writeElement("Value", ss.str());
        writer. endElement("HeartRateBpm");
    }
    writer.endElement("Trackpoint");
}

