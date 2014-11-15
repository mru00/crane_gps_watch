// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#include <iostream>

#include "DataTypes.hpp"
#include "ImageWriter.hpp"

ImageWriter::ImageWriter(const std::string& filename) {
    std::cerr << "Writing image to " << filename << std::endl;
    f.open(filename, std::ios_base::binary|std::ios_base::out);
}

void ImageWriter::onWatch(const WatchInfo &i) { }
void ImageWriter::onWatchEnd(const WatchInfo &) { }
void ImageWriter::onWorkout(const WorkoutInfo &i)  { }
void ImageWriter::onWorkoutEnd(const WorkoutInfo &)  { }
void ImageWriter::onTrack(const TrackInfo &i)  { }
void ImageWriter::onTrackEnd(const TrackInfo &)  { }
void ImageWriter::onSample(const SampleInfo &i) { }
void ImageWriter::onReadBlocks(int id, int count) { }
void ImageWriter::onReadBlock(int id, int addr, unsigned char* data) {
    f.seekp(addr);
    f.write((char*) data, 0x1000);
}
