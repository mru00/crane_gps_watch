Crane GPS Watch client v.0.1 (crane_gps_watch_client)
=====================================================
11 November 2014


Developer: Rudolf Mühlbauer <mru@sisyphus.teil.cc>

[![Build Status](https://travis-ci.org/mru00/crane_gps_watch.svg)](https://travis-ci.org/mru00/crane_gps_watch)


Crane GPS Watch client is a client for downloading track files in [TCX  format](http://en.wikipedia.org/wiki/Training_Center_XML)
from the following GPS watches.

Supported Watches:
* [Crane GPS Watch from Aldi/Hofer](https://www.produktservice.info/20014414/20014414.html)
* [Conrad GPS Nav II](http://www.conrad.de/ce/de/product/372884/Multi-NAV-2-GPS-Pulsuhr-mit-Brustgurt-Schwarz)
* [Runtastic Gps Watch](https://www.runtastic.com/shop/en/runtastic-gps-watch-with-heart-rate-monitor)


I suspect that Latitude is the original manufacturer.
As the watch is commonly rebranded, the following Watches might just work:

Proably the same Watch:
* [Latitude](http://latitude.com.hk/product.asp)
* [Pyle Gps watch](http://www.pyleaudio.com/sku/PSWGP405BK/GPS-Watch-w-Coded-Heart-Rate-Transmission,-Navigation,-Speed,-Distance,-Workout-Memory,-Compass,--PC-link--(Black-Color))

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



Installation:
-------------

This program uses libxml2. Please install libxml2 before building Crane GPS Watch client.

Crane GPS Watch client uses make to build:

1. Download source code archive from githup repo:

    git clone https://github.com/mru00/crane_gps_watch.git

or

    wget https://github.com/mru00/crane_gps_watch/archive/master.zip
    unzip master.zip

2. cd to unpacking directory
3. build the software 

    ./configure && make

4. install

    make install



Please also have a look at `.travis.yml`, which describes a full build for the [Travis-ci](travis-ci.org) CI platform.

USAGE:
------

	crane_gps_watch_client --help      show the help screen

	crane_gps_watch_client 
    [--output "filename"]            override default filename
    [--split]                        write file for each track
    [--device "/dev/tty..."]         override default device file
    [--to_image filename]            write watch data to file <for testing>
    [--from_image filename]          read watch data from file <for testing>
    [--verbose]                      write debug output to console

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


