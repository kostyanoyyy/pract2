#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "utils.h"

typedef struct {
    int   pid;
    char *comm;
} Proc;

static char* xstrdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *p = (char*)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

static int is_digits(const char *s) {
    if (!s || !*s) return 0;
    for (; *s; ++s) {
        if (!isdigit((unsigned char)*s)) return 0;
    }
    return 1;
}

static int cmp_pid(const void *a, const void *b) {
    const Proc *pa = (const Proc*)a, *pb = (const Proc*)b;
    if (pa->pid < pb->pid) return -1;
    if (pa->pid > pb->pid) return 1;
    return 0;
}

int list_processes(FILE *out, FILE *err) {
    DIR *d = opendir("/proc");
    if (!d) {
        fprintf(err, "proc: cannot open /proc: %s\n", strerror(errno));
        return 1;
    }

    size_t cap = 256, n = 0;
    Proc *arr = (Proc*)malloc(cap * sizeof(Proc));
    if (!arr) {
        closedir(d);
        fprintf(err, "proc: no memory\n");
        return 1;
    }

    struct dirent *de;
    while ((de = readdir(d)) != NULL) {
        if (!is_digits(de->d_name)) continue;

        if (n == cap) {
            cap *= 2;
            Proc *tmp = (Proc*)realloc(arr, cap * sizeof(Proc));
            if (!tmp) {
                fprintf(err, "proc: realloc failed\n");
                break;
            }
            arr = tmp;
        }

        int pid = atoi(de->d_name);

        char path[256];
        snprintf(path, sizeof(path), "/proc/%d/comm", pid);

        char *cmd = NULL;
        FILE *f = fopen(path, "r");
        if (f) {
            char buf[256];
            if (fgets(buf, sizeof buf, f)) {
                size_t k = strlen(buf);
                if (k && buf[k-1] == '\n') buf[k-1] = '\0';
                cmd = xstrdup(buf);
            }
            fclose(f);
        }
        if (!cmd) cmd = xstrdup("?");

        arr[n].pid  = pid;
        arr[n].comm = cmd;
        n++;
    }
    closedir(d);

    qsort(arr, n, sizeof(Proc), cmp_pid);

    fprintf(out, "%-8s %s\n", "PID", "COMMAND");
    for (size_t i = 0; i < n; ++i) {
        fprintf(out, "%-8d %s\n", arr[i].pid, arr[i].comm ? arr[i].comm : "?");
        free(arr[i].comm);
    }
    free(arr);
    return 0;
}
