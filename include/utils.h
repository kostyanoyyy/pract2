#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

int list_users(FILE *out, FILE *err);        // -u / --users
int list_processes(FILE *out, FILE *err);    // -p / --processes

int redirect_stdout_to(const char *path, FILE *err); // -l / --log PATH
int redirect_stderr_to(const char *path, FILE *err); // -e / --errors PATH

void print_help(FILE *out);                  // -h / --help

#endif // UTILS_H
