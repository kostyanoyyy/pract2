#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "utils.h"

int main(int argc, char **argv) {
    int opt, users=0, procs=0, show_help=0;
    const char *log_path = NULL, *err_path = NULL;

    static struct option long_opts[] = {
        {"users",     no_argument,       0, 'u'},
        {"processes", no_argument,       0, 'p'},
        {"help",      no_argument,       0, 'h'},
        {"log",       required_argument, 0, 'l'},
        {"errors",    required_argument, 0, 'e'},
        {0,0,0,0}
    };

    while ((opt = getopt_long(argc, argv, "uphl:e:", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'u': users = 1; break;
            case 'p': procs = 1; break;
            case 'h': show_help = 1; break;
            case 'l': log_path = optarg; break;
            case 'e': err_path = optarg; break;
            default: print_help(stderr); return 2;
        }
    }

    if (err_path && redirect_stderr_to(err_path, stderr) != 0) return 1;
    if (log_path && redirect_stdout_to(log_path, stderr) != 0) return 1;

    if (show_help || (!users && !procs)) { print_help(stdout); return 0; }

    int rc = 0;
    if (users)  rc |= list_users(stdout, stderr);
    if (procs)  rc |= list_processes(stdout, stderr);
    return rc ? 1 : 0;
}
