#!/bin/bash

if [ "$1" == "" ]; then
	echo "Usage: ebar.sh <single script line>"
	exit
fi

inscript="$@"

SUCCESS=0

while [ 1 ]
do
	"$@" 1>/dev/null 2>/dev/null
	ret="$?"
	if [ "$ret" == "$SUCCESS" ]; then
		zenity --info --text "Done with inscript: $inscript" &
		paplay /usr/share/sounds/ubuntu/stereo/phone-incoming-call.ogg &
		exit 0
	fi
	sleep 3
done

exit 0
