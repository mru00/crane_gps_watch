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


#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "DataTypes.hpp"
#include "Callback.hpp"
#include "TcxWriter.hpp"

std::string format_xml_exception(int code) {
    std::ostringstream ss;
    ss << "XML error #" << code;
    return ss.str();
}




XmlFileWriter::XmlFileWriter() :w(nullptr) {
    LIBXML_TEST_VERSION;
}

XmlFileWriter::~XmlFileWriter() {
}

void XmlFileWriter::open(const std::string& filename) {
    std::cerr << "XmlFileWriter: writing to " << filename << std::endl;
    w = xmlNewTextWriterFilename((filename).c_str(), 0);
    if (w == nullptr) throw std::runtime_error("Failed to create XML writer");
    xmlTextWriterSetIndent(w, 1);
}

bool XmlFileWriter::isOpen() {
    return w != nullptr;
}

void XmlFileWriter::close() {
    xmlFreeTextWriter(w);
    w = nullptr;
}

void XmlFileWriter::startDocument(const std::string& encoding) {
    int rc;
    rc = xmlTextWriterStartDocument(w, NULL, encoding.c_str(), NULL);
    if (rc < 0) throw std::runtime_error(format_xml_exception(rc));
}
void XmlFileWriter::endDocument() {
    int rc;
    rc = xmlTextWriterEndDocument(w);
    if (rc < 0) throw std::runtime_error(format_xml_exception(rc));
}

void XmlFileWriter::startElement(const std::string& name) {
    int rc;
    rc = xmlTextWriterStartElement(w, BAD_CAST name.c_str());
    if (rc < 0) throw std::runtime_error("Error XML");
    stack.push(name);
}

void XmlFileWriter::endElement(const std::string& name) {
    int rc;
    rc = xmlTextWriterEndElement(w);
    if (rc < 0) throw std::runtime_error("error");
    if (name.size()) {
        assert(name == stack.top());
    }
    stack.pop();
}

void XmlFileWriter::writeAttribute(const std::string& name, const std::string& value) {
    int rc;
    rc = xmlTextWriterWriteAttribute(w, BAD_CAST name.c_str(), BAD_CAST value.c_str());
    if (rc < 0) throw std::runtime_error("Error XML");
}

void XmlFileWriter::writeElement(const std::string& name, const std::string& value) {
    int rc;
    rc = xmlTextWriterWriteElement(w, BAD_CAST name.c_str(), BAD_CAST value.c_str());
    if (rc < 0) throw std::runtime_error("error");
}






TcxWriter::TcxWriter(std::string filename, bool split_by_track) : filename(filename), split_by_track(split_by_track) {
}
TcxWriter::~TcxWriter() {
    if (writer.isOpen()) {
        // XXX shouldn't happen
        writer.endDocument();
    }
}
void TcxWriter::onWatch(const WatchInfo &) {
    if (!split_by_track) {
        writer.open(filename);
        writer.startDocument();
        writer.startElement("TrainingCenterDatabase");
        writer.writeAttribute("xmlns", "http://www.garmin.com/xmlschemas/TrainingCenterDatabase/v2");
        writer.startElement("Activities");
    }
}
void TcxWriter::onWatchEnd(const WatchInfo &) {
    if (!split_by_track) {
        writer.endElement("Activities");
        writer.endElement("TrainingCenterDatabase");
        writer.endDocument();
    } 
}
void TcxWriter::onWorkout(const WorkoutInfo &i)  {
    if (split_by_track) {
        std::string fn = i.start_time;
        fn += ".tcx";
        std::replace(fn.begin(), fn.end(), ':', '_');
        writer.open(fn);
        writer.startDocument();
        writer.startElement("TrainingCenterDatabase");
        writer.writeAttribute("xmlns", "http://www.garmin.com/xmlschemas/TrainingCenterDatabase/v2");
        writer.startElement("Activities");
    }

    current_wo = i;
    writer.startElement("Activity");
    writer.writeAttribute("Sport", "Other");
    writer.writeElement("Id", i.start_time);
    writer.startElement("Lap");
    writer.writeAttribute("StartTime", i.start_time);
    writer.writeElement("TotalTimeSeconds", "0");
    writer.writeElement("DistanceMeters", "0");
      {
        std::ostringstream ss;
        ss<< (int)(i.calories/100);
        writer.writeElement("Calories", ss.str());
      }
    writer.writeElement("Intensity", "Active");
    writer.writeElement("TriggerMethod", "Manual");
}
void TcxWriter::onWorkoutEnd(const WorkoutInfo &)  { 
    writer.writeElement("Notes", current_wo.start_time);
    writer.endElement("Lap");
    writer.endElement("Activity");

    if (split_by_track) {
        writer.endElement("Activities");
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
    writer.writeElement("Time", i.time);
    if (i.lon.loc != 0) {
        writer.startElement("Position");
        writer.writeElement("LatitudeDegrees", i.lat);
        writer.writeElement("LongitudeDegrees", i.lon);
        writer.endElement("Position");
        writer.writeElement("AltitudeMeters", i.ele);
    }
    if (i.hr != 0) {
        std::ostringstream ss;
        ss << (int)i.hr;
        writer.startElement("HeartRateBpm");
        writer.writeElement("Value", ss.str());
        writer.endElement("HeartRateBpm");
    }
    writer.endElement("Trackpoint");
}

void TcxWriter::onReadBlocks(int id, int count) {}
void TcxWriter::onReadBlock(int id, int addr) {}

