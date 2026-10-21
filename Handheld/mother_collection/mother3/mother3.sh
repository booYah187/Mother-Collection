 #!/bin/bash

PORTNAME="Mother 3"

if [ -d "/opt/system/Tools/PortMaster/" ]; then
  controlfolder="/opt/system/Tools/PortMaster"
elif [ -d "/opt/tools/PortMaster/" ]; then
  controlfolder="/opt/tools/PortMaster"
else
  controlfolder="/roms/ports/PortMaster"
fi

source $controlfolder/control.txt
get_controls

CUR_TTY=/dev/tty0
$ESUDO chmod 666 $CUR_TTY

GAMEDIR="/$directory/ports/mother_collection/mother3"

#export GBARECOMP_SELFHEAL=0
#export SDL_VIDEODRIVER=kmsdrm

export SDL_VIDEO_EGL_NO_DEVICE_ENUMERATION=1
export SDL_VIDEO_FORCE_RENDERER=1
export SDL_RENDER_DRIVER=opengles2

cd "$GAMEDIR"
$ESUDO chmod 666 /dev/uinput
SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"
export LD_LIBRARY_PATH="$GAMEDIR/libs":"/lib":$LD_LIBRARY_PATH
export PATH="$GAMEDIR/libs:$PATH"
$GPTOKEYB "m3" &
./m3 --rom ./mother3.gba --bios ./gba_bios.bin 2>&1 | tee log.txt

$ESUDO kill -9 $(pidof gptokeyb)
$ESUDO systemctl restart oga_events &
printf "\033c" >> $CUR_TTY

oga_events &
printf "\033c" >> $CUR_TTY














