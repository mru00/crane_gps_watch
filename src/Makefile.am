# Copyright (C) 2014 mru@sisyphus.teil.cc

bin_PROGRAMS = crane_gps_watch_client
crane_gps_watch_client_SOURCES = crane_gps_watch_client.cpp \
                                 XmlFileWriter.cpp ImageWriter.cpp DebugWriter.cpp DataTypes.cpp MemoryBlock.cpp ImageLink.cpp SerialLink.cpp TcxWriter.cpp Watch.cpp \
                                 Callback.hpp XmlFileWriter.hpp ImageWriter.hpp DebugWriter.hpp DataTypes.hpp MemoryBlock.hpp ImageLink.hpp SerialLink.hpp TcxWriter.hpp Watch.hpp

crane_gps_watch_client_CXXFLAGS = -Wall -Wextra -Weffc++ $(XML_CFLAGS)
crane_gps_watch_client_LDADD = $(XML_LIBS)


MOSTLYCLEANFILES = *.tcx

