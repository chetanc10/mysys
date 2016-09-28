# !/bin/bash

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f /home/vchn075/ChetaN/snips/cubuntu/mybashalias.sh ]; then
    . /home/vchn075/ChetaN/snips/cubuntu/mybashalias.sh
fi

# subversion editor setting
export SVN_EDITOR=vim

# bash default editor setting
export EDITOR=vim

# key-map to invoke "vim ."
bind -x '"\C-o":"vim ."'

