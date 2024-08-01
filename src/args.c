// hello
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LOCAL
#include "args.h"

args_t *parse_args(int argc, char **argv) {
    args_t *args = (args_t *) malloc(sizeof(args_t));
    args->nocolor = 0;
    args->nowelcome = 0;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            usage(argv[0]);
            exit(1);
        }

        // check dir

        else if (!strcmp(argv[i], "-nc") || !strcmp(argv[i], "--no-color")) {
            args->nocolor = 1;
        }

        else if (!strcmp(argv[i], "-nw") || !strcmp(argv[i], "--no-welcome")) {
            args->nowelcome = 1;
        }

        else {
            fprintf(stderr, "Error: unknown argument `%s`\n", argv[i]);
            usage(argv[0]);
            exit(1);
        }
    }

    return args;
}

void usage(char *cmd) {
    fprintf(stderr,
"Usage:\r\n$ %s [options]\r\n\
options:\r\n\
\t-nc | --no-color\tdisable colored output\r\n\
\t-nw | --no-welcome\tdisable welcome screen\r\n", cmd);
}

