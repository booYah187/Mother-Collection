#!/bin/bash
# Wrapper for the mother_collection launcher
LAUNCHER_DIR="/home/deck/Games/mother_collection"
LAUNCHER="$LAUNCHER_DIR/launcher"
LOG="$LAUNCHER_DIR/log.txt"

exec >> "$LOG" 2>&1

echo
echo "===== WRAPPER launch $(date) ====="
echo "ENV: DISPLAY=$DISPLAY XDG_SESSION_TYPE=$XDG_SESSION_TYPE"

# --- Environment repair ---
export PATH="/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:$PATH"
export SDL_VIDEODRIVER="${SDL_VIDEODRIVER:-x11}"
export XDG_RUNTIME_DIR="${XDG_RUNTIME_DIR:-/run/user/1000}"
export HOME="${HOME:-/home/deck}"

# --- CRITICAL FIX: Point to the EXACT folder where you put the libs ---
# The probe showed: /home/deck/Games/mother_collection/libs/libSDL2_image-2.0.so.0
export LD_LIBRARY_PATH="$LAUNCHER_DIR/libs:$LD_LIBRARY_PATH"

# Remove Steam's 32-bit overlay hook
#unset LD_PRELOAD

echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"

# --- Verify the libs now resolve ---
echo "--- ldd check ---"
ldd "$LAUNCHER" 2>&1 | grep -i "not found" && echo "STILL MISSING LIBS (above)" || echo "ldd: all libraries resolved"

echo "Starting launcher..."
cd "$LAUNCHER_DIR" || exit 1
"$LAUNCHER" "$@"
RESULT=$?
echo "Launcher exited with status: $RESULT"
exit "$RESULT"