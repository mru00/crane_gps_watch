#! /bin/bash -xeu

# Copyright (C) 2014 mru@sisyphus.teil.cc

# called by the autoimporter.py when the watch is connected.
# starts the watch client to import new tracks, starts mytourbook


# adapt to your needs:
client=~/dev/crane_gps_watch_client/crane_gps_watch_client
tb_bin=mytourbook
tb=~/opt/tourbook/mytourbook/$tb_bin



notify-send --expire-time=5000 "receiving tcx track to $(pwd)"

trap '{
notify-send --expire-time=5000 --icon=gtk-dialog-error "failed to receive watch data";
}' ERR

sleep 3


$client --split --device $1 --verbose


if [[ -z "$(pgrep -f $tb)" ]]; then
  nohup $tb &
else
  wmctrl -a $tb_bin
fi

echo "completed"

notify-send --expire-time=5000 "finished loading tcx track"
