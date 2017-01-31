#!/bin/bash

declare -a cutils=(vim git at subversion skype meld vlc ssh tomboy cscope exuberant-ctags nmap openssh-server openssh-client gparted unity-tweak-tool synaptic curl wifi-radar wireshark qemu-system-x86)

declare -a clibs=(libpcap-dev)

echo "Installer program for basic system utilities and libraries. Enter x during any stage of question from me to kill me"

for i in "${cutils[@]}"
do
	echo -ne "\n\n****Install $i ? (y|n): "
	read answer
	[[ "$answer" == "n" ]] && continue;
	[[ "$answer" == "x" ]] && exit 0;
	if [ "$i" == "skype" ]; then
		echo -ne "Skype installation. DIY. Download the setup file fir this distro and install and press enter here once done.. "
		read answer
		continue;
	fi
	echo -e "Installing $i"
	sudo apt-get install $i
done

echo -ne "\nProceed to lib Installations? (y|n): "
read answer
[[ "$answer" != "y" ]] && exit 0;
[[ "$answer" == "x" ]] && exit 0;

for i in "${clibs[@]}"
do
	echo -ne "\n\n****Install $i ? (y|n): "
	read answer
	[[ "$answer" == "n" ]] && continue;
	[[ "$answer" == "x" ]] && exit 0;
	echo -e "Installing $i"
	sudo apt-get install $i
done

