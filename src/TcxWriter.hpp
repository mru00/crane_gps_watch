// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#pragma once


#include <stack>

#include <libxml/xmlwriter.h>


class XmlFileWriter;
class XmlFileWriter {
  public:
    XmlFileWriter();

    ~XmlFileWriter();

    void open(const std::string& filename);
    bool isOpen();
    void close();
    void startDocument(const std::string& encoding = "UTF-8");
    void startElement(const std::string& name);
    void writeAttribute(const std::string& name, const std::string& value);
    void writeElement(const std::string& name, const std::string& value);
    void endElement(const std::string& name = "");
    void endDocument();

  private:
    xmlTextWriterPtr w;
    std::stack<std::string> stack;
};


class TcxWriter : public Callback{
  public:
    TcxWriter(std::string filename, bool split_by_track);
    virtual ~TcxWriter();
    virtual void onWatch(const WatchInfo &);
    virtual void onWatchEnd(const WatchInfo &);
    virtual void onWorkout(const WorkoutInfo &i) ;
    virtual void onWorkoutEnd(const WorkoutInfo &);
    virtual void onTrack(const TrackInfo&);
    virtual void onTrackEnd(const TrackInfo&);
    virtual void onSample(const SampleInfo &i); 
    virtual void onReadBlocks(int id, int count);
    virtual void onReadBlock(int id, int addr, unsigned char* data);

  private:
    XmlFileWriter writer;
    std::string filename;
    WorkoutInfo current_wo;
    bool split_by_track;
};
