#!/bin/bash

name=$1

if [ "$name" == "" ]; then
	echo "Usage: pbar.sh <process name>"
	echo "process name should be as is given in cmd in terminal"
	exit
fi

FALSE=0
TRUE=1

pgrep $name > /dev/null

if [ "$2" == "-v" ]; then
	ps aux | grep "$name" | grep -v grep | grep -v pbar.sh
else
	ps aux | grep "$name" | grep -v grep | grep -v pbar.sh > /dev/null
fi

terminated=$?
if [ "$terminated" == "$FALSE" ]; then
	echo -n "Shall I proceed to wait? [y|n]: "
	read yes
	if [ "$yes" != "y" ]; then
		exit 0
	fi
	echo "Going to wait now.."
	while [ 1 ]
	do
		ps aux | grep "$name" | grep -v grep | grep -v pbar.sh > /dev/null
		#pgrep $name > /dev/null
		terminated=$?
		if [ "$terminated" == "$TRUE" ]; then
			break
		fi
		sleep 1
	done
	#notify-send "process from $name completed"
	zenity --info --text "process from $name completed!" &
	paplay /usr/share/sounds/ubuntu/stereo/phone-incoming-call.ogg &
else
	echo "$name: Process not found!"
fi

exit 0
