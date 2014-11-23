# Copyright (C) 2014 mru@sisyphus.teil.cc


dist_pkgdata_SCRIPTS = gps_watch_onconnect.sh
dist_pkgdata_DATA = gps_watch_monitor.desktop


if CROSS_COMPILING

all: start_watch_client.bat
start_watch_client.bat: $(srcdir)/start_watch_client.bat.in
	unix2dos -n $< $@

endif

