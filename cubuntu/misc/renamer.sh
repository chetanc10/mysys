#!/bin/bash

Usage="Usage: renamer.sh <src-directory>"

[[ -z "$1" ]] && echo $Usage && exit -1

search_dir=$@

despacer () {
	#echo "Working on Folder: $1"
	cd "$1"
	local entry
	for entry in ./*
do
	[[ -d "$entry" ]] && despacer "$entry"
	#echo "despacing file: $entry in $PWD"
	rename 's/ /_/g' "$entry"
done
	#echo "going back"
	cd ..
}

debracker () {
	#echo "Working on Folder: $1"
	cd "$1"
	local entry
	for entry in ./*
do
	[[ -d "$entry" ]] && debracker "$entry"
	#echo "debracking file: $entry in $PWD"
	rename 's/\[.*\]//g' "$entry"
done
	#echo "going back"
	cd ..
}

echo -n "Want despacer? (y|n): "
read answer
[[ "$answer" == "y" ]] && despacer "$search_dir" && echo "despacer is done!"

echo -n "Want debracker? (y|n): "
read answer
[[ "$answer" == "y" ]] && debracker "$search_dir" && echo "debracker is done!"

