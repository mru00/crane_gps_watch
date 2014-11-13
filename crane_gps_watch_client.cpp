// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#include <getopt.h>


#include "DataTypes.hpp"
#include "Callback.hpp"
#include "TcxWriter.hpp"
#include "DebugWriter.hpp"
#include "Watch.hpp"



std::string format_date_filename() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    char buffer[PATH_MAX];
    std::strftime(buffer, PATH_MAX, "%Y-%m-%d_%I-%M-%S", &tm);
    return buffer;
}

int main(int argc, char** argv) {

    bool split_by_track = false;
    bool show_debug = false;
    int c;

    std::string device = "/dev/ttyUSB0";
    std::string output_fn = format_date_filename() + ".tcx";
    opterr = 0;

    while (1) {
        static struct option long_options[] = {
              {"help", no_argument, 0, 'h'},
              {"output", required_argument, 0, 'f'},
              {"device", required_argument, 0, 'd'},
              {"split", no_argument, 0, 's'},
              {"verbose", no_argument, 0, 'v'},
              {0, 0, 0, 0}
        };

        int option_index = 0;
        c = getopt_long(argc, argv, "h", long_options, &option_index);
        if (c == -1) break;

        switch (c) {
          case 'h':
            std::cerr <<
              "crane_gps_watch_client --help\n"
              "\n"
              "crane_gps_watch_client [--output output-filename] [--device /dev/ttyUSB0] [--split] [--verbose]\n" 
              "\n"
              "See https://github.com/mru00/crane_gps_watch for details"
              << std::endl;
            exit (0);
          case 'd':
            device = optarg;
            break;
          case 's':
            split_by_track = true;
            break;
          case 'f':
            output_fn = optarg;
            break;
          case 'v':
            show_debug = true;
            break;
          default:
            std::cerr << "unknown option" << std::endl;
            exit(1);
        }
    }


    Watch i(device);

    TcxWriter w(output_fn, split_by_track);
    DebugWriter d;

    i.addRecipient(&w);
    if (show_debug) {
        i.addRecipient(&d);
    }

    i.parse();

    return 0;
}
