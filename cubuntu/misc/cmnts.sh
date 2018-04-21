#!/bin/bash

# Invoke directly as cmntx.sh, this will install script-soft links 
# for various sda mountables on the system, but before that, we need to
# update the reqParts array with respective partition info.. -TODO
# User shall create the softlinks in usr/bin/ as below for now:
# ln -s cmntx.sh sda3y.sh; ln -s cmntx.sh sda3n.sh
# Automatic mountable detection might & respective soft-link comes later -TODO
# Invoke Syntax sda<sda_number><y|n>.sh
# Eg. sda7y.sh, sda10n.sh (Use softlinks or ./sdaxx.sh)
# Just having Mount Directory name and partition name same for easy map
# So, /media/vchn075/sdaxx is just a mount directory, not actual fs partition

declare -A reqparts=( ["c1"]="/dev/sda3" ["c2"]="/dev/sda4" )

[[ "$0" == *"cmnts.sh"* ]] && echo "Error: Can't use the script directly!" && exit -1;

Mntd=$(echo $(echo $(echo $0 | awk -F/ '{print $NF}')) | cut -d . -f 1)
Req=$(echo $Mntd | rev | cut -c1 | rev)
Mntd=$(echo $Mntd | cut -d . -f 1 | rev | cut -c 2- | rev)
Dev="${reqparts[$Mntd]}"

MntPath="/media/vchn075/$Mntd"

#echo "MntPath : $MntPath"
#echo "Dev     : $Dev"
#echo "Req     : $Req"

[ -z $Dev ] && echo "No Partition Device listed for $Mntd!" && exit -1
[ ! -e $Dev ] && echo "Device Partition '$Dev' doesn't exist!" && exit -2

cat /proc/mounts | grep $MntPath > /dev/null
[ $? -eq 1 ] && MntSt="n" || MntSt="y"
#echo "MntSt   : $MntSt"

#exit 0

if [ "$MntSt" == "n" ] && [ "$Req" == "y" ]; then
# 	echo Mounting!
	sudo mkdir -p $MntPath
	sudo mount $Dev $MntPath
	mret=$?
	if [ $mret -ne 0 ]; then
		echo Unable to mount Partition $Mntd!
		sudo rm -rf $MntPath
	fi
	exit $mret
elif [ "$MntSt" == "y" ] && [ "$Req" == "n" ]; then
# 	echo Unmouting!
	sudo umount $MntPath
	mret=$?
	if [ $mret == 0 ]; then
		sudo rm -rf $MntPath
	else
		echo Unable to unmount Partition $Mntd!
	fi
	exit $mret
fi

exit 0

