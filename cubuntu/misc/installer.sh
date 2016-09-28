#!/bin/bash
echo "Want to install openssh-server?"
read answer
if [ "$read" == "yes" ]; then
	echo "u said yes!"
fi

## declare an array variable
##declare -a arr=("element1" "element2" "element3")
#
## now loop through the above array
#for i in "${arr[@]}"
#do
   #echo "$i"
   ## or do whatever with individual element of the array
#done

#sudo apt-get install vim tomboy ssh subversion nmap wifi-radar libusb-1.0-0-dev synapse openssh-server openssh-client unity-tweak-tool exuberant-ctags cscope curl
