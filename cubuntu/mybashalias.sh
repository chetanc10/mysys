
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
alias retag='rm -rf cscope.out; rm -rf tags; cscope -Rb; ctags -R'

# Mounting partitions
alias c1y='/home/vchn075/ChetaN/snips/cubuntu/mounters/mntC1.sh 1 && cd /media/vchn075/C1'
alias c1n='cd ; /home/vchn075/ChetaN/snips/cubuntu/mounters/mntC1.sh 0'
alias c2y='/home/vchn075/ChetaN/snips/cubuntu/mounters/mntC2.sh 1 && cd /media/vchn075/C2'
alias c2n='cd ; /home/vchn075/ChetaN/snips/cubuntu/mounters/mntC2.sh 0'

# 'Hugos' partition
alias hugo='/home/vchn075/ChetaN/snips/cubuntu/mounters/mntHugos.sh 1'
alias hugod='/home/vchn075/ChetaN/snips/cubuntu/mounters/mntHugos.sh 1 && cd /media/vchn075/Hugos/fslu_nvme_sdk/ear5/fslu_nvme_sdk_ear5/Layerscape2-SDK-20150828-yocto'
alias hngo='cd ; /home/vchn075/ChetaN/snips/cubuntu/mounters/mntHugos.sh 0'

# Process completion waiter
alias pbar='/home/vchn075/ChetaN/snips/cubuntu/misc/pbar.sh'

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
alias fslu='cd /media/vchn075/Hugos/fslu_nvme_sdk/ear5/fslu_nvme_sdk_ear5/Layerscape2-SDK-20150828-yocto/build_ls2085aissd_release/tmp/work/ls2085aissd-fsl-linux/linux-ls2-sdk/4.0-r0/git'
alias sfslu='cd /home/vchn075/ChetaN/Projects/fslu_nvme'

# NTGU_W710 #
#alias ntgl='cd /home/vchn075/ChetaN/Projects/ntgu_w710/WNAPxxx_80211N/sdk/build/projects/WAC710/linux-3.14.43'
#alias ntgu='cd /home/vchn075/ChetaN/Projects/ntgu_w710/WNAPxxx_80211N/sdk/build/projects/WAC710/uboot-2012.07'
#alias ntgs='cd /home/vchn075/ChetaN/Projects/ntgu_w710/WNAPxxx_80211N/sdk'
