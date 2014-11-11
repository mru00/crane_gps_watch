Crane GPS Watch client v.0.1 (crane_gps_watch_client)
11 November 2014

Developer: Rudolf Mühlbauer <email>

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

	crane_gps_watch_client track_name - get track file from the watch

Resulting track file in tcx format you can convert into gpx format 
by means of gpsbabel or use tcx format as it is.

Enjoy!

Version History:

	0.1:	:initial release.
