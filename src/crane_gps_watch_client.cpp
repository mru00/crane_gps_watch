// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//


#include <memory>

#include <getopt.h>

#include "../config.h"

#include "DataTypes.hpp"
#include "Callback.hpp"
#include "TcxWriter.hpp"
#include "DebugWriter.hpp"
#include "ImageWriter.hpp"
#include "Watch.hpp"
#include "SerialLink.hpp"
#include "ImageLink.hpp"



std::string format_date_filename() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    const size_t len_max = 1000;
    char buffer[len_max];
    std::strftime(buffer, len_max, "%Y-%m-%d_%I-%M-%S", &tm);
    return buffer;
}

int main(int argc, char** argv) {

    bool split_by_track = false;
    bool show_debug = false;
    int c;

    std::string from_image;
    std::string to_image;
    std::string device_fn = "/dev/ttyUSB0";
    std::string output_fn = format_date_filename() + ".tcx";
    opterr = 0;

    while (1) {
        static struct option long_options[] = {
              {"help", no_argument, 0, 'h'},
              {"output", required_argument, 0, 'f'},
              {"device", required_argument, 0, 'd'},
              {"from_image", required_argument, 0, 'i'},
              {"to_image", required_argument, 0, 't'},
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
              PACKAGE_STRING "\n"
              "crane_gps_watch_client --help\n"
              "\n"
              "crane_gps_watch_client [--output output-filename] [--device /dev/ttyUSB0 | --from_image image-file] [--to_image image-file] [--split] [--verbose]\n" 
              "\n"
              "See README.md or https://github.com/mru00/crane_gps_watch for details\n"
              "Send bugreports to " PACKAGE_BUGREPORT
              << std::endl;
            exit (0);
          case 'd':
            device_fn = optarg;
            break;
          case 's':
            split_by_track = true;
            break;
          case 'f':
            output_fn = optarg;
            break;
          case 'i':
            from_image = optarg;
            break;
          case 't':
            to_image = optarg;
            break;
          case 'v':
            show_debug = true;
            break;
          default:
            std::cerr << "unknown option" << std::endl;
            exit(1);
        }
    }

    std::shared_ptr<DeviceInterface> device;
    if (from_image != "") {
        device = std::make_shared<ImageLink>(from_image);
    }
    else {
        device = std::make_shared<SerialLink>(device_fn);
    }

    Watch i(device);

    if (to_image != "") {
        i.addRecipient(std::make_shared<ImageWriter>(to_image));
    }
    i.addRecipient(std::make_shared<TcxWriter>(output_fn, split_by_track));
    if (show_debug) {
        i.addRecipient(std::make_shared<DebugWriter>());
    }

    i.parse();

    return 0;
}
