#!/bin/bash

perc=$(echo -n `upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep "percentage" | sed -e 's|percentage: ||; s|%$||'`)

#perc=$(echo -n `upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep "state" | sed -e 's|state: ||; s|$||'`)
echo "$perc"

# notify-send "Battery level is low: $perc%"

zenity --question --ok-label 'Alright' --cancel-label 'Hmph' --text "Battery level is : $perc%"
