#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

int redirect_stdout_to(const char *path, FILE *err)
{
    FILE *f = freopen(path, "w", stdout);
    if (!f) {
        fprintf(err, "log: cannot redirect stdout to '%s': %s\n", path, strerror(errno));
        return 1;
    }
    return 0;
}

int redirect_stderr_to(const char *path, FILE *err)
{
    FILE *f = freopen(path, "w", stderr);
    if (!f) {
        fprintf(err, "log: cannot redirect stderr to '%s': %s\n", path, strerror(errno));
        return 1;
    }
    return 0;
}
