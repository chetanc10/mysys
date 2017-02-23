#!/bin/bash

Usage="Usage: stacker.sh <src-directory> [src-dir/sub-dir]"

[[ -z "$1" ]] && echo $Usage && exit -1

tel=/home/vchn075/Music/Miks_Telugu
hin=/home/vchn075/Music/Miks_Hindi
eng=/home/vchn075/Music/Miks_English

echo "For stacking choices:
	RET : do nothing and proceed to next entry
	d   : remove the current file
	t   : move the current file to directory" $tel"
	h   : move the current file to directory" $hin"
	e   : move the current file to directory" $eng"
	x|q : exit the utility right now!
	"

mkdir -p $tel
mkdir -p $hin
mkdir -p $eng

skip_done=0

stacker () {
	local entry
	for entry in "$1"/*
	do
		if [ -d "$entry" ]; then
			if [ -d "$2" ] && [ $skip_done == 0 ]; then
				#skip till $2 entry is found; we know we want to go beyond..
				[[ "$entry" != "$2" ]] && continue || skip_done=1
			else
				echo -n "Wanna stack the directory $entry? (y|q): "
				read answer
				[[ "$answer" == "y" ]] && stacker "$entry"
				[[ "$answer" == "q" ]] && echo "Exiting.." && exit 0
			fi
			continue
		fi
		cvlc $entry 1>/dev/null 2>/dev/null &
		echo -n "'$entry' Choice: "
		read answer
		pkill vlc

		## Using case-esac
		case $answer in
			d) rm -rf $entry && continue
				;;
			t) mv $entry $tel/ && continue
				;;
			h) mv $entry $hin/ && continue
				;;
			e) mv $entry $eng/ && continue
				;;
			x|q) echo "Exiting.." && exit 0
				;;
			c|*) [[ $(ls -A "$1" | wc -l) -eq 0 ]] && rmdir "$1"
				;;
		esac
	done
}

stacker "$@" && echo "stacker is done!"

