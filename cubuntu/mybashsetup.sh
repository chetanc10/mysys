# !/bin/bash

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f /home/vchn075/ChetaN/snips/cubuntu/mybashalias.sh ]; then
    . /home/vchn075/ChetaN/snips/cubuntu/mybashalias.sh
fi

# file extracter used to extract from various compressed files
if [ -f /home/vchn075/ChetaN/snips/cubuntu/misc/extract.sh ]; then
    . /home/vchn075/ChetaN/snips/cubuntu/misc/extract.sh
fi

# vimindent feature being exported to this bash session
if [ -f /home/vchn075/ChetaN/snips/cubuntu/misc/vimindent.sh ]; then
    . /home/vchn075/ChetaN/snips/cubuntu/misc/vimindent.sh
fi

# low power mode startup
if [ -f /home/vchn075/ChetaN/snips/cubuntu/misc/lpmode.sh ]; then
	. /home/vchn075/ChetaN/snips/cubuntu/misc/lpmode.sh
fi

# subversion editor setting
export SVN_EDITOR=vim

# bash default editor setting
export EDITOR=vim

# key-map to invoke "vim ."
bind -x '"\C-o":"vim ."'

# for wireshark and tls
#export SSLKEYLOGFILE=/home/vchn075/.sslkeylog.log

