#!/bin/bash

function ask() {
	Xdialog --title "Ready to flash?" --clear \
		--yesno "Flash file ($1) now?" 10 50


	case $? in
		0)
			dfu-programmer atmega32u4 erase
			dfu-programmer atmega32u4 flash $1
			ask $1
			;;
		*)
			echo "Not flashing now"
			exit 0;;
	esac
}

if [ -z "$2" ]; then
	ask $1
else
	dfu-programmer atmega32u4 erase
	dfu-programmer atmega32u4 flash $1
fi

