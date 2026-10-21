#!/bin/bash

if [ -d "/opt/system/Tools/PortMaster/" ]; then
  controlfolder="/opt/system/Tools/PortMaster"
elif [ -d "/opt/tools/PortMaster/" ]; then
  controlfolder="/opt/tools/PortMaster"
elif [ -d "/roms/ports" ]; then
  controlfolder="/roms/ports/PortMaster"
 elif [ -d "/roms2/ports" ]; then
  controlfolder="/roms2/ports/PortMaster"
else
  controlfolder="/storage/roms/ports/PortMaster"
fi

source $controlfolder/control.txt

get_controls

GAMEDIR="/$directory/ports/mother_collection/mother"

cd $GAMEDIR

export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"

$ESUDO chmod 666 /dev/uinput
export LD_LIBRARY_PATH="$GAMEDIR/libs":"/lib":$LD_LIBRARY_PATH
export PATH="$GAMEDIR/libs:$PATH"
$GPTOKEYB "snes9x" &
exec /usr/bin/retroarch -c /run/muos/storage/info/config/retroarch.cfg --bps=./bandana.bps -L "./snes9x_libretro.so" "./mother.nes" 2>&1 | tee log.txt
tee ./log.txt
$ESUDO kill -9 $(pidof gptokeyb)
$ESUDO systemctl restart oga_events &
unset LD_LIBRARY_PATH
printf "\033c" >> /dev/tty1
cd /mnt/sdcard/ports/mother_collection/mother/ 
./Snes9x EarthBound Beginnings.sfc