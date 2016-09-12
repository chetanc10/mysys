#!/bin/bash

name=$1

if [ "$name" == "" ]; then
	echo "Usage: pbar <process name>"
	echo "process name should be as is given in cmd in terminal"
	exit
fi

FALSE=0
TRUE=1

ps aux | grep $name | grep -v grep | grep -v pbar > /dev/null
#ps aux | grep $name | grep -v grep | grep -v pbar
terminated=$?
if [ "$terminated" == "$FALSE" ]; then
	#echo Now we wait!
	while [ 1 ]
	do
		ps aux | grep $name | grep -v grep | grep -v pbar > /dev/null
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
