
#!/bin/bash

GAMEDIR="/home/deck/Games/mother_collection/mother"
ROM="$GAMEDIR/mother.nes"
LOG="$GAMEDIR/log.txt"

exec >> "$LOG" 2>&1

echo
echo "===== MOTHER launch $(date) ====="

cd "$GAMEDIR" || exit 1

if [ ! -f "$ROM" ]; then
    echo "ERROR: ROM not found"
    exit 1
fi

echo "Launching RetroArch..."

"/home/deck/Games/Retro/Emulation/tools/launchers/retroarch.sh" -L /snes9x_libretro.so /home/deck/Games/mother_collection/mother/mother.nes
RESULT=$?
echo "RetroArch exit status: $RESULT"
exit "$RESULT"
