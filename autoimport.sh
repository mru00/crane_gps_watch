#! /bin/bash -xeu


notify-send "receiving tcx track to $(pwd)"

~/dev/crane_gps_watch_client/crane_gps_watch_client --device $1

tb=~/opt/tourbook/mytourbook/mytourbook

if [[ -z "$(pgrep -f $tb)" ]]; then
  nohup $tb &
else
  wmctrl -a mytourbook
fi

echo "completed"

notify-send "finished tcx track"
