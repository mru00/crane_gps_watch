// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#include <iostream>

#include "DataTypes.hpp"
#include "ImageWriter.hpp"

ImageWriter::ImageWriter(const std::string& filename) : f() {
    std::cerr << "Writing image to " << filename << std::endl;
    f.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    f.open(filename, std::ios_base::binary|std::ios_base::out);
}

void ImageWriter::onReadBlock(int , int addr, unsigned char* data, size_t size) {
    f.seekp(addr);
    f.write((char*) data, size);
}
