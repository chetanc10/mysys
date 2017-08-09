# !/bin/bash

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f /home/vchn075/ChetaN/snips/cubuntu/c10bashalias.sh ]; then
    . /home/vchn075/ChetaN/snips/cubuntu/c10bashalias.sh
fi

# Improve history file storage capability
unset HISTSIZE
unset HISTFILESIZE
HISTSIZE=5000
HISTFILESIZE=10000

# subversion editor setting
export SVN_EDITOR=vim

# bash default editor setting
export EDITOR=vim

# key-map to invoke "vim ."
bind -x '"\C-o":"vim ."'

# Simplify normal user prompt string
PROMPT_COMMAND='echo -ne "\033]0;${PWD}\007"'
PS1='${debian_chroot:+($debian_chroot)}\$ '
