#!/bin/bash

declare -a c10utils=(vim git at subversion skype meld vlc ssh tomboy cscope exuberant-ctags nmap openssh-server openssh-client gparted unity-tweak-tool synaptic curl wifi-radar wireshark)

declare -a c10libs=(libpcap-dev)

echo "Installer program for basic system utilities and libraries. Enter x during any stage of question from me to kill me"

for i in "${c10utils[@]}"
do
	echo -ne "\n\n****Install $i ? (y|n): "
	read answer
	[[ "$answer" == "n" ]] && continue;
	[[ "$answer" == "x" ]] && exit 0;
	if [ "$i" == "skype" ]; then
		echo "Skype installation: Do it yourself for now"
		continue;
	fi
	echo -e "Installing $i"
	sudo apt-get install $i
done

echo -ne "\nProceeed to lib Installations? (y|n): "
read answer
[[ "$answer" != "y" ]] && exit 0;
[[ "$answer" == "x" ]] && exit 0;

for i in "${c10libs[@]}"
do
	echo -ne "\n\n****Install $i ? (y|n): "
	read answer
	[[ "$answer" == "n" ]] && continue;
	[[ "$answer" == "x" ]] && exit 0;
	echo -e "Installing $i"
	sudo apt-get install $i
done

