
###############################
###############################
## my bash alias definitions ##
###############################
###############################

###############################
#### multi-purpose aliases ####
###############################

# reboot/poweroff
alias chav='sudo poweroff'
alias jiyo='sudo reboot'

# source code browsing
alias detag='rm -rf cscope.out; rm -rf tags'
alias retag='cscope -Rb && ctags -R'

###############################
#### path specific aliases ####
###############################

### Miscellaneous regulars ####
alias src='cd /home/vchn075/ChetaN/Refs'
alias scrap='cd /home/vchn075/ChetaN/Junk/scrap'
alias wrk='cd /home/vchn075/ChetaN/snips'

### Current Projects ###

# XILU_iOVS #
alias iovs='cd /home/vchn075/ChetaN/Projects/xilu_iovs'

# FSLU_NVME #
alias sfslu='cd /home/vchn075/ChetaN/Projects/fslu_nvme'

# NTGU_W710 #
#alias ntgl='cd /home/vchn075/ChetaN/Projects/ntgu_w710/WNAPxxx_80211N/sdk/build/projects/WAC710/linux-3.14.43'
#alias ntgu='cd /home/vchn075/ChetaN/Projects/ntgu_w710/WNAPxxx_80211N/sdk/build/projects/WAC710/uboot-2012.07'
#alias ntgs='cd /home/vchn075/ChetaN/Projects/ntgu_w710/WNAPxxx_80211N/sdk'
