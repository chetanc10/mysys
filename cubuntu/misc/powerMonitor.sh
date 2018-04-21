#!/bin/sh

###########################################################################
#
# Usage: system-low-battery
#
# Checks if the battery level is low. If “low_threshold” is exceeded
# a system notification is displayed, if “critical_threshold” is exceeded
# a popup window is displayed as well. If “OK” is pressed, the system
# shuts down after “timeout” seconds. If “Cancel” is pressed the script
# does nothing.
#
# This script is supposed to be called from a cron job.
#
###########################################################################

# This is required because the script is invoked by cron. Dbus information
# is stored in a file by the following script when a user logs in. Connect
# it to your autostart mechanism of choice.
#
# #!/bin/sh
# touch $HOME/.dbus/Xdbus
# chmod 600 $HOME/.dbus/Xdbus
# env | grep DBUS_SESSION_BUS_ADDRESS > $HOME/.dbus/Xdbus
# echo 'export DBUS_SESSION_BUS_ADDRESS' >> $HOME/.dbus/Xdbus
# exit 0
#
# if [ -r ~/.dbus/Xdbus ]; then
#   . ~/.dbus/Xdbus
# fi

low_threshold=62
critical_threshold=61
timeout=10
# shutdown_cmd='/usr/sbin/pm-hibernate'
shutdown_cmd='/usr/bin/whoami'

# get percentage of battery
level=$(echo -n `upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep "percentage" | sed -e 's|percentage: ||; s|%$||'`)
echo $level
# get state of battery: 'charging'/'discharging'
state=$(echo -n `upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep "state" | sed -e 's|state: ||; s|$||'`)
echo $state

if [ "$state" != "discharging" ]; then
	echo "primary check discharge match not success!"
	exit 0
fi

do_shutdown() {
	sleep $timeout && kill $zenity_pid 2>/dev/null

	state=$(echo -n `upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep "state" | sed -e 's|state: ||; s|$||'`)

	if [ "$state" != "discharging" ]; then
		echo "not discharging!"
		exit 0
	else
		echo "doomed"
		$shutdown_cmd
	fi
}

if [ "$level" -gt "$critical_threshold" ] && [ "$level" -le "$low_threshold" ]
then
	echo "check level may!"
	zenity --warning "Battery level low: $level%"
elif [ "$level" -le $critical_threshold ]
then
	echo "check level warning"
# 	DISPLAY=:0 zenity --question --ok-label 'Alright Alright!' --cancel-label 'Shut Up!' \
# 		--text "Battery level reached Critical $level% Idiota! System shuts down in 3 minutes unless U give me power..." &
	DISPLAY=:0 zenity --info --ok-label 'Alright Alright!' \
		--text "Battery level reached Critical $level% Idiota! System shuts down in 3 minutes unless U give me power..." &
	zenity_pid=$!

	do_shutdown &
	shutdown_pid=$!

# 	trap 'kill $shutdown_pid; echo "killed shutter"' 1

	if ! wait $zenity_pid; then
		echo "killing shutter?!(headbang)"
		kill $shutdown_pid 2>/dev/null
	fi
else
	echo "+1!"
fi

exit 0
