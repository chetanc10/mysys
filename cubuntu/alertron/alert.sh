
function display_help() {
	echo -e "\nUsage: alert.sh <time-options> ["Alert Message"]
	time-options are as below:
	HH:MM<AM or PM>         - scheduled for today
	HH:MM <AM or PM>        - scheduled for tomorrow
	noon, midnight, teatime, tomorrow, noon tomorrow,
	next week, next monday, friday, august, 2:30AM next monday,
	alert-message can be any combinations of symbols, letters, spaces 
	enclosed within quotation marks.
	"
}

if [ "$1" == "--help" ]; then
	display_help
	exit 0
elif [ "$1" == "" ]; then
	echo -n "alert time setting: "
	read alert_time
	echo -n "alert message: "
	read alert_message
	echo $alert_message > content
else
	alert_time=$1
	if [ "$2" == "" ]; then
		echo "Unknown Alert Boss!" > content
	else
		echo $2 > content
	fi
fi

at $alert_time -f ./notifier.sh 2>/dev/null

if [ "$?" != 0 ]; then
	echo "Invalid Arguments! Try 'alert.sh --help'"
	exit -1
fi

unset aler_time
unset alert_message
unset display_help

