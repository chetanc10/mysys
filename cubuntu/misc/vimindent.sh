#!/bin/bash

if [ ! -r "/home/vchn075/ChetaN/snips/cubuntu/misc/indent.vim" ]; then
	echo "MYERR: File not found at expected location: /home/vchn075/ChetaN/snips/cubuntu/misc/indent.vim"
	return -1;
fi

function vimindent {
	if [ -z "$1" ]; then
		echo "Specify one of the following:
		1. cp - for copying indent.vim 
		2. rm - for removing indent.vim"
		return -1;
	elif [ "$1" == "cp" ]; then
		echo "Copying indent.vim here.. Now, just do the following:
				1. > vim .
				2. In Command Mode - :source indent.vim
				3. Press <Ctrl> key along with l";
		cp /home/vchn075/ChetaN/snips/cubuntu/misc/indent.vim .;
		return 0;
	elif [ "$1" == "rm" ]; then
		echo "Removing indent.vim from current directory";
		rm -rf ./indent.vim;
		return 0;
	else
		echo "Unknown option: $1";
		return -2;
	fi
}
