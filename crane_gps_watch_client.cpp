// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#include <ostream>
#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <algorithm>

#include <cassert> 
#include <ctime> 
#include <climits>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdio.h>


#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>


class GpsLocation;
std::ostream& operator<<(std::ostream&, const GpsLocation&);

class GpsEle;
std::ostream& operator<<(std::ostream&, const GpsEle&);

class GpsTime;
std::ostream& operator<<(std::ostream&, const GpsTime&);

struct GpsLocation {
    int loc;
    GpsLocation& operator += (const GpsLocation& other) {
        loc += other.loc;
        return *this;
    }
    operator const std::string() const {
        std::ostringstream ss;
        ss << (*this);
        return ss.str();
    }
};

struct GpsTimeUpd {
    unsigned mm, ss;
};

struct GpsEle {
    unsigned short ele;
    GpsEle& operator += (const GpsEle& other) {
        ele += other.ele;
        return *this;
    }
    operator std::string() const {
        std::ostringstream ss;
        ss << (*this);
        return ss.str();
    }
};

struct GpsTime {
    unsigned char YY, MM, DD, hh, mm, ss;
    GpsTime& operator=(const GpsTimeUpd& other) {
        mm = other.mm;
        ss = other.ss;
        return *this;
    }
    operator std::string() const {
        std::ostringstream ss;
        ss << (*this);
        return ss.str();
    }
};

std::ostream& operator<< (std::ostream& s, const GpsLocation& l) {
    s << std::setprecision(15) << l.loc / 10000000.0;
    return s;
}

std::ostream& operator<< (std::ostream& s, const GpsEle& l) {
    s << l.ele;
    return s;
}

std::ostream& operator<< (std::ostream& s, const GpsTime& t) {
    s << std::dec
      << "20" << (int)t.YY << "-"  
      << std::setfill('0') << std::setw(2) << (int)t.MM << "-" 
      << std::setfill('0') << std::setw(2) << (int)t.DD << "T" 
      << std::setfill('0') << std::setw(2) << (int)t.hh << ":" 
      << std::setfill('0') << std::setw(2) << (int)t.mm << ":" 
      << std::setfill('0') << std::setw(2) << (int)t.ss << "Z";
    return s;
}

struct SampleInfo {
    SampleInfo() {}
    enum { Full, None, Diff, HrOnly, TimeOnly, End } type;
    GpsTime time;
    GpsTimeUpd time_upd;
    GpsLocation lon, lat;
    GpsEle ele;
    unsigned char hr;
    unsigned char fb;
};

struct WorkoutInfo {
    unsigned nsamples;
    unsigned lapcount;
    GpsTime start_time;
    GpsTime workout_time;
    unsigned profile;
    double total_km;
    double speed_avg;
    double speed_max;
    double calories;
};

struct WatchInfo {
    unsigned timezone;
    unsigned sample_interval;
    unsigned selected_profile;
    unsigned nblocks;
    std::vector<std::string> path_names;
    std::vector<std::string> profile_names;
    std::string version;
};


class Watch;

class WatchMemoryBlock {
  private:
    WatchMemoryBlock(const unsigned id, const unsigned count) 
      : id(id)
        , count(count)
        , memory(count*blockSize, 0) 
    {
    };

    WatchMemoryBlock(const WatchMemoryBlock&) = delete;

    void dump() {
        std::cerr << "memory:" << memory.size()  <<std::endl;
        mem_it_t it = memory.begin();
        while (it != memory.end()) {
            std::cerr << std::hex << (int)*it++ << ",";
        }
        std::cerr << std::endl;
    }
  public:
    typedef unsigned char byte_t;
    typedef std::vector<byte_t> mem_t;
    typedef mem_t::iterator mem_it_t;
    static const unsigned blockSize = 0x1000;

  private:
    const unsigned id; 
    const unsigned count;
    mem_t memory;
    friend class Watch;
};




class SerialLink {
  public:
    SerialLink() : fd(-1) {}
    void open(std::string devicename) {
        fd = ::open (devicename.c_str(), O_RDWR);
        if (fd == -1) {
            perror("failed to open serial device");
        }

        // http://trainingkits.gweb.io/serial-linux.html

        struct termios options;

        tcgetattr(fd, &options);
        cfsetispeed(&options, B115200);
        cfsetospeed(&options, B115200);
        cfmakeraw(&options);

        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_cc[VMIN] = 0;
        options.c_cc[VTIME] = 10;
        options.c_cflag |= (CLOCAL | CREAD);

        if(tcsetattr(fd, TCSANOW, &options)!= 0) {
            std::cerr << "error!" <<std::endl;
        }
    }

    void readMemory(unsigned addr, unsigned count, WatchMemoryBlock::mem_it_t& it) {
        unsigned char opcode;
        std::vector<unsigned char> tx(8);
        std::vector<unsigned char> rx;
        std::vector<unsigned char>::iterator tx_it = tx.begin();
        tx[0] = addr ;
        tx[1] = addr >> 8;
        tx[2] = addr >> 16;
        tx[3] = count;
        sendCommand(0x12, tx);
        receiveReply(opcode, rx);
        std::copy(rx.begin(), rx.end(), it);
    }

    std::string readVersion() {
        assert(fd != -1);
        std::vector<unsigned char> version;
        unsigned char opcode;
        sendCommand(0x10, std::vector<unsigned char>());
        receiveReply(opcode, version);

        std::string str(version.size(), '\0');
        std::copy(version.begin(), version.end(), str.begin());
        return str;
    }

  private:

    void sendCommand(unsigned char opcode, std::vector<unsigned char> payload) {
        assert (fd != -1);

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

    void receiveReply(unsigned char& opcode, std::vector<unsigned char>& target) {
        assert(fd!=-1);
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

    unsigned char expect(unsigned char val) {
        unsigned char rcv = read();
        assert (rcv == val);
        return rcv;
    }

    void write(std::vector<unsigned char>& buf) {
        ssize_t n;
        n = ::write(fd, &buf[0], buf.size());
        assert((size_t)n==buf.size());
    }

    unsigned char read() {
        assert(fd!=-1);
        unsigned char val;
        ssize_t n = ::read(fd, &val, 1);
        assert(n == 1);
        return val;
    }

    void read(std::vector<unsigned char>& buf) {
        assert(fd!=-1);
        ssize_t n = ::read(fd, &buf[0], buf.size());
        assert( (size_t)n == buf.size());
    }

    unsigned short checksum(unsigned char opcode, std::vector<unsigned char> payload) {
        unsigned short cs = opcode;
        for (auto v : payload) {
            cs += v;
        }
        return cs;
    }
  private:
    int fd;
};


class Callback {
  public:
    virtual void onWatch(const WatchInfo &) = 0;
    virtual void onWatchEnd(const WatchInfo &) = 0;
    virtual void onWorkout(const WorkoutInfo &) = 0;
    virtual void onWorkoutEnd(const WorkoutInfo &) = 0;
    virtual void onSample(const SampleInfo &) = 0;
    virtual void onReadBlocks(int id, int count) = 0;
    virtual void onReadBlock(int id, int addr) = 0;
};

class Broadcaster : public Callback {

  public:
    void addRecipient(Callback* c) { recipients.push_back(c); }

    virtual void onWatch(const WatchInfo &i) { 
        for (auto c : recipients) c->onWatch(i);
    }
    virtual void onWatchEnd(const WatchInfo &i) { 
        for (auto c : recipients) c->onWatchEnd(i);
    }
    virtual void onWorkout(const WorkoutInfo &i) {
        for (auto c : recipients) c->onWorkout(i);
    }
    virtual void onWorkoutEnd(const WorkoutInfo &i) {
        for (auto c : recipients) c->onWorkoutEnd(i);
    }
    virtual void onSample(const SampleInfo &i) {
        for (auto c : recipients) c->onSample(i);
    }
    virtual void onReadBlocks(int id, int count) {
        for (auto c : recipients) c->onReadBlocks(id, count);
    }
    virtual void onReadBlock(int id, int addr) {
        for (auto c : recipients) c->onReadBlock(id, addr);
    }
  private:
    std::vector<Callback*> recipients;
};


class Watch {

  public:
    Watch(){
    }

    void parse() {
        parseBlock0();
    }

    void addRecipient(Callback* c) {
        br.addRecipient(c);
    }
  private:

    void parseGpsEle(GpsEle& l, WatchMemoryBlock::mem_it_t it) {
        l.ele = *it++;
        l.ele+= *it++ << 8;
    }
    void parseGpsLocation(GpsLocation& l, WatchMemoryBlock::mem_it_t it) {
        l.loc = *it++;
        l.loc+= *it++ << 8;
        l.loc+= *it++ << 16;
        l.loc+= *it++ << 24;
    }
    void parseGpsTimeUpd(GpsTimeUpd& t, WatchMemoryBlock::mem_it_t it) {
        t.mm = *it++;
        t.ss = *it++;
    }
    void parseGpsTime(GpsTime& t, WatchMemoryBlock::mem_it_t it) {
        t.YY = *it++;
        t.MM = *it++;
        t.DD = *it++;
        t.hh = *it++;
        t.mm = *it++;
        t.ss = *it++;
        //assert (t.YY == 14);
    }
    void parseSample(SampleInfo& si, WatchMemoryBlock::mem_it_t& it) {

        auto type = *it;
        si.fb = type;
        switch (type) {
          case 0x00: 
              {
                si.type = SampleInfo::Full;
                si.hr = *(it+24);
                parseGpsTime(si.time, it+2);
                parseGpsLocation(si.lon, it+8);
                parseGpsLocation(si.lat, it+12);
                parseGpsEle(si.ele, it+16);
                it += 25;
                break;
              }
          case 0x01: 
              {
                si.type = SampleInfo::Diff;
                si.hr = *(it+20);
                parseGpsTimeUpd(si.time_upd, it+2);
                parseGpsLocation(si.lon, it+4);
                parseGpsLocation(si.lat, it+8);
                parseGpsEle(si.ele, it+12);
                it += 21;
                break;
              }
          case 0x02: 
              {
                si.type = SampleInfo::TimeOnly;
                si.hr = 0;
                parseGpsTimeUpd(si.time_upd, it+1);
                it += 3;
                break;
              }
          case 0x03: 
              {
                si.type = SampleInfo::HrOnly;
                si.hr = *(it+7);
                parseGpsTime(si.time, it+1);
                it += 8;
                break;
              }
          case 0x80: 
              {
                si.type = SampleInfo::None;
                si.hr = *(it+24);
                parseGpsTime(si.time, it+2);
                parseGpsLocation(si.lon, it+8);
                parseGpsLocation(si.lat, it+12);
                parseGpsEle(si.ele, it+16);
                it += 25;
                break;
              }
          case 0xff: 
              {
                si.type = SampleInfo::End;
                it += 0;
                break;
              }
          default: 
              {
                assert(0 && type);
              }
        }
    }
    void parseWO(WorkoutInfo& wo, int first, int count) {
        
        WatchMemoryBlock cb(first, count);
        readBlock(cb);

        WatchMemoryBlock::mem_it_t it = cb.memory.begin();

        wo.nsamples = *it++;  // [0..1]
        wo.nsamples+= *it++ << 8;

        wo.lapcount = *it++;  // [2]

        // [3..8]
        std::vector<unsigned char> reverse_time(6);
        std::reverse_copy(it+3, it +3+6, reverse_time.begin());
        parseGpsTime(wo.start_time, reverse_time.begin());
        it += 6;

        it += 3; // total workout time [9..12]
        it += 1; // profile [15]

        it = cb.memory.begin() + 16;
        it += 4; // km [16..19]
        it += 2; // speed avg [20..21]
        it += 2; // speed max [22..23]
        it += 8; // ? [24..32]
        
        it = cb.memory.begin() + 32;
        wo.calories = *it++; // [32..33]
        wo.calories+= *it++ << 8;

        br.onWorkout(wo);

        it = cb.memory.begin() + 0x1000;
        GpsTime t;
        GpsLocation lon;
        GpsLocation lat;
        GpsEle ele;
        for (unsigned i =0; i< wo.nsamples;i++) {
            SampleInfo si;
            parseSample(si, it);
            if (si.type == SampleInfo::Full || si.type == SampleInfo::HrOnly) {
                t = si.time;
            }
            else if (si.type == SampleInfo::Diff || si.type == SampleInfo::TimeOnly) {
                si.time = (t = si.time_upd);
            }
            else if (si.type == SampleInfo::End) {
                break;
            }

            if (si.type == SampleInfo::Full) {
                lon = si.lon;
                lat = si.lat;
                ele = si.ele;
            }
            else if (si.type == SampleInfo::Diff) {
                si.lon = (lon += si.lon);
                si.lat = (lat += si.lat);
                si.ele = (ele += si.ele);
            }
            else {
                si.lon.loc = 0;
                si.lat.loc = 0;
                si.ele.ele = 0;
            }

            br.onSample(si);

        }
    }
    void parseBlock0() {
        sl.open("/dev/ttyUSB0");

        std::string version = sl.readVersion();
        WatchMemoryBlock mb(0, 1);
        readBlock(mb);
        WatchInfo wi;

        wi.version = version;
        br.onWatch(wi);

        WatchMemoryBlock::mem_it_t mem_it = mb.memory.begin();
        unsigned char cur;
        unsigned char first;
        mem_it += 0x100;
        for (;;) {
            first = *mem_it++;
            if (first == 0xff) {
                break;
            }
            
            while (*mem_it != 0xff) {
                cur = *mem_it++;
            }
            WorkoutInfo wo;
            parseWO(wo, first, 1+cur-first);
            br.onWorkoutEnd(wo);
            mem_it ++;
        }
        
        br.onWatchEnd(wi);
    }

    void readBlock(WatchMemoryBlock& b) {
        // read b.count bytes from block# b.id
        const unsigned readSize = 0x80;
        const unsigned rcount = b.blockSize / readSize;
        
        br.onReadBlocks(b.id, b.count);

        WatchMemoryBlock::mem_it_t mem_it = b.memory.begin();
        for (unsigned block = 0; block < b.count; block++) {
            unsigned block_start = (b.id+block)*b.blockSize;
            br.onReadBlock(b.id+block, block_start);
            for (unsigned nbyte = 0; nbyte < rcount; nbyte++) {
                sl.readMemory(block_start + nbyte*readSize, readSize, mem_it);
                mem_it += readSize;
            }
        }
        //b.dump();
    }

  private:
    SerialLink sl;
    Broadcaster br;
};


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

class DebugWriter : public Callback {

  public:
    virtual void onWatch(const WatchInfo &i) {
        std::cout << "watch version: " << i.version << std::endl;
    }
    virtual void onWatchEnd(const WatchInfo &) {
        std::cout << "watch end" << std::endl;
    }
    virtual void onWorkout(const WorkoutInfo &)  {
        std::cout << "workout info" << std::endl;
    }
    virtual void onWorkoutEnd(const WorkoutInfo &)  {
        std::cout << "workout end" << std::endl;
    }
    virtual void onSample(const SampleInfo &i) {
        /*std::cout << "sample info: " <<  (int)i.fb << " = " << i.time 
          << " hr: " << (int)i.hr 
          << " lon: " << i.lon
          << " lat: " << i.lat
          << " ele: " << i.ele
          << std::endl;
          */
    }
    virtual void onReadBlocks(int id, int count) {
        std::cout << "reading block #" << id << " + " << count << std::endl;
    }
    virtual void onReadBlock(int id, int addr) {
        std::cout << "reading block #" << std::dec << id << " from 0x"<< std::hex << addr << std::endl;
    }
};


std::string format_date_filename() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    char buffer[PATH_MAX];
    std::strftime(buffer, PATH_MAX, "%Y-%m-%d_%I-%M-%S", &tm);
    return buffer;
}


int main() {

    TcxWriter w( format_date_filename() + ".tcx");
    DebugWriter d;

    Watch i;
    i.addRecipient(&w);
    i.addRecipient(&d);
    i.parse();

    return 0;
}
