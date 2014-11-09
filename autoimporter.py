#! /usr/bin/env python


import os
import os.path
import pyudev
import pyudev.glib
import gobject
import glib

from pyudev.glib import GUDevMonitorObserver as MonitorObserver


HOME=os.path.expanduser("~")
DEVDIR=os.path.join(HOME, "dev/crane_gps_watch_client")
DESTDIR=os.path.join(HOME, "Documents/TcxTracks")
BIN=os.path.join(DEVDIR, "autoimport.sh")
DEVICE="/dev/ttyUSB0"

def device_added_callback(device, event, b):

    if event != "add":
        return
    if b['ID_MODEL'] != "CP2104_USB_to_UART_Bridge_Controller":
        return

    if not os.path.exists(DESTDIR):
        os.makedirs(DESTDIR)
    os.chdir(DESTDIR)
    os.system("gnome-terminal --window --profile=keep-open --title 'Importing gps track to %s' --command '%s %s'" % (DESTDIR, BIN, b['DEVNAME']))



context = pyudev.Context()
monitor = pyudev.Monitor.from_netlink(context)
monitor.filter_by(subsystem = 'tty')

observer = MonitorObserver(monitor)

observer.connect('device-event', device_added_callback)
monitor.start();


os.system("notify-send 'starting tcx auto importer'")

mainloop = gobject.MainLoop();
mainloop.run()
