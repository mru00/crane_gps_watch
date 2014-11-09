// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#pragma once

#include <stack>
#include <stdexcept>
#include <cassert>
#include <iostream>


#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>


std::string format_xml_exception(int code) {
    std::ostringstream ss;
    ss << "XML error #" << code;
    return ss.str();
}

class XmlFileWriter {
  public:
    XmlFileWriter(std::string filename) {
        LIBXML_TEST_VERSION;
        std::cerr << filename << std::endl;
        w = xmlNewTextWriterFilename((filename).c_str(), 0);
        if (w == nullptr) throw std::runtime_error("Failed to create XML writer");
        xmlTextWriterSetIndent(w, 1);
    }

    ~XmlFileWriter() {
        xmlFreeTextWriter(w);
    }

    void startDocument(const std::string& encoding = "UTF-8") {
        int rc;
        rc = xmlTextWriterStartDocument(w, NULL, encoding.c_str(), NULL);
        if (rc < 0) throw std::runtime_error(format_xml_exception(rc));
    }

    void startElement(const std::string& name) {
        int rc;
        rc = xmlTextWriterStartElement(w, BAD_CAST name.c_str());
        if (rc < 0) throw std::runtime_error("Error XML");
        stack.push(name);
    }

    void writeAttribute(const std::string& name, const std::string& value) {
        int rc;
        rc = xmlTextWriterWriteAttribute(w, BAD_CAST name.c_str(), BAD_CAST value.c_str());
        if (rc < 0) throw std::runtime_error("Error XML");
    }

    void writeElement(const std::string& name, const std::string& value) {
        int rc;
        rc = xmlTextWriterWriteElement(w, BAD_CAST name.c_str(), BAD_CAST value.c_str());
        if (rc < 0) throw std::runtime_error("error");
    }

    void endElement(const std::string& name = "") {
        int rc;
        rc = xmlTextWriterEndElement(w);
        if (rc < 0) throw std::runtime_error("error");
        if (name.size()) {
            assert(name == stack.top());
        }
        stack.pop();
    }

    void endDocument() {
        int rc;
        rc = xmlTextWriterEndDocument(w);
        if (rc < 0) throw std::runtime_error(format_xml_exception(rc));
    }


  private:
    xmlTextWriterPtr w;
    std::stack<std::string> stack;
};


class TcxWriter : public Callback{
  public:
    TcxWriter(std::string filename) :writer(filename) {
        std::cerr<<"writing to " <<filename<<std::endl;
        writer.startDocument();
    }
    virtual ~TcxWriter() {
        writer.endDocument();
    }
    virtual void onWatch(const WatchInfo &) {
        writer.startElement("TrainingCenterDatabase");
        writer.writeAttribute("xmlns", "http://www.garmin.com/xmlschemas/TrainingCenterDatabase/v2");
        writer.startElement("Activities");


    }
    virtual void onWatchEnd(const WatchInfo &) {
        writer.endElement("Activities");
        writer.endElement("TrainingCenterDatabase");
    }
    virtual void onWorkout(const WorkoutInfo &i)  {
        writer.startElement("Activity");
        writer.writeAttribute("Sport", "Other");
        writer.writeElement("Id", i.start_time);
        writer.startElement("Lap");
        writer.writeAttribute("StartTime", i.start_time);
        writer.writeElement("TotalTimeSeconds", "0");
        writer.writeElement("DistanceMeters", "0");
          {
            std::ostringstream ss;
            ss<<i.calories/100.0;
            writer.writeElement("Calories", ss.str());
          }
        writer.writeElement("Itensity", "Active");
        writer.writeElement("TriggerMethod", "Manual");
        writer.startElement("Track");
    }
    virtual void onWorkoutEnd(const WorkoutInfo &)  { 
        writer.endElement("Track");
        writer.endElement("Lap");
        writer.endElement("Activity");
    }
    virtual void onSample(const SampleInfo &i) { 
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

    virtual void onReadBlocks(int id, int count) {}
    virtual void onReadBlock(int id, int addr) {}

  private:
    XmlFileWriter writer;
};
