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
    writer. startElement("Lap");
    writer. writeAttribute("StartTime", i.start_time.format());

      {
        long duration = i.workout_time.time.tm_hour*60*60 + i.workout_time.time.tm_min*60 +i.workout_time.time.tm_sec;
        std::ostringstream ss;
        ss<< duration;
        writer. writeElement("TotalTimeSeconds", ss.str());
      }
      {
        std::ostringstream ss;
        ss<< (double)(i.total_km)/10;
        writer.writeElement("DistanceMeters", ss.str());
      }
      {
        std::ostringstream ss;
        ss<< (double)(i.calories)/100;
        writer.writeElement("Calories", ss.str());
      }
      {
        std::ostringstream ss;
        ss<< (double)(i.speed_max)/10;
        writer.writeElement("MaximumSpeed", ss.str());
      }
      {
        std::ostringstream ss;
        ss<< (i.hr_avg);
        writer. startElement("AverageHeartRateBpm");
        writer.  writeElement("Value", ss.str());
        writer. endElement("AverageHeartRateBpm");
      }
      {
        std::ostringstream ss;
        ss<< (i.hr_max);
        writer. startElement("MaximumHeartRateBpm");
        writer.  writeElement("Value", ss.str());
        writer. endElement("MaximumHeartRateBpm");
      }
    writer.  writeElement("Intensity", "Active");
    writer.  writeElement("TriggerMethod", "Manual");
      {
        std::ostringstream ss;
        ss<< "Average speed (km/h): ";
        ss<< (double)(i.speed_avg)/10 << std::endl;
        ss<< "Minimum heart rate: ";
        ss<< (i.hr_min) << std::endl;
        ss<< "Time below training zone: ";
        ss<< std::setw(2) << std::setfill('0') << i.below_zone_time.time.tm_hour << ":";
        ss<< std::setw(2) << std::setfill('0') << i.below_zone_time.time.tm_min << ":";
        ss<< std::setw(2) << std::setfill('0') << i.below_zone_time.time.tm_sec << std::endl;
        ss<< "Time in training zone: ";
        ss<< std::setw(2) << std::setfill('0') << i.in_zone_time.time.tm_hour << ":";
        ss<< std::setw(2) << std::setfill('0') << i.in_zone_time.time.tm_min << ":";
        ss<< std::setw(2) << std::setfill('0') << i.in_zone_time.time.tm_sec << std::endl;
        ss<< "Time above training zone: ";
        ss<< std::setw(2) << std::setfill('0') << i.above_zone_time.time.tm_hour << ":";
        ss<< std::setw(2) << std::setfill('0') << i.above_zone_time.time.tm_min << ":";
        ss<< std::setw(2) << std::setfill('0') << i.above_zone_time.time.tm_sec;
        writer.writeElement("Notes", ss.str());
      }
}
void TcxWriter::onWorkoutEnd(const WorkoutInfo &)  { 
    writer.  writeElement("Notes", current_wo.start_time.format());
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

        {
          // required for turtle sports
          std::ostringstream ss;
          ss << (double)i.distance/10;
          writer. writeElement("DistanceMeters", ss.str());
        }
        {
          std::ostringstream ss;
          ss << (double)i.speed/100;
          writer. writeElement("Speed", ss.str());
        }
        { 
          std::ostringstream ss;
          ss << (int)i.orientation;
          writer. writeElement("Orientation", ss.str());
        }
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

