# !/bin/bash

# pq***** <process1> [process2]
# 	pqcheck : check if process1 exists
# 	pqwait  : wait and ring once <process1> completes
# 	pqexec  : execute [process2]

if [ "$0" == "./pqcheck" ] && [ $# != 1 ]; then
	echo "USAGE: pqcheck <process1>"
	exit -1
elif [ "$0" == "pqwait" ] && [ $# != 1 ]; then
	echo "USAGE: pqwait <process1>"
	exit -2
elif [ "$0" == "pqexec" ] && [ $ != 2 ]; then
	echo "USAGE: pqexec <process1> <process2>"
	exit -3
fi

pqcmd="$0"
pqcmd=${pqcmd:4:1}
proc1=$1
proc2=$2
TRUE=1
FALSE=0

ps aux | grep $proc1 | grep -v grep | grep -v pbar > /dev/null
#ps aux | grep $proc1 | grep -v grep | grep -v pbar
existing=$?

execute_proc2 () {
	paplay /usr/share/sounds/ubuntu/stereo/service-login.ogg
	exec $proc2
}

if [ "$existing" == "$FALSE" ]; then
	if [ $pqcmd == 'c' ] || [ $pqcmd == 'w' ]; then
		paplay /usr/share/sounds/ubuntu/stereo/dialog-error.ogg
		zenity --info --text "process from $proc1 doesn't exist"
	elif [ $pqcmd == 'e' ]; then
		execute_proc2
	fi
fi

# Now the process is still ON, so we check for completion and sleep a second
# as long as the process is not completed

while [ "$existing" == "$TRUE" ]; do
	sleep 1
	ps aux | grep $proc1 | grep -v grep | grep -v pbar > /dev/null
	existing=$?
done

execute_proc2

exit 0

