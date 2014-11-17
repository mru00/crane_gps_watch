Crane GPS Watch client v.0.1 (crane_gps_watch_client)
=====================================================
16 November 2014


Developer: Rudolf Mühlbauer <mru@sisyphus.teil.cc>

[![Build Status](https://travis-ci.org/mru00/crane_gps_watch.svg)](https://travis-ci.org/mru00/crane_gps_watch)


Crane GPS Watch client is a client for downloading track files in [TCX  format](http://en.wikipedia.org/wiki/Training_Center_XML)
from the following GPS watches.

Supported Watches:
* `1.20.22_KRIP0_00` [Crane GPS Watch from Aldi/Hofer](https://www.produktservice.info/20014414/20014414.html)
* `1.20.21_DLES0_00` [Conrad GPS Nav II](http://www.conrad.de/ce/de/product/372884/Multi-NAV-2-GPS-Pulsuhr-mit-Brustgurt-Schwarz)


I suspect that Latitude is the original manufacturer.
As the watch is commonly rebranded, the following Watches might just work:

Proably the same Watch:
* [Latitude NAV Master II](http://www.latitude.com.hk/dbimages/document/856/gps_series_catalog.pdf) (probably the original manufacturer)
* [Pyle Gps watch](http://www.pyleaudio.com/sku/PSWGP405BK/GPS-Watch-w-Coded-Heart-Rate-Transmission,-Navigation,-Speed,-Distance,-Workout-Memory,-Compass,--PC-link--(Black-Color))
* [Runtastic Gps Watch](https://www.runtastic.com/shop/en/runtastic-gps-watch-with-heart-rate-monitor)

The TCX format is used instead the popular GPX format, for the following reason:
The GPS watch is able to record workouts where only heartrate is available, no GPS.
It is not valid to create GPS records without coordinates. In TCX, this is possible.

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


Bug reports:
------------

Please report problems and bugs. If you get compiler errors, please send me the complete compiler output.
If the generated output files are wrong, please create an image (`--to_image`) and send me the image with a 
description of the expected output.


Installation:
-------------

This program uses libxml2. Please install libxml2 before building Crane GPS Watch client.

Crane GPS Watch client uses make to build:

1. Clone source code archive from githup repo:

	git clone https://github.com/mru00/crane_gps_watch.git
    

or download Zip file:

	wget https://github.com/mru00/crane_gps_watch/archive/master.zip
	unzip master.zip

2. cd to unpacking directory
3. build the software 

	./configure && make && make check

4. install

	sudo make install


See INSTALL for details. Note that it is possible to install without root rights, use the `./configure --prefix=...` option.


Please also have a look at `.travis.yml`, which describes a full build for the [Travis-ci](travis-ci.org) CI platform.

USAGE:
------

	crane_gps_watch_client --help      show the help screen

	crane_gps_watch_client 
    [--output "filename"]            override default filename, write single file for all tracks.
    [--split]                        write file for each track.
    [--device "/dev/tty..."]         override default device file.
    [--to_image filename]            write watch data to file <for testing>.
    [--from_image filename]          read watch data from file <for testing>.
    [--verbose]                      write debug output to console. repeat to get more output.

As default, the output file is written in the current working directory.
The filename is created from the current time and date. Use the `--output` option
to specify a different filename.


`--split` makes the program write a single .tcx for every track currently stored on the watch.
Default is to write a single .tcx file that contains all tracks. When this option is used, the `--output` option 
is ignored. The files are written in the current directory. The filename is taken from the start time of the workout track.
If a file already exists, it will be overwritten.

The `--device` option is used to specify the device file, default is '/dev/ttyUSB0'.


Resulting track file in TCX format you can convert into GPX format 
by means of [gpsbabel](http://www.gpsbabel.org/) or use TCX format as it is.

Enjoy!


Auto import
-----------

Note: this is experimental and not well documented and badly implemented and not intended for the end-user.

The script requires the package `python-pyudev` to be installed.

This package also ships the script `gps_watch_monitor.py` and `gps_watch_onconnect.sh`.


`gps_watch_monitor.py` is intended to be auto-started with your X session.
To enable auto-start copy the file `gps_watch_monitor.desktop` (usually in `/usr/local/share/crane_gps_watch_client`) 
to `~/.config/autostart`. 

It monitors the UDEV bus for
new connections on the USB bus. If the watch is connected, 
it will start `gps_watch_onconnect.sh` in a terminal window.

`gps_watch_onconnect.sh` can start the client to import the files, 
and execute additional tasks. To customize the default behaviour, 
copy the script (usually `/usr/local/share/crane_gps_watch_client/gps_watch_onconnect.sh`) 
to your home as: `~/.gps_watch_onconnect.sh` and 
modify it to your needs.


Version History:
----------------

	0.1:	:initial release.




See Also:
---------
* "Gps Visualizer": http://www.gpsvisualizer.com/
* "Sport route planner": http://www.sportrouteplanner.com/ 
* "My Tourbook": http://mytourbook.sourceforge.net/mytourbook/ works great with the TCX files.
* "Viking GPS data editor and analyzer" http://sourceforge.net/projects/viking/

* "Tcx files": http://en.wikipedia.org/wiki/Training_Center_XML


