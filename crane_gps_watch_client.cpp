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

    int c;

    std::string output_fn = format_date_filename() + ".tcx";
    opterr = 0;

    while (1) {
        static struct option long_options[] = {
              {"help", no_argument, 0, 'h'},
              {"output", required_argument, 0, 'f'},
              {0, 0, 0, 0}
        };

        int option_index = 0;
        c = getopt_long(argc, argv, "h", long_options, &option_index);
        if (c == -1) break;

        switch (c) {
          case 'h':
            std::cerr << "help" << std::endl;
            exit (0);
          case 'f':
            output_fn = optarg;
            break;
          default:
            std::cerr << "unknown option" << std::endl;
            exit(1);
        }
    }

    TcxWriter w(output_fn);
    DebugWriter d;

    Watch i;
    i.addRecipient(&w);
    i.addRecipient(&d);
    i.parse();

    return 0;
}
