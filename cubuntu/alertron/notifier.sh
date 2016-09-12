
/bin/sh
#notify-send -u critical "$(cat ./content)"
paplay /usr/share/sounds/ubuntu/stereo/phone-incoming-call.ogg &
alert_message=`cat ./content`
DISPLAY=:0.0 zenity --info --text="$alert_message"
#DISPLAY=:0.0 zenity --text-info --width=100 --height=100 --filename="./content"
rm -rf ./content
exit

