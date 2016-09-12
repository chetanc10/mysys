#!/bin/bash

# Kill all processes which are not to be run in low power mode (lpmode)

function lpmode {
echo "killing processes.."
	killall artha 2>/dev/null && echo "artha"
	#killall dropbox 2>/dev/null && echo "dropbox"
}
