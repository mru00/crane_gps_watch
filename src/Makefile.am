# Copyright (C) 2014 mru@sisyphus.teil.cc

bin_PROGRAMS = crane_gps_watch_client
crane_gps_watch_client_SOURCES = crane_gps_watch_client.cpp \
 GpsLint.cpp XmlFileWriter.cpp ImageWriter.cpp \
 DebugWriter.cpp DataTypes.cpp MemoryBlock.cpp ImageLink.cpp SerialLink.cpp TcxWriter.cpp Watch.cpp \
 genx.c charProps.c \
 Callback.hpp GpsLint.hpp XmlFileWriter.hpp ImageWriter.hpp \
 DebugWriter.hpp DataTypes.hpp MemoryBlock.hpp ImageLink.hpp SerialLink.hpp TcxWriter.hpp Watch.hpp

crane_gps_watch_client_CXXFLAGS = -Wall -Wextra

if CROSS_COMPILING
crane_gps_watch_client_LDFLAGS = -static-libgcc -static-libstdc++ -static
endif


MOSTLYCLEANFILES = *.tcx


# gps_watch_monitor.py:
bin_SCRIPTS = gps_watch_monitor.py
CLEANFILES = $(bin_SCRIPTS)
EXTRA_DIST = gps_watch_monitor.py.in



do_subst = sed \
 -e 's,[@]datadir[@],$(datadir),g' \
 -e 's,[@]PACKAGE[@],$(PACKAGE),g' \
 -e 's,[@]PACKAGE_VERSION[@],$(PACKAGE_VERSION),g' \
 -e 's,[@]PACKAGE_STRING[@],$(PACKAGE_STRING),g' \
 -e 's,[@]PACKAGE_URL[@],$(PACKAGE_URL),g' \
 -e 's,[@]PACKAGE_BUGREPORT[@],$(PACKAGE_BUGREPORT),g' \
 -e 's,[@]PACKAGE_COPYRIGHT[@],mru@sisyphus.teil.cc,g' \
 -e 's,[@]PKGDATADIR[@],$(pkgdatadir),g' \
 -e 's,[@]HOST_CPU[@],$(host_cpu),g'

gps_watch_monitor.py: $(srcdir)/gps_watch_monitor.py.in
	$(do_subst) < $< > $@
	chmod +x $@

if CROSS_COMPILING
all: setup.ss
setup.ss: $(srcdir)/setup.ss.in
	$(do_subst) < $< > $@
endif

