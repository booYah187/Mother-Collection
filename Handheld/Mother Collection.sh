#!/bin/bash
# PORTMASTER: mother_collection.zip, launch.sh

PORTNAME="The Mother Collection"

if [ -d "/opt/system/Tools/PortMaster/" ]; then
    controlfolder="/opt/system/Tools/PortMaster"
elif [ -d "/opt/tools/PortMaster/" ]; then
    controlfolder="/opt/tools/PortMaster"
else
    controlfolder="/roms/ports/PortMaster"
fi

source "$controlfolder/control.txt"
get_controls

CUR_TTY=/dev/tty0
$ESUDO chmod 666 "$CUR_TTY"

# PortMaster supplies $directory. Keep a fallback for desktop testing.
if [ -n "${directory:-}" ]; then
    GAMEDIR="/${directory}/ports/mother_collection"
else
    GAMEDIR="${COLLECTION_DIR:-$(cd "$(dirname "$0")" && pwd)}"
fi

cd "$GAMEDIR" || exit 1

$ESUDO chmod 666 /dev/uinput

export SDL_VIDEO_EGL_NO_DEVICE_ENUMERATION=1
export SDL_VIDEO_FORCE_RENDERER=1
export SDL_RENDER_DRIVER=opengles2

export SDL_GAMECONTROLLERCONFIG="${sdl_controllerconfig:-${SDL_GAMECONTROLLERCONFIG:-}}"
export SCREEN_WIDTH="${DISPLAY_WIDTH:-${SCREEN_WIDTH:-640}}"
export SCREEN_HEIGHT="${DISPLAY_HEIGHT:-${SCREEN_HEIGHT:-480}}"

export LD_LIBRARY_PATH="$GAMEDIR/libs${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

$GPTOKEYB "launcher" &
./launcher 2>&1 | tee log.txt

# Capture output for troubleshooting.
./launcher 2>&1 | tee "$GAMEDIR/log.txt"
STATUS=${PIPESTATUS[0]}

if [ -n "$GPTOKEYB_PID" ]; then
    kill "$GPTOKEYB_PID" 2>/dev/null || true
    wait "$GPTOKEYB_PID" 2>/dev/null || true
fi

# Restore the frontend's event handling.
if command -v oga_events >/dev/null 2>&1; then
    oga_events &
fi

$ESUDO kill -9 $(pidof gptokeyb)
$ESUDO systemctl restart oga_events &
printf "\033c" >> $CUR_TTY

oga_events &
printf "\033c" >> $CUR_TTY
