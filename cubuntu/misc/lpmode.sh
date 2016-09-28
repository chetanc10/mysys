#!/bin/bash

# Kill all processes which are not to be run in low power mode (lpmode)

echo "killing processes.."
killall artha 2>/dev/null && echo "artha"
killall tomboy 2>/dev/null && echo "tomboy"
#killall dropbox 2>/dev/null && echo "dropbox"
