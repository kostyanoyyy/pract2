#include <stdio.h>
#include "utils.h"

void print_help(FILE *out) {
    fprintf(out,
        "Usage: pr2 [OPTIONS]\n"
        "  -u, --users           List system users with home directories (sorted)\n"
        "  -p, --processes       List running processes (sorted by PID)\n"
        "  -h, --help            Show this help and exit\n"
        "  -l, --log PATH        Redirect standard output to file PATH\n"
        "  -e, --errors PATH     Redirect standard error to file PATH\n"
    );
}
