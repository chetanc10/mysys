# !/bin/bash

# pq***** <process1> [process2]
# 	pqscan  : scan and see if process1 exists
# 	pqwait  : wait and ring once <process1> completes
# 	pqexec  : execute [process2]

pq_exists=0
pq_noproc=1

_pqscan () {
	pgrep $1 | grep -v grep > /dev/null
	local pqstate=$?
	if [ "$pqstate" == "$pq_exists" ]; then
		echo "Process $1 exists as shown below.." >&2
		ps aux | grep $1 | grep -v grep >&2
		return 0

	else
		echo "Process $1 not found!" >&2
		return 1

	fi
}

_pqwait () {
	_pqscan $1
	local pqstate=$?

	[ "$pqstate" != "$pq_exists" ] && return $pqstate

	if [ "$pqcmd" == "w" ]; then
		echo -n "Shall I proceed to wait? [y|n]: " >&2
		read yes
		[ "$yes" != "y" ] && exit 0
	fi

	while [ 1 ]
	do
		pgrep $1 | grep -v grep > /dev/null
		[ "$?" == "$pq_noproc" ] && break
		sleep 1
	done

	if [ "$pqcmd" == "w" ]; then
		zenity --info --text "process $1 completed!" &
		paplay /usr/share/sounds/ubuntu/stereo/phone-incoming-call.ogg &
	fi

	return $pqstate
}

_pqexec () {
	_pqwait $1
	local pqstate=$?
	[ "$pqstate" == "$pq_noproc" ] && (echo "So, just run '${@:2}' in terminal" >&2) && return $pqstate
	local proc2_with_args=${@:2}
	zenity --info --text "process $1 done. Starting $proc2_with_args.." &
	paplay /usr/share/sounds/ubuntu/stereo/service-login.ogg &
	echo "" >&2
	exec $proc2_with_args
}

_pqinstall () {
	echo "Installing pqscripts.." >&2
	sudo ln -s $0 ./pqscan.sh
	sudo ln -s $0 ./pqwait.sh
	sudo ln -s $0 ./pqexec.sh
	echo "Installed pqscan.sh, pqwait.sh, pqexec.sh in /bin/" >&2
}

if [[ "$0" != *"procs.sh"* ]]; then
	if [ -z "$1" ]; then
		echo "Usage: $0 <process>"
		echo "process name required"
		exit -1
	elif [[ "$0" == *"pqexec"* ]] && [ -z "$2" ]; then
		echo "Usage: $0 <process1> <process2>"
		echo "process2 name required; if process2 is user defined process/script, absolute path's needed"
		exit -2
	fi
fi

pqcmd="$0"
pqcmd=${pqcmd:4:1}

case "$pqcmd" in
	's' ) _pqscan $1
		;;
	'w' ) _pqwait $1
		;;
	'e' ) _pqexec $1 ${@:2}
		;;
	* ) _pqinstall
		;;
	esac

exit 0

