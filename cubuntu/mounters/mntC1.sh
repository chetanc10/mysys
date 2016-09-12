#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Illegal number of parameters"
	exit 1
fi

MountRequest=$1
Mounted=0
NotMounted=1
MOUNT=1
UNMOUNT=0

cat /proc/mounts | grep C1 > /dev/null
MountStatus=$?
if [ $MountStatus -eq $NotMounted ] && [ $MountRequest -eq $MOUNT ]; then
# Mount the drive
# 	echo Mounting!
	sudo mkdir -p /media/vchn075/C1
	sudo mount /dev/sda6 /media/vchn075/C1
	mret=$?
	if [ $mret -ne 0 ]; then
		echo Unable to mount /dev/sda6!
		sudo rm -rf /media/vchn075/C1
	fi
	exit $mret
elif [ $MountStatus -eq $Mounted ] && [ $MountRequest -eq $UNMOUNT ]; then
# Unmount the drive
# 	echo Unmouting!
	sudo umount /media/vchn075/C1
	mret=$?
	if [ $mret == 0 ]; then
		sudo rm -rf /media/vchn075/C1
	else
		echo Unable to unmount /dev/sda6!
	fi
	exit 0
fi

exit 0
