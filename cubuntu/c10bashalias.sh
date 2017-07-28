
###############################
###############################
## my bash alias definitions ##
###############################
###############################

###############################
#### multi-purpose aliases ####
###############################

# source code browsing
alias detag='rm -rf cscope.out; rm -rf tags'
alias retag='cscope -Rb && ctags -R'

###############################
#### path specific aliases ####
###############################

### Miscellaneous regulars ####
alias src='cd /home/vchn075/ChetaN/refs'
alias scrap='cd /home/vchn075/ChetaN/junk/scrap'
alias wrk='cd /home/vchn075/ChetaN/snips'
alias gitst='git status'

### Current projects ###

# WAC_720 #
alias wacu='cd /home/vchn075/ChetaN/projects/wac_720'

# XILU_iOVS #
alias iovs='cd /home/vchn075/ChetaN/projects/xilu_iovs'
alias dpdk_iovs='cd /home/vchn075/ChetaN/projects/xilu_iovs/svn/xilu_iovs/trunk/dpdk-16.04'

# FSLU_NVME #
alias sfslu='cd /home/vchn075/ChetaN/projects/fslu_nvme'

