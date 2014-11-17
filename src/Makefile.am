# Copyright (C) 2014 mru@sisyphus.teil.cc

bin_PROGRAMS = crane_gps_watch_client
crane_gps_watch_client_SOURCES = crane_gps_watch_client.cpp \
                                 GpsLint.cpp XmlFileWriter.cpp ImageWriter.cpp DebugWriter.cpp DataTypes.cpp MemoryBlock.cpp ImageLink.cpp SerialLink.cpp TcxWriter.cpp Watch.cpp \
                                 Callback.hpp GpsLint.hpp XmlFileWriter.hpp ImageWriter.hpp DebugWriter.hpp DataTypes.hpp MemoryBlock.hpp ImageLink.hpp SerialLink.hpp TcxWriter.hpp Watch.hpp

crane_gps_watch_client_CXXFLAGS = -Wall -Wextra -Weffc++ $(XML_CFLAGS)
crane_gps_watch_client_LDADD = $(XML_LIBS)


MOSTLYCLEANFILES = *.tcx


# gps_watch_monitor.py:
bin_SCRIPTS = gps_watch_monitor.py
CLEANFILES = $(bin_SCRIPTS)
EXTRA_DIST = gps_watch_monitor.py.in

do_subst = sed -e 's,[@]datadir[@],$(datadir),g' -e 's,[@]PACKAGE[@],$(PACKAGE),g' -e 's,[@]PKGDATADIR[@],$(pkgdatadir),g'

gps_watch_monitor.py: $(srcdir)/gps_watch_monitor.py.in
	$(do_subst) < $< > $@
	chmod +x $@

