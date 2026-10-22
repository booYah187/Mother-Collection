# Mother-Collection
<img src="https://github.com/booYah187/Mother-Collection/blob/main/Handheld/mother_collection/Mother%20Collection.png?raw=true" alt="alt text" width="198" height="300" /> <img src="https://github.com/booYah187/Mother-Collection/blob/main/Handheld/mother_collection/assets/boxart/mother.png?raw=true" alt="alt text" width="198" height="300" />
<img src="https://github.com/booYah187/Mother-Collection/blob/main/Handheld/mother_collection/assets/boxart/earthbound.png?raw=true" alt="alt text" width="198" height="300" /> <img src="https://github.com/booYah187/Mother-Collection/blob/main/Handheld/mother_collection/assets/boxart/mother3.png?raw=true" alt="alt text" width="198" height="300" />


Mother (Earthbound Beginnings Remake, Earthbound Recompiled, and Mother3Recomp, packaged with customizable launcher and guides for retro handhelds via Portmaster and linux desktop. Portmaster port has only been tested personally on muOs and Jelos and has not gone through official Portmaster testing standards and formatting at this time.

Download desktop or handheld version from releases, add guides.zip contents to mother_collection/guides (note the Mother guide is the Japanese Mother Encyclopdia, will be replaced with Earthbound Beginnings Remake Player's Guide upon release), place roms and gba bios in respective folders, run launcher. Hash enforcement is not implemented at this time to allow romhacks and different game versions, but base hashes tested are noted in "placeromhere" in respective game folders. Note that no roms or bios are included with this package, user must source and provide their own. Note that Earthbound can use msu audio by creating msu folder (with msu audio tracks) inside the mother_collection/earthbound directory. 

For building Earthbound Recompiled and Mother3Recomp from source, see https://github.com/seanstaggsQU/earthboundRecompLinux2026 and https://github.com/josiahg2502-glitch/Mother3Recomp respectively.
Note that minor modifications have been made to the individual games, i.e. addition of m3 battle sprites to Earthbound, Ninten bandana in Earthbound Beginnings Remake, minor bug fixes, etc. Additional minor modifications are planned, but without any long term roadmap. (Personal source repos pending)

For the launcher background images, boxart, background music, guides, and game executables can be customized by replacing the respective files in assets/ guides/ and gamefolders. mReader is bundled for viewing guide, different reader can be used by replacing mreader/reader with launch script for epub reader of choice.

Building launcher from src:
# MOTHER / EarthBound Collection Launcher

SDL2/C launcher for PortMaster ARM64 and x86_64 Linux.

## Dependencies

- SDL2
- SDL2_image
- SDL2_ttf
- SDL2_mixer
- A C compiler and Make

On Debian/Ubuntu, install the development packages:

```sh
sudo apt install build-essential pkg-config libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
make
```

Run locally from the project root:

```sh
./launcher
```

## Assets

Optional assets are loaded from `assets/`:

- `backgrounds/mother.png`, `backgrounds/earthbound.png`, `backgrounds/mother3.png`
- `boxart/mother.png`, `boxart/earthbound.png`, `boxart/mother3.png`
- `music/mother.ogg`, `music/earthbound.ogg`, `music/mother3.ogg`

Missing images are replaced with simple labeled placeholders. The launcher scales images to fit; keep your own aspect ratios in mind.

## ROM validation

Edit `src/config.h` and set each game's exact ROM filename and SHA-1 hash. The hash is checked using `sha1sum` if configured. If a hash is left as `SET_SHA1_HASH_HERE`, validation is skipped for development builds. The ROM filename is expected beside that game's launch script, as requested; the example paths in `src/config.h` reflect this.

## Game scripts and guides

Expected game scripts:

- `mother/mother.sh`
- `earthbound/earthbound.sh`
- `mother3/mother3.sh`

Expected guide files:

- `guides/mother.epub`
- `guides/earthbound.epub`
- `guides/mother3.epub`

The guide launcher expects `mreader/reader` by default (matching the `./reader` executable name in your mReader launch script). It passes the EPUB path as `argv[1]` and sets `LD_LIBRARY_PATH` to include `mreader/libs`. Adjust `MREADER_EXECUTABLE` in `src/config.h` if your bundled binary has a different name.

## Controls

- D-pad: cycle focus among game, Player's Guide, and next-page arrow
- A / Enter: activate focused item
- L / R: previous / next page
- B / Escape: no action in this initial skeleton
- Window close / Ctrl-C: exit

The exact physical button mapping depends on the firmware's SDL controller mapping. The launcher supports SDL controller buttons and keyboard events.

## Build targets

- `make`: native build for the machine compiling it
- `make clean`
- `make print-config`

For ARM64 PortMaster builds, use the appropriate target sysroot/toolchain and set `CC`, `PKG_CONFIG`, and any required `PKG_CONFIG_PATH` / `PKG_CONFIG_LIBDIR` values. The source itself is architecture-neutral.
