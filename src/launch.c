
#define _XOPEN_SOURCE 700

#include "launch.h"
#include "hash.h"

#include <SDL.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static char collection_root[PATH_MAX];

static int get_root(void)
{
    if (collection_root[0])
        return 1;

    char resolved[PATH_MAX];

    if (!realpath(".", resolved))
        return 0;

    int n = snprintf(collection_root, sizeof(collection_root),
                     "%s", resolved);

    return n >= 0 && (size_t)n < sizeof(collection_root);
}

static int make_path(char *out, size_t out_size, const char *relative)
{
    if (!out || !out_size || !relative)
        return 0;

    int n = snprintf(out, out_size, "%s/%s",
                     collection_root, relative);

    return n >= 0 && (size_t)n < out_size;
}

static int file_exists(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static void cleanup_before_exec(Launcher *launcher)
{
    launcher_shutdown(launcher);
}

/*
 * A NULL-terminated list of accepted SHA-1 strings is expected.
 * Return:
 *   1 = accepted
 *   0 = no accepted hash matched
 *
 * A NULL hash list disables validation.
 */
static int validate_rom(const GamePage *game,
                        const char *rom_path,
                        char *error,
                        size_t error_size)
{
    if (error && error_size)
        error[0] = '\0';

    if (!game->sha1_hashes || !game->sha1_hashes[0])
        return 1;

    char last_error[384] = {0};

    for (size_t i = 0; game->sha1_hashes[i]; ++i) {
        const char *expected = game->sha1_hashes[i];

        if (strlen(expected) != 40)
            continue;

        char hash_error[384] = {0};

        int result = sha1_file_matches(
            rom_path, expected, hash_error, sizeof(hash_error));

        if (result == 1)
            return 1;

        if (hash_error[0])
            snprintf(last_error, sizeof(last_error), "%s",
                     hash_error);
    }

    if (error && error_size) {
        if (last_error[0]) {
            snprintf(error, error_size, "%s", last_error);
        } else {
            snprintf(error, error_size,
                     "ROM SHA-1 did not match any accepted hash.");
        }
    }

    return 0;
}

/*
 * Resolve a ROM filename or wildcard pattern relative to the
 * collection root. If hashes are configured, try every matching
 * regular file until an accepted ROM is found.
 *
 * The selected absolute path is returned in rom_path.
 */
static int find_rom(const GamePage *game,
                    char *rom_path,
                    size_t rom_path_size,
                    char *error,
                    size_t error_size)
{
    char pattern[PATH_MAX];

    if (!make_path(pattern, sizeof(pattern), game->rom_filename)) {
        snprintf(error, error_size, "ROM search path is too long.");
        return 0;
    }

    glob_t matches;
    memset(&matches, 0, sizeof(matches));

    int gr = glob(pattern, 0, NULL, &matches);

    if (gr != 0 || matches.gl_pathc == 0) {
        globfree(&matches);
        snprintf(error, error_size,
                 "No ROM matched:\n%s", game->rom_filename);
        return 0;
    }

    char last_error[384] = {0};

    for (size_t i = 0; i < matches.gl_pathc; ++i) {
        const char *candidate = matches.gl_pathv[i];

        if (!file_exists(candidate))
            continue;

        char hash_error[384] = {0};

        if (!validate_rom(game, candidate,
                          hash_error, sizeof(hash_error))) {
            if (hash_error[0])
                snprintf(last_error, sizeof(last_error),
                         "%s", hash_error);
            continue;
        }

        int n = snprintf(rom_path, rom_path_size, "%s", candidate);

        if (n < 0 || (size_t)n >= rom_path_size) {
            globfree(&matches);
            snprintf(error, error_size, "ROM path is too long.");
            return 0;
        }

        globfree(&matches);
        return 1;
    }

    globfree(&matches);

    if (last_error[0]) {
        snprintf(error, error_size,
                 "No matching ROM passed validation.\n%s",
                 last_error);
    } else {
        snprintf(error, error_size,
                 "No regular ROM file found for:\n%s",
                 game->rom_filename);
    }

    return 0;
}

static int dispatch_script(Launcher *launcher,
                           const char *relative_script,
                           const char *rom_path)
{
    char script[PATH_MAX];

    if (!get_root()) {
        launcher_show_message(
            launcher, "Could not determine collection directory.", 5000);
        return 0;
    }

    if (!make_path(script, sizeof(script), relative_script)) {
        launcher_show_message(launcher, "Launch script path is too long.", 5000);
        return 0;
    }

    if (!file_exists(script)) {
        char message[512];
        snprintf(message, sizeof(message),
                 "Launch script not found:\n%s", relative_script);
        launcher_show_message(launcher, message, 6000);
        return 0;
    }

    cleanup_before_exec(launcher);

    if (rom_path)
        execl("/bin/sh", "sh", script, rom_path, (char *)NULL);
    else
        execl("/bin/sh", "sh", script, (char *)NULL);

    fprintf(stderr, "Could not execute %s\n", script);
    return -1;
}

int launch_game(Launcher *launcher, int page)
{
    if (page < 0 || page >= PAGE_COUNT)
        return 0;

    const GamePage *game = &GAME_PAGES[page];

    if (!get_root()) {
        launcher_show_message(
            launcher, "Could not determine collection directory.", 5000);
        return 0;
    }

    char rom_path[PATH_MAX];
    char error[512] = {0};

    if (!find_rom(game, rom_path, sizeof(rom_path),
                  error, sizeof(error))) {
        char message[768];

        snprintf(message, sizeof(message),
                 "%s ROM unavailable.\n%s",
                 game->title, error[0] ? error : "ROM search failed.");

        launcher_show_message(launcher, message, 7000);
        return 0;
    }

    return dispatch_script(launcher, game->game_script, rom_path);
}

int launch_guide(Launcher *launcher, int page)
{
    if (page < 0 || page >= PAGE_COUNT)
        return 0;

    if (!get_root()) {
        launcher_show_message(
            launcher, "Could not determine collection directory.", 5000);
        return 0;
    }

    char reader[PATH_MAX];
    char guide[PATH_MAX];
    char mreader_dir[PATH_MAX];
    char gptokeyb[PATH_MAX];
    char gptk_config[PATH_MAX];

    if (!make_path(reader, sizeof(reader), MREADER_EXECUTABLE) ||
        !make_path(guide, sizeof(guide), GAME_PAGES[page].guide_path) ||
        !make_path(mreader_dir, sizeof(mreader_dir), "mreader") ||
        !make_path(gptokeyb, sizeof(gptokeyb), "mreader/gptokeyb") ||
        !make_path(gptk_config, sizeof(gptk_config),
                   "mreader/reader.gptk")) {
        launcher_show_message(launcher, "Guide path is too long.", 5000);
        return 0;
    }

    if (!file_exists(reader)) {
        launcher_show_message(
            launcher,
            "mReader executable not found.\nExpected: mreader/reader",
            6000);
        return 0;
    }

    if (!file_exists(guide)) {
        char message[512];
        snprintf(message, sizeof(message),
                 "Player's Guide file not found:\n%s",
                 GAME_PAGES[page].guide_path);
        launcher_show_message(launcher, message, 6000);
        return 0;
    }

        /*
     * gptokeyb is optional. ARM64 handheld builds bundle it;
     * desktop builds can launch mReader without it.
     */
    int use_gptokeyb =
        file_exists(gptokeyb) &&
        access(gptokeyb, X_OK) == 0 &&
        file_exists(gptk_config);

    const char *old_ld = getenv("LD_LIBRARY_PATH");
    char libs[PATH_MAX * 2];
    int n;

    if (old_ld && *old_ld) {
        n = snprintf(libs, sizeof(libs),
                     "%s/mreader/libs:%s", collection_root, old_ld);
    } else {
        n = snprintf(libs, sizeof(libs),
                     "%s/mreader/libs", collection_root);
    }

    if (n < 0 || (size_t)n >= sizeof(libs)) {
        launcher_show_message(
            launcher, "mReader library path is too long.", 5000);
        return 0;
    }

    if (setenv("LD_LIBRARY_PATH", libs, 1) != 0) {
        launcher_show_message(
            launcher, "Could not set mReader library path.", 5000);
        return 0;
    }

    /*
     * mReader expects resources such as resources/fonts to be
     * relative to its own directory.
     */
    if (chdir(mreader_dir) != 0) {
        fprintf(stderr, "Could not change directory to %s\n", mreader_dir);
        launcher_show_message(
            launcher, "Could not enter the mReader directory.", 5000);
        return -1;
    }

    cleanup_before_exec(launcher);

    /*
     * Run gptokeyb with reader.gptk, then launch mReader.
     * The shell waits for mReader to exit and stops gptokeyb
     * afterward. Positional parameters avoid path-quoting issues.
     */
        if (use_gptokeyb) {
        static const char *const guide_runner =
            "\"$1\" reader -c \"$2\" &\n"
            "gptokeyb_pid=$!\n"
            "\"$3\" \"$4\"\n"
            "reader_status=$?\n"
            "kill \"$gptokeyb_pid\" 2>/dev/null || true\n"
            "wait \"$gptokeyb_pid\" 2>/dev/null || true\n"
            "exit \"$reader_status\"\n";

        execl("/bin/sh", "sh", "-c", guide_runner,
              "mreader-guide",
              gptokeyb,
              gptk_config,
              reader,
              guide,
              (char *)NULL);
    } else {
        /* Desktop build: mReader handles input itself. */
        execl(reader, reader, guide, (char *)NULL);
    }

    fprintf(stderr, "Could not launch mReader guide\n");
    return -1;
}
