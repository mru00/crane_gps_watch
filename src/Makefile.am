# Copyright (C) 2014 mru@sisyphus.teil.cc
#
#

bin_PROGRAMS = crane_gps_watch_client
crane_gps_watch_client_SOURCES = crane_gps_watch_client.cpp \
                                 ImageWriter.cpp DebugWriter.cpp DataTypes.cpp ImageLink.cpp SerialLink.cpp TcxWriter.cpp Watch.cpp \
                                 Callback.hpp DataTypes.hpp ImageWriter.hpp DebugWriter.hpp MemoryBlock.hpp ImageLink.hpp SerialLink.hpp TcxWriter.hpp Watch.hpp

crane_gps_watch_client_CXXFLAGS = -std=c++11 -Werror -Wall $(XML_CFLAGS) $(CXXFLAGS)
crane_gps_watch_client_LDADD = $(XML_LIBS)


