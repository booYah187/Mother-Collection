#include <stddef.h>
#ifndef COLLECTION_CONFIG_H
#define COLLECTION_CONFIG_H

#define COLLECTION_TITLE "THE MOTHER COLLECTION"

#define PAGE_COUNT 3

/* Relative to the collection root. */
#define MREADER_EXECUTABLE "mreader/reader"

typedef struct {
    const char *id;
    const char *title;
    const char *subtitle;
    const char *background_path;
    const char *boxart_path;
    const char *music_path;
    const char *game_script;

    /*
     * ROM filename or glob pattern, relative to the collection root.
     * Examples:
     *   mother/mother.nes
     *   earthbound/*.sfc
     *   mother3/*.gba
     */
    const char *rom_filename;

    /*
     * NULL-terminated list of accepted SHA-1 hashes.
     * NULL or an empty list disables hash validation.
     */
    const char *const *sha1_hashes;

    const char *guide_path;
} GamePage;

/*
 * Accepted SHA-1 lists.
 *
 * Replace the example hashes with verified 40-character SHA-1
 * values for ROM revisions you want to accept.
 *
 * Keep NULL as the final entry in each list.
 */
static const char *const mother_hashes[] = {
    NULL
};

static const char *const earthbound_hashes[] = {
    /* Add each accepted EarthBound ROM SHA-1 here. */
    NULL
};

static const char *const mother3_hashes[] = {
    NULL
};

/*
 * Game configuration.
 *
 * ROM paths and wildcard patterns are relative to the collection root.
 * Hash validation is disabled for games whose lists contain only NULL.
 */
static const GamePage GAME_PAGES[PAGE_COUNT] = {
    {
        "mother", "MOTHER: Earthbound Beginnings Remake", "A strange journey begins",
        "assets/backgrounds/mother.png",
        "assets/boxart/mother.png",
        "assets/music/mother.ogg",
        "mother/mother.sh",
        "mother/mother.nes",
        mother_hashes,
        "guides/mother.epub"
    },
    {
        "earthbound", "EARTHBOUND: Earthbound Recompiled",
        "A familiar world, an unfamiliar adventure",
        "assets/backgrounds/earthbound.png",
        "assets/boxart/earthbound.png",
        "assets/music/earthbound.ogg",
        "earthbound/earthbound.sh",
        "earthbound/*.sfc",
        earthbound_hashes,
        "guides/earthbound.epub"
    },
    {
        "mother3", "MOTHER 3: Mother3Recomp", "Somewhere, a new story begins",
        "assets/backgrounds/mother3.png",
        "assets/boxart/mother3.png",
        "assets/music/mother3.ogg",
        "mother3/mother3.sh",
        "mother3/mother3.gba",
        mother3_hashes,
        "guides/mother3.epub"
    }
};

#endif