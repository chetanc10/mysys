#!/bin/bash

name=$1

if [ "$name" == "" ]; then
	echo "Usage: pbar <process name>"
	echo "process name should be as is given in cmd in terminal"
	exit
fi

FALSE=0
TRUE=1

if [ "$2" == "-v" ]; then
	andprintto=""
else
	echo "devnull"
	andprintto="> /dev/null"
fi

pgrep $name $andprintout
ps aux | grep $name | grep -v grep | grep -v pbar $andprintout
terminated=$?
if [ "$terminated" == "$FALSE" ]; then
	ps aux | grep $name | grep -v grep | grep -v pbar
	echo -n "Shall I proceed to wait? [y|n]: "
	read yes
	if [ "$yes" != "y" ]; then
		exit 0
	fi
	#echo Waiting for $name to finish!
	while [ 1 ]
	do
		#ps aux | grep $name | grep -v grep | grep -v pbar $andprintout
		pgrep $name $andprintout
		terminated=$?
		if [ "$terminated" == "$TRUE" ]; then
			break
		fi
		sleep 1
	done
	#notify-send "process from $name completed"
	zenity --info --text "process from $name completed!" &
	paplay /usr/share/sounds/ubuntu/stereo/phone-incoming-call.ogg
	paplay /usr/share/sounds/ubuntu/stereo/phone-incoming-call.ogg
	paplay /usr/share/sounds/ubuntu/stereo/phone-incoming-call.ogg
	paplay /usr/share/sounds/ubuntu/stereo/phone-incoming-call.ogg
	paplay /usr/share/sounds/ubuntu/stereo/phone-incoming-call.ogg
else
	echo "$name: Process not found!"
fi

exit 0
