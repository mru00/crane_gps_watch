Crane GPS Watch client for Linux and Windows
===============================================

Copyright (C) 2014-2015 <mru@sisyphus.teil.cc>


[![Build Status](https://travis-ci.org/mru00/crane_gps_watch.svg)](https://travis-ci.org/mru00/crane_gps_watch)  [![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://cdn.rawgit.com/mru00/crane_gps_watch/master/donate.html)


Crane GPS Watch client is an unofficial, alternative open source Linux and Windows program for downloading GPS tracks from different GPS watches.

The program provides an alternative to the "GPS Master" software which is usually shipped with the watch. It does not provide many of the features of "GPS Master", but works cross platform and from the command line.

Supported Watches:
* `1.20.22_KRIP0_00` [unknown model](http://www.ebay.de/itm/GPS-Uhr-mit-Herzfrequnzmessung-Kompass-PC-Download-Pulsuhr-NEU/300968440724)
* `1.20.22_KRIP0_00` [Crane GPS Watch from Aldi/Hofer](https://www.produktservice.info/20014414/20014414.html)
* `1.20.21_KRIP0_00` [Ultrasport NavRun 500](http://www.ebay.de/itm/Ultrasport-GPS-Pulsuhr-NavRun-500-Uhr-Watch-mit-Brustgurt-/301393562885)
* `1.20.22_DLES0_00` [Conrad Multi NAV-3](http://www.produktinfo.conrad.com/datenblaetter/300000-324999/323447-an-01-de-GPS_SPORT_PULSUHR_MULTI_NAV_3.pdf)
* `1.20.21_DLES0_00` [Conrad GPS Nav II](http://www.conrad.de/ce/de/product/372884/Multi-NAV-2-GPS-Pulsuhr-mit-Brustgurt-Schwarz)
* `1.20.26_RUNT0_00` [Runtastic Gps Watch](https://www.runtastic.com/shop/en/runtastic-gps-watch-with-heart-rate-monitor)

As the watch is commonly rebranded, the following watches might just work:

* [Latitude NAV Master II](http://www.latitude.com.hk/dbimages/document/856/gps_series_catalog.pdf) (probably the original manufacturer)
* [Pyle Gps watch](http://www.pyleaudio.com/sku/PSWGP405BK/GPS-Watch-w-Coded-Heart-Rate-Transmission,-Navigation,-Speed,-Distance,-Workout-Memory,-Compass,--PC-link--(Black-Color))
* [Ascen Smart GPS watch GPS300](http://forums.watchuseek.com/f296/new-comer-ascen-gps-watch-comprehensive-review-pics-630057.html)
* [Tech4O Discover GPS](http://tech4o.shptron.com/p/discover-gps/tech4o_outdoor-multifunction_gps_discover-gps?pp=8)

I guess that all GPS watches delivered with "GPS Master" software work.


[Krippl Watches](http://www.krippl-watches.com/en/wrist-watch/) is probably the importer/brander of the Crane watch.
The site lists several other re-brands of the GPS watch:
* ASCOT
* TWINS
* Runtastic


Communication protocol: http://www.usglobalsat.com/downloads/SiRF_Binary_Protocol.pdf

Output format:
--------------

The TCX format is used instead the popular GPX format, for the following reason:
The GPS watch is able to record workouts where only heartrate is available, no GPS.
It is not valid to create GPS records without coordinates. In TCX, this is possible.

If you require a different output format, have a look at [gpsbabel](http://www.gpsbabel.org/).
The program can convert between many different formats (e.g. GPX). Or file a feature request.

Lap information is not yet stored in the output file. This is being worked on.
https://github.com/mru00/crane_gps_watch/issues/8



USAGE:
------

```bash
crane_gps_watch_client --help  # show the help screen

crane_gps_watch_client 
    [--clear]                  # delete workout info on watch.
    [--output "filename"]      # override default filename, write single file for all tracks.
    [--epo "epo-file"]         # download epo data
    [--split]                  # write file for each track.
    [--device "/dev/tty..."]   # override default device file / com port.
    [--to_image filename]      # write watch data to file <for testing>.
    [--from_image filename]    # read watch data from file <for testing>.
    [--verbose]                # write debug output to console. repeat to get more output.
```


As default, the output file is written in the current working directory.
The filename is created from the current time and date. Use the `--output` option
to specify a different filename.


`--split` makes the program write a single .tcx for every track currently stored on the watch.
Default is to write a single .tcx file that contains all tracks. When this option is used, the `--output` option 
is ignored. The files are written in the current directory. The filename is taken from the start time of the workout track.
If a file already exists, it will be overwritten.

The `--device` option is used to specify the device file, default is 'auto'. 'auto' enables
the automatic port detection. If auto-detect does not work for you, please file a bug report.

`--clear` clears the workouts from the watch *after* they are successfully transfered.

`--epo` allows you to download EPO data (MTK7d.EPO) for AGPS. This feature is still in development. 
I still don't know the proper way to obtain MTK7d.EPO files. The article https://geekblog.kevredon.org/?p=556 lists
one server.


Enjoy!


Disclaimer:
-----------

Please note that I (the developer) am not affiliated with any of the
manufacturers listed. The protocol is implemented completely clean-room
only by observing the behaviour of the watch, without any attempts to 
infringe copyrighted material. The original software and firmware 
was not reverse engineered in any way.


All trademarks mentioned belong to their owners, third party brands, product names, trade names, corporate names and company names mentioned may be trademarks of their respective owners or registered trademarks of other companies and are used for purposes of explanation and to the owner's benefit, without implying a violation of copyright law.


This software is distributed under the GNU General Public License.
This program is free software: you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published 
by the Free Software Foundation, either version 3 of the License, 
or (at your option) any later version. 

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the GNU General Public License for more details or file COPYING.


Bug reports & Feature requests:
-------------------------------

Please report problems, bugs, feature requests and success 
messages either via the
github issue tracker or mail.

If the generated output files are wrong, please create an image (`--to_image`) and send me the image with a 
description of the expected output.


I can only test 32bit executables, but users built and run successfully on 64bit. If you have issues on 64bit systems, please report them.

Installation:
-------------

Crane GPS Watch client uses make to build:

```bash
# Clone source code archive from githup repo:
git clone https://github.com/mru00/crane_gps_watch.git

# or download Zip file:

wget https://github.com/mru00/crane_gps_watch/archive/master.zip
unzip master.zip


cd crane_gps_watch_client

# build the software
./configure
make

# install the software
sudo make install
```

Make sure you have the rights to access the serial port. Usually you have to
be member of the `dialout` group (debian based distros). Consult a 
[search engine](https://duckduckgo.com/?q=serial+port+access+group+linux) 
to find a solution for other distros and let me know so I can update this documentation.

`make check` can be used to run the test suite. The test suite requires 
additional programs to be installed: `xmllint` from `libxml2-utils`.

See INSTALL for details. Note that it is possible to install without root rights, use the `./configure --prefix=...` option.


Please also have a look at `.travis.yml`, which describes a full build for the 
[Travis-CI](https://travis-ci.org/mru00/crane_gps_watch) platform, including all dependencies.



Cross compilation for windows:
------------------------------


The program also works under windows.
Windows installers are available in the `release` directory.

The program works using Wine, except access to the com port. The testsuite on Travis
also builds and checks the windows executable with the same testcases.

To build for windows, use `mingw-w64`:

```bash
sudo apt-get install mingw-w64
./configure --host i686-w64-mingw32 --build i686-linux-pc
wine ./src/crane_gps_watch_client.exe --help
```


Auto import
-----------

The script requires the package `python-pyudev` to be installed.

This repository also ships the scripts `gps_watch_monitor.py` and `gps_watch_onconnect.sh`.


`gps_watch_monitor.py` is intended to be auto-started with your X session.
To enable auto-start copy the file `gps_watch_monitor.desktop` (usually in 
`/usr/local/share/crane_gps_watch_client` after installation) 
to `~/.config/autostart`.

It monitors the UDEV bus for
new connections on the USB bus. If the watch is connected, 
it will start `gps_watch_onconnect.sh` in a terminal window.

`gps_watch_onconnect.sh` can start the client to import the files, 
and execute additional tasks. To customize the default behaviour, 
copy the script (usually `/usr/local/share/crane_gps_watch_client/gps_watch_onconnect.sh`) 
to your home as: `~/.gps_watch_onconnect.sh` and 
modify it to your needs.


TODO:
-----

WIP:
* get more details about the watch itself (manufacturer, architecture), firmware upgrade server...


In testing:
* AGPS download
* Auto-scan for serial port
* clear watch memory

Low-prio:
* find proper name for the project
* Implement filters (e.g. correct altitude data from online sources)
* http://www.movable-type.co.uk/scripts/latlong.html for distances in tcx files. would be required for turtle sports

See Also:
---------

GPS Software:

* [Gps Visualizer](http://www.gpsvisualizer.com/)
* [Sport route planner](http://www.sportrouteplanner.com/)
* [My Tourbook](http://mytourbook.sourceforge.net/mytourbook/) works great with the TCX files. I like it.
* [Viking GPS data editor and analyzer](http://sourceforge.net/projects/viking/)
* [Alternatives to My tourbook](http://alternativeto.net/software/my-tourbook/)
* [Turtle Sport](http://turtlesport.sourceforge.net/EN/home.html) does currently not work with the tcx files, due to a bug. Use gpsbabel tcx->gpx and import gpx instead.
* [gpsbabel](http://www.gpsbabel.org/)
* [serial port monitor](http://www.serial-port-monitor.com/)

Technical links:

* [Tcx files](http://en.wikipedia.org/wiki/Training_Center_XML)
* [sirf](http://www.usglobalsat.com/downloads/SiRF_Binary_Protocol.pdf)
* [EPO format](ftp://ftp.leadtek.com/gps/9023/G-monitor%203.26/User%20Manual/EPO%20Format%20and%20Protocol/LEADTEK%20EPO%20Format%20and%20Protocol%20Reference%20Manual%20V1.0.pdf)

Similar projects:

* [Kalenji gps watch reader](https://code.google.com/p/kalenji-gps-watch-reader), support for W Kalenji 300, CW Kalenji 700, On Move 500, On Move 700, Kaymaze 700
* [crane-gps-watch](https://github.com/mru00/GpsWatch), reverse engineering repository of this project, written in perl, easy to hack&test stuff.


Uses:
* [genx xml library](http://www.tbray.org/ongoing/When/200x/2004/02/20/GenxStatus).
* Inno setup


Thanks:
-------

* Yarema
* Wolfram
* Peter

