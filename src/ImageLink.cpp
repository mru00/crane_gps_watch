// Copyright (C) 2014 - 2015 mru@sisyphus.teil.cc
//
// 
//

#include <fstream>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

#include "ImageLink.hpp"




ImageLink::ImageLink(const std::string& filename) {
    try {
        from_watch.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        from_watch.open(filename, std::ios_base::binary|std::ios_base::in);
    }
    catch (const std::ifstream::failure & e) {
        std::stringstream ss;
        ss << "Failed to open image file '" << filename << "' for reading: " << e.what();
        throw std::runtime_error (ss.str());
    }

    try {
        to_watch.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        to_watch.open(filename + ".to", std::ios_base::binary|std::ios_base::out);
    }
    catch (const std::ifstream::failure& e ) {
        throw std::runtime_error(std::string("failed to open image file to_watch for writing: ") + e.what());
     }
}

void ImageLink::readMemory(unsigned addr, unsigned count, unsigned char* it) {
    from_watch.seekg(addr);
    from_watch.read((char*)(it), count);
}

void ImageLink::writeMemory(unsigned addr, unsigned count, unsigned char* it) {
    to_watch.seekp(addr);
    to_watch.write((char*)(it), count);
}

std::string ImageLink::readVersion() {
    return "memory image device";
}

std::string ImageLink::readVersion2() {
    return "memory image device";
}

void ImageLink::clearFlash1() {
}


void ImageLink::clearFlash2(unsigned int) {
}
