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


#include <cstring>



#include "SerialPort.hpp"

#include "SerialLink.hpp"


const bool inhibit_clear = false;


SerialLink::SerialLink(const std::string& filename) : filename(filename) {
    port.open(filename);
    std::cerr << "Connected to watch on '" << filename << "'" << std::endl;
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
    std::cerr << "sending write 0x16 " << addr << "/" << count << std::endl;
    if (inhibit_clear) {
        return;
    }

    std::vector<unsigned char> tx(8 + count);
    std::vector<unsigned char> rx;
    tx[0] = addr;
    tx[1] = addr >> 8;
    tx[2] = addr >> 16;
    tx[3] = count;
    std::copy(it, it+count, &tx[0]+4);
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
    std::cerr << "sending clear1 0x24" << std::endl;
    if (inhibit_clear) {
        return;
    }
    std::vector<unsigned char> tmp;
    sendCommand(0x24, std::vector<unsigned char>());
    receiveReply(0x25, tmp);
}
void SerialLink::clearFlash2() {
    std::cerr << "sending clear2 0x14" << std::endl;
    if (inhibit_clear) {
        return;
    }
    std::vector<unsigned char> tmp;
    std::vector<unsigned char> data(4, 0x00);
    sendCommand(0x14, data);
    receiveReply(0x15, tmp);
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
}

void SerialLink::receiveReply(const unsigned char opcode, std::vector<unsigned char>& target) {
    unsigned short l;
    unsigned short cs;
    expect(0xa0);
    expect(0xa2);
    l = read() << 8;
    l += read();
    expect(opcode);
    target.resize(l-1);
    read(target);
    cs = checksum(opcode, target);
    expect(cs >> 8 &0xff);
    expect(cs & 0xff);
    expect(0xb0);
    expect(0xb3);
}

unsigned char SerialLink::expect(unsigned char val) {
    unsigned char rcv = read();
    if ( rcv != val ) {
        throw std::runtime_error("Serial link: protocol error: unexpected answer");
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
    return cs;
}

