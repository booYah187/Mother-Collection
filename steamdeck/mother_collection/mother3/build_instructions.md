git clone https://github.com/josiahg2502-glitch/Mother3Recomp
cd Mother3Recomp/
touch tools/build-linux-fixed.sh
chmod +x tools/setup_deps.sh tools/run_mother3.sh tools/regen.sh tools/build-linux-fixed.sh

build-linux-fixed.sh
#!/usr/bin/env bash
# Native Linux Build Script for MOTHER 3 Recomp (Fixed for ARM64/PortMaster - v2)
set -euo pipefail

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$DIR"

echo "=== MOTHER 3 Native Recompilation - ARM64 Build ==="
echo "Working directory: $DIR"

# 1. Ensure submodules are present
echo "Ensuring submodules and build dependencies are present..."
if [ ! -d "gbarecomp/src" ]; then
    echo "Cloning pinned gbarecomp..."
    git clone https://github.com/mstan/gbarecomp.git gbarecomp
    git -C gbarecomp checkout 5e89943528aee7bc1c399bdbd035364a4b41dd10
fi

if [ ! -d "recomp-ui/src" ]; then
    echo "Cloning pinned recomp-ui..."
    git clone https://github.com/mstan/recomp-ui.git recomp-ui
    git -C recomp-ui checkout 43c40b909ee1caca5bb84dec6a2f4b4000582d60
fi

# Apply patches
echo "Checking and applying repository patches..."
if [ -f "patches/gbarecomp-mother3.patch" ]; then
    git -C gbarecomp apply --check "$DIR/patches/gbarecomp-mother3.patch" 2>/dev/null && \
    git -C gbarecomp apply "$DIR/patches/gbarecomp-mother3.patch" || true
fi
if [ -f "patches/recomp-ui-ups-and-translation.patch" ]; then
    git -C recomp-ui apply --check "$DIR/patches/recomp-ui-ups-and-translation.patch" 2>/dev/null && \
    git -C recomp-ui apply "$DIR/patches/recomp-ui-ups-and-translation.patch" || true
fi

# Initialize submodules
echo "Initializing submodules..."
git -C gbarecomp submodule update --init external/arm-recomp-core external/rbengine external/recomp-net 2>/dev/null || true

# ---------------------------------------------------------
# CRITICAL: Check for Generated Sources
# ---------------------------------------------------------
if [ ! -f "generated/recompiled_000.cpp" ]; then
    echo ""
    echo "!!! ERROR: generated/ folder is missing or empty !!!"
    echo "The build will fail or produce a non-functional stub binary."
    echo ""
    echo "To fix this, you MUST run the regeneration script FIRST:"
    echo "  ./tools/regen.sh <path_to_rom> <path_to_bios>"
    echo ""
    echo "Example:"
    echo "  ./tools/regen.sh /buildenv/m3/2/Mother\ 3\ \(HQ\).gba /buildenv/m3/2/gba_bios.bin"
    echo ""
    read -p "Press Enter to abort, or type 'skip' to attempt building without game data (will crash): " response
    if [ "$response" != "skip" ]; then
        exit 1
    fi
    echo "Proceeding with stub build (expect crash)..."
fi

# ---------------------------------------------------------
# Fix Compiler and SDL2 for ARM64/PortMaster
# ---------------------------------------------------------
echo "Configuring CMake build..."

# Find SDL2 CMake config path
SDL2_CMAKE_DIR=""
if [ -d "/usr/lib/cmake/SDL2" ]; then
    SDL2_CMAKE_DIR="/usr/lib/cmake/SDL2"
elif [ -d "/usr/lib/aarch64-linux-gnu/cmake/SDL2" ]; then
    SDL2_CMAKE_DIR="/usr/lib/aarch64-linux-gnu/cmake/SDL2"
elif [ -d "/usr/local/lib/cmake/SDL2" ]; then
    SDL2_CMAKE_DIR="/usr/local/lib/cmake/SDL2"
fi

if [ -z "$SDL2_CMAKE_DIR" ]; then
    echo "ERROR: Could not find SDL2 CMake config directory!"
    exit 1
fi

echo "Using SDL2 CMake config at: $SDL2_CMAKE_DIR"

# Build the main project with explicit SDL2_DIR and C++20 flags
# We pass these directly to the root CMake command
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_STANDARD=20 \
  -DCMAKE_CXX_STANDARD_REQUIRED=ON \
  -DSDL2_DIR="$SDL2_CMAKE_DIR" \
  -DCMAKE_CXX_COMPILER=g++-13 \
  -DCMAKE_C_COMPILER=gcc-13 \
  || {
    echo "CMake configuration failed. Check output above."
    exit 1
}

# Build
echo "Building..."
cmake --build build --target Mother3Recomp -j$(nproc 2>/dev/null || echo 2)

if [ -f "build/Mother3Recomp" ]; then
    cp build/Mother3Recomp ./Mother3Recomp
    chmod +x ./Mother3Recomp
    
    # Verify size
    SIZE=$(du -h ./Mother3Recomp | cut -f1)
    echo ""
    echo "Build succeeded! Binary size: $SIZE"
    if [ "$SIZE" == "30M" ] || [ "$SIZE" == "29M" ] || [ "$SIZE" == "31M" ]; then
        echo "WARNING: Binary is suspiciously small (~30MB). Did you forget to run regen.sh?"
        echo "A full build should be >80MB."
    else
        echo "Binary size looks correct."
    fi
    
    echo "Run ./tools/run_mother3.sh to play."
else
    echo "Build failed: Mother3Recomp binary not found."
    exit 1
fi

./tools/setup_deps.sh

cd /buildenv/m3/2/Mother3Recomp/gbarecomp
rm -rf build
mkdir build
cd build

cmake .. -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++-16 \
  -DCMAKE_C_COMPILER=/usr/bin/gcc-16 \
  -DCMAKE_CXX_STANDARD=20 \
  -DCMAKE_CXX_STANDARD_REQUIRED=ON

ninja gba_recompile

cd ..
cd ..

./tools/regen.sh ./m3.gba ./gba_bios.bin

update root CMakeLists.txt

cmake_minimum_required(VERSION 3.20)
project(Mother3Recomp C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GBARECOMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/gbarecomp"
    CACHE PATH "Path to this game's gbarecomp worktree")
set(RECOMP_UI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/recomp-ui"
    CACHE PATH "Path to the shared recomp-ui checkout")
set(GBARECOMP_RUNTIME_UI_ROOT "${RECOMP_UI_ROOT}"
    CACHE PATH "recomp-ui root used by the live GBA runtime adapter")
if(NOT EXISTS "${GBARECOMP_ROOT}/CMakeLists.txt")
    message(FATAL_ERROR
        "GBARECOMP_ROOT is not a gbarecomp checkout:\n"
        "  ${GBARECOMP_ROOT}\n"
        "Configure with -DGBARECOMP_ROOT=<path> if the local worktree link is absent.")
endif()

add_subdirectory("${GBARECOMP_ROOT}" gbarecomp_build EXCLUDE_FROM_ALL)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generated/recompiled.cpp")
    message(FATAL_ERROR
        "Stale monolithic generated/recompiled.cpp found. "
        "Run tools/regen.ps1 with the current gbarecomp worktree.")
endif()
file(GLOB GAME_GENERATED_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/*.cpp")

add_executable(Mother3Recomp
    src/main.cpp
    ${GAME_GENERATED_SOURCES})

set(GAME_RECOMPILED_SOURCES ${GAME_GENERATED_SOURCES})
list(FILTER GAME_RECOMPILED_SOURCES INCLUDE REGEX
    "/recompiled_[0-9]+\\.cpp$")
if(GAME_RECOMPILED_SOURCES)
    if(MSVC)
        set_source_files_properties(${GAME_RECOMPILED_SOURCES} PROPERTIES
            COMPILE_OPTIONS "/O1;/EHs-c-;/GR-")
    else()
        set_source_files_properties(${GAME_RECOMPILED_SOURCES} PROPERTIES
            COMPILE_OPTIONS "-O1;-g0;-fno-exceptions;-fno-rtti;-fno-strict-aliasing;-fno-math-errno")
    endif()
endif()

target_include_directories(Mother3Recomp PRIVATE
    "${GBARECOMP_ROOT}/src/armv4t"
    "${GBARECOMP_ROOT}/src/gba"
    "${GBARECOMP_ROOT}/src/runtime"
    "${GBARECOMP_ROOT}/src/debug")

target_link_libraries(Mother3Recomp PRIVATE
    "-Wl,--start-group"
    gbarecomp_runtime
    gbarecomp_debug
    gbarecomp_gba
    gbarecomp_armv4t
    "-Wl,--end-group")

target_compile_definitions(Mother3Recomp PRIVATE
    GBARECOMP_DEFAULT_DEBUG_PORT=19893
    GBARECOMP_WINDOW_TITLE="Mother3Recomp"
    GBARECOMP_DEFAULT_GAME_CONFIG="game.toml")

option(GBAGAME_RECOMP_UI "Build the recomp-ui launcher and in-game menu" ON)
if(GBAGAME_RECOMP_UI)
    if(NOT EXISTS "${RECOMP_UI_ROOT}/recomp_ui.cmake")
        message(FATAL_ERROR
            "recomp-ui is not available at ${RECOMP_UI_ROOT}. "
            "Run git submodule update --init --recursive or configure with "
            "-DRECOMP_UI_ROOT=<path>.")
    endif()
    include("${RECOMP_UI_ROOT}/recomp_ui.cmake")
    add_library(game_launcher_ui STATIC src/game_launcher_boot.cpp)
    target_include_directories(game_launcher_ui PRIVATE
        "${GBARECOMP_ROOT}/src/runtime"
        "${GBARECOMP_ROOT}/src/gba"
        src)
    recomp_target_launcher_ui(game_launcher_ui CONSOLE gba)
    if(NOT TARGET SDL2::SDL2)
    # Try pkg-config first (often works better on embedded systems)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    
    # Manually create the imported target if CMake didn't find it
    if(NOT TARGET SDL2::SDL2)
        add_library(SDL2::SDL2 UNKNOWN IMPORTED)
        set_target_properties(SDL2::SDL2 PROPERTIES
            IMPORTED_LOCATION "${SDL2_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
        )
    endif()
endif()
    target_link_libraries(Mother3Recomp PRIVATE game_launcher_ui)
    set_property(SOURCE src/main.cpp APPEND PROPERTY
        COMPILE_DEFINITIONS GBAGAME_RECOMP_UI)
endif()

if(MSVC)
    target_link_options(Mother3Recomp PRIVATE /STACK:16777216)
elseif(MINGW OR WIN32)
    target_link_options(Mother3Recomp PRIVATE -Wl,--stack,16777216)
endif()

./tools/build-linux-fixed.sh (arm64)
./tools/build_linux.sh (x86_64)