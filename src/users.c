#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "utils.h"

typedef struct {
    char *name;
    char *home;
} User;

static char* xstrdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *p = (char*)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

static int cmp_user(const void *a, const void *b) {
    const User *ua = (const User*)a, *ub = (const User*)b;
    if (!ua->name && !ub->name) return 0;
    if (!ua->name) return -1;
    if (!ub->name) return 1;
    return strcmp(ua->name, ub->name);
}

int list_users(FILE *out, FILE *err) {
    FILE *fp = fopen("/etc/passwd", "r");
    if (!fp) {
        fprintf(err, "users: cannot open /etc/passwd: %s\n", strerror(errno));
        return 1;
    }

    size_t cap = 64, n = 0;
    User *arr = (User*)malloc(cap * sizeof(User));
    if (!arr) {
        fclose(fp);
        fprintf(err, "users: no memory\n");
        return 1;
    }

    char buf[4096];
    while (fgets(buf, sizeof buf, fp) != NULL) {
        // формат строки: login:pw:uid:gid:gecos:home:shell
        // возьмём поля 1 (login) и 6 (home)
        // используем strtok (перезапускаем на каждой строке — безопасно)
        char *login = strtok(buf, ":\n");
        (void)strtok(NULL, ":\n"); // pw
        (void)strtok(NULL, ":\n"); // uid
        (void)strtok(NULL, ":\n"); // gid
        (void)strtok(NULL, ":\n"); // gecos
        char *home  = strtok(NULL, ":\n"); // home

        if (login && home) {
            if (n == cap) {
                cap *= 2;
                User *tmp = (User*)realloc(arr, cap * sizeof(User));
                if (!tmp) {
                    fprintf(err, "users: realloc failed\n");
                    break;
                }
                arr = tmp;
            }
            arr[n].name = xstrdup(login);
            arr[n].home = xstrdup(home);
            if (!arr[n].name || !arr[n].home) {
                fprintf(err, "users: strdup failed\n");
                break;
            }
            n++;
        }
    }
    fclose(fp);

    qsort(arr, n, sizeof(User), cmp_user);

    for (size_t i = 0; i < n; ++i) {
        fprintf(out, "%-20s %s\n", arr[i].name ? arr[i].name : "?", arr[i].home ? arr[i].home : "?");
        free(arr[i].name);
        free(arr[i].home);
    }
    free(arr);
    return 0;
}
