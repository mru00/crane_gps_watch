// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// simplistic interface to the gps watch
//

#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <sstream>


#include <cstring>



#include "SerialPort.hpp"

#include "SerialLink.hpp"




SerialLink::SerialLink(const std::string& filename) : filename(filename) {
    port.open(filename);
    std::cerr << "Connected to watch on " << filename << std::endl;
}


void SerialLink::readMemory(unsigned addr, unsigned count, unsigned char* it) {
    unsigned char opcode;
    std::vector<unsigned char> tx(8);
    std::vector<unsigned char> rx;
    tx[0] = addr;
    tx[1] = addr >> 8;
    tx[2] = addr >> 16;
    tx[3] = count;
    sendCommand(0x12, tx);
    receiveReply(opcode, rx);
    std::copy(rx.begin(), rx.end(), it);
}

std::string SerialLink::readVersion() {
    std::vector<unsigned char> version;
    unsigned char opcode;
    sendCommand(0x10, std::vector<unsigned char>());
    receiveReply(opcode, version);

    std::string str(version.size(), '\0');
    std::copy(version.begin(), version.end(), str.begin());
    return str;
}


void SerialLink::sendCommand(unsigned char opcode, std::vector<unsigned char> payload) {

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

void SerialLink::receiveReply(unsigned char& opcode, std::vector<unsigned char>& target) {
    unsigned short l;
    unsigned short cs;
    expect(0xa0);
    expect(0xa2);
    l = read() << 8;
    l += read();
    opcode = read();
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

unsigned short SerialLink::checksum(unsigned char opcode, std::vector<unsigned char> payload) {

    unsigned short cs = opcode;
    for (auto v : payload) {
        cs += v;
    }
    return cs;
}

