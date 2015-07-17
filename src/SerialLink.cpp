// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// simplistic interface to the gps watch
//

#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <fstream>

#include <cstring>

#include "SerialPort.hpp"
#include "SerialLink.hpp"


std::ofstream f_tx;


SerialLink::SerialLink(const std::string& filename) : filename(filename) {
    port.open(filename);
    std::cerr << "Connected to watch on '" << filename << "'" << std::endl;

    f_tx.open("tx.txt");
}


void SerialLink::readMemory(unsigned addr, unsigned count, unsigned char* it) {
    std::vector<unsigned char> tx(8);
    std::vector<unsigned char> rx;
    tx[0] = addr;
    tx[1] = addr >> 8;
    tx[2] = addr >> 16;
    tx[3] = count;
    sendCommand(0x12, tx);
    receiveReply(0x13, rx);
    std::copy(rx.begin(), rx.end(), it);
}

void SerialLink::writeMemory(unsigned addr, unsigned count, unsigned char* it) {
    //std::cerr << "sending write 0x16 " << std::hex << addr << "/" << count << std::endl;
    std::vector<unsigned char> tx(4 + count);
    std::vector<unsigned char> rx;
    tx[0] = addr;
    tx[1] = addr >> 8;
    tx[2] = addr >> 16;
    tx[3] = count;
    std::copy(it, it+count, &tx[0]+4);
    if (0) {
        for (auto v : tx) {
            std::cout << std::setw(2) << std::setfill('0') << std::hex << (int) v << " ";
        }
        std::cout << std::endl;
    }
    sendCommand(0x16, tx);
    receiveReply(0x17, rx);
    // XXX assert rx.size == 0
}

std::string SerialLink::readVersion() {
    std::vector<unsigned char> version;
    sendCommand(0x10, std::vector<unsigned char>());
    receiveReply(0x11, version);

    std::string str(version.size(), '\0');
    std::copy(version.begin(), version.end(), str.begin());
    return str;
}


std::string SerialLink::readVersion2() {
    // This opcode is not implemented in older firmware versions.
    // refer to https://github.com/mru00/crane_gps_watch/issues/16
    std::vector<unsigned char> version;
    sendCommand(0x2c, std::vector<unsigned char>());
    receiveReply(0x2d, version);
    std::stringstream ss;
    for (auto v : version) {
        ss << "."  << std::setfill('0') << std::setw(2) <<  (int)v;
    }
    return ss.str();
}
void SerialLink::clearFlash1() {
    //std::cerr << "sending clear1 0x24" << std::endl;
    std::vector<unsigned char> tmp(0);
    sendCommand(0x24, std::vector<unsigned char>());
    receiveReply(0x25, tmp);
}
void SerialLink::clearFlash2(unsigned int segment) {
    //std::cerr << "sending clear2 0x14" << std::endl;
    std::vector<unsigned char> tmp(0);
    std::vector<unsigned char> data(3);
    // XXX copy segment to data
    data[0] = segment;
    data[1] = segment >> 8;
    data[2] = segment >> 16;
    sendCommand(0x14, data);
    receiveReply(0x15, tmp);
}

void SerialLink::setEpoEol(unsigned char year, unsigned char month, unsigned char day) {

    //std::cerr << "sending epo eol" << std::endl;
    std::vector<unsigned char> tmp(0);
    std::vector<unsigned char> data(3);
    // XXX copy segment to data
    data[0] = year;
    data[1] = month;
    data[2] = day;
    sendCommand(0x2e, data);
    receiveReply(0x2f, tmp);
}


// privates:

void SerialLink::sendCommand(const unsigned char opcode, const std::vector<unsigned char>& payload) {

    std::vector<unsigned char> frame(2+2+1+ payload.size() +2+2);
    std::vector<unsigned char>::iterator it = frame.begin();

    unsigned short l = payload.size() + 1;
    unsigned short cs = checksum(opcode, payload);
    *it++ = 0xa0;
    *it++ = 0xa2;
    *it++ = l >> 8 & 0xff;
    *it++ = l & 0xff;
    *it++ = opcode;
    std::copy(payload.begin(), payload.end(), it); 
    it += payload.size();
    *it++ = cs >> 8 & 0xff;
    *it++ = cs & 0xff;
    *it++ = 0xb0;
    *it++ = 0xb3;

    write(frame);

    int i = 0;
    for (auto v: frame) {
        f_tx << std::hex << std::setw(2) << std::setfill('0') << (int)v << " ";
        if ( ++i % 16 == 0 ) {
            f_tx << std::endl;
        }
    }
    f_tx << std::endl << std::endl;
}

void SerialLink::receiveReply(const unsigned char opcode, std::vector<unsigned char>& target) {
    unsigned short l;
    unsigned short cs;
    expect(0xa0, "frame leader 1");
    expect(0xa2, "frame leader 2");
    l = read() << 8;
    l += read();
    expect(opcode, "opcode");
    target.resize(l-1);
    read(target);
    cs = checksum(opcode, target);
    expect(cs >> 8 &0xff, "checksum 1");
    expect(cs & 0xff, "checksum 2");
    expect(0xb0, "frame trailer 1");
    expect(0xb3, "frame trailer 2");
}

unsigned char SerialLink::expect(unsigned char val, const std::string& desc) {
    unsigned char rcv = read();
    if ( rcv != val ) {
        std::stringstream ss;
        ss << "Serial link: protocol error in '"
          << desc << "'"
          << " expected: " << (int) val
          << " received: " << (int) rcv;
        throw std::runtime_error(ss.str());
    }
    return rcv;
}

void SerialLink::write(std::vector<unsigned char>& buf) {
    port.write(buf);
}

unsigned char SerialLink::read() {
    std::vector<unsigned char> buf(1);
    port.read(buf);
    return buf[0];
}

void SerialLink::read(std::vector<unsigned char>& buf) {
    port.read(buf);
}

unsigned short SerialLink::checksum(unsigned char opcode, const std::vector<unsigned char>& payload) {

    unsigned short cs = opcode;
    for (auto v : payload) {
        cs += v;
    }
    return cs & 0x7fff;
}

