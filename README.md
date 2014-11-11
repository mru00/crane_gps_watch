Crane GPS Watch client v.0.1 (crane_gps_watch_client)
11 November 2014

Developer: Rudolf Mühlbauer <mru@sisyphus.teil.cc>

Crane GPS Watch client is a client for downloading track files in tcx format
from the GPS watch like Crane from Aldi store or GPS Nav II from Conrad store.


This software is distributed under the GNU General Public License.
This program is free software: you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published 
by the Free Software Foundation, either version 3 of the License, 
or (at your option) any later version. 

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the GNU General Public License for more details or file COPYING.


This program uses libxml2. Please install libxml2 before building Crane GPS Watch client.

Crane GPS Watch client uses make to build:

1. Download source code archive from githup repo:
git clone git://github.com/mru00/crane_gps_watch
2. cd to unpacking directory
3. make

Resulting binary file crane_gps_watch_client you can copy 
to destination folder e.g. /usr/bin or /usr/local/bin.


USAGE:

	crane_gps_watch_client --help      show the help screen

	crane_gps_watch_client 
    [--output "filename"]            override default filename
    [--split]                        write file for each track
    [--device "/dev/tty..."]         override default device file

As default, the output file is written in the current working directory.
The filename is created from the current time and date. Use the `--output` option
to specify a different filename.

The `--device` option is used to specify the device file, default is '/dev/ttyUSB0'.

`--split` makes the program write a single .tcx for every track currently stored on the watch.
Default is to write a single .tcx file that contains all tracks. When this option is used, the `--output` option 
is ignored. The files are written in the current directory. The filename is taken from the start time of the workout track.
If a file already exists, it will be overwritten.

Resulting track file in tcx format you can convert into gpx format 
by means of gpsbabel or use tcx format as it is.

Enjoy!

Version History:

	0.1:	:initial release.

