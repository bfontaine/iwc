#include <stdio.h>
#include <unistd.h>

#include "iwc.h"
#include "cli.h"
#include "version.h"

int print_help(char *exe) {
        printf("Usage:\n"
               "\t%1$s [<options>] [<file>]\n"
               "\n"
               "Options:\n"
               "\t-h: show this help and exit\n"
               "\t-v: print the version and exit\n"
               "\n", exe);
        return 0;
}

int print_version(void) {
        printf("iwc(1) - version %s\n", IWC_VERSION);
        return 0;
}

int main(int argc, char **argv) {

        char lines_count = 0,
             chars_count = 0,
             words_count = 0;

        int optch;
        extern int opterr;

        opterr = 1;

        while ((optch = getopt(argc, argv, "hvclw")) != -1) {
                switch (optch) {
                case 'c':
                        chars_count = 1;
                        break;
                case 'l':
                        lines_count = 1;
                        break;
                case 'w':
                        words_count = 1;
                        break;
                case 'h':
                        return print_help(argv[0]);
                case 'v':
                        return print_version();
                }
        }

        if (!chars_count && !lines_count && !words_count) {
                lines_count = 1;
        }

        int chars = 0,
            lines = 0,
            words = 0;

        int file_no = STDIN_FILENO;

        iwc_counts(file_no,
                        chars_count ? &chars : NULL,
                        lines_count ? &lines : NULL,
                        words_count ? &words : NULL);

        return 0;
}
