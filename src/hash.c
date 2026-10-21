#include "hash.h"

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Uses the standard sha1sum utility, avoiding a new crypto-library dependency.
 * Paths are passed as argv values (not interpolated into a shell command).
 */
int sha1_file_matches(const char *path, const char *expected_hex,
                      char *error, unsigned long error_size)
{
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        snprintf(error, error_size, "Could not create hash-check pipe: %s", strerror(errno));
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        snprintf(error, error_size, "Could not start SHA-1 checker: %s", strerror(errno));
        return -1;
    }

    if (pid == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("sha1sum", "sha1sum", path, (char *)NULL);
        _exit(127);
    }

    close(pipefd[1]);
    char output[256] = {0};
    ssize_t got = read(pipefd[0], output, sizeof(output) - 1);
    close(pipefd[0]);

    int status = 0;
    waitpid(pid, &status, 0);

    if (got <= 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        snprintf(error, error_size, "Could not calculate SHA-1. Is sha1sum available?");
        return -1;
    }

    char actual[41] = {0};
    if (strlen(output) < 40) {
        snprintf(error, error_size, "Unexpected SHA-1 checker output.");
        return -1;
    }
    memcpy(actual, output, 40);

    if (strcasecmp(actual, expected_hex) == 0) return 1;

    snprintf(error, error_size, "ROM hash mismatch. Expected SHA-1: %.40s; found: %.40s",
             expected_hex, actual);
    return 0;
}
