#!/bin/bash

init() {
    clear
    date
    echo 'Welcome to ALSA volume manager!'
    echo 'What do you want to do?'
    echo '1. Volume Up'
    echo '2. Volume Down'
    echo '3. Set volume'
    echo '4. Get current volume info'
    read -p 'Choose 1-4: ' input
}

# Volume Up
volume_up() {
    cmd=$(amixer set Master 10%+)
    echo 'Your volume has increased by 10%'
}

# Volume Down
volume_down() {
    cmd=$(amixer set Master 10%-)
    echo 'Your volume decreased by 10%'
}


while true;
do
    init

    if [ $input -eq 1 ]
    then
        volume_up

    elif [ $input -eq 2 ]
    then
        volume_down value

    elif [ $input -eq 3 ]
    then
	read -p 'Set your volume into (in %): ' value
        amixer set Master $value%

    elif [ $input -eq 4 ]
    then
        echo 'Here is some detail on your sound'
        amixer get Master

    else
        echo 'Choose number between 1-3'

    fi
    read -p 'Press [Enter] key to continue...'

done
