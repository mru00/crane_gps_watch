# Copyright (C) 2014 mru@sisyphus.teil.cc


dist_pkgdata_SCRIPTS = gps_watch_onconnect.sh
dist_pkgdata_DATA = gps_watch_monitor.desktop


if CROSS_COMPILING

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

all: start_watch_client.bat icon.ico
start_watch_client.bat: $(srcdir)/start_watch_client.bat.in
	$(do_subst) < $< | unix2dos -n > $@

icon.ico: $(srcdir)/icon.ico.in
	cp $< $@

endif



