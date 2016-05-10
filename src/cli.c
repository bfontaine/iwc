#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "iwc.h"
#include "cli.h"
#include "version.h"

int print_help(char *exe) {
        printf("Usage:\n"
               "\t%1$s [<options>] [<file> ...]\n"
               "\n"
               "Options:\n"
               "\t-h: show this help and exit\n"
               "\t-v: print the version and exit\n"
               "\t-c: count the number of bytes\n"
               "\t-l: count the number of lines\n"
               "\t-w: count the number of words\n"
               "\t-m: same as -c\n"
               "\n", exe);
        return 0;
}

int print_version(void) {
        printf("iwc(1) - version %s\n", IWC_VERSION);
        return 0;
}

int main(int argc, char **argv) {
        int optch;
        extern int opterr;

        opterr = 1;

        counter_t *counter = iwc_make_counter();

        if (counter == NULL) {
                return 1;
        }

        while ((optch = getopt(argc, argv, "hvmclw")) != -1) {
                switch (optch) {
                case 'm':
                        // we don't do anything specific here for now
                case 'c':
                        counter->count_bytes = 1;
                        break;
                case 'l':
                        counter->count_lines = 1;
                        break;
                case 'w':
                        counter->count_words = 1;
                        break;
                case 'h':
                        return print_help(argv[0]);
                case 'v':
                        return print_version();
                case '?':
                        fprintf(stderr, "Unknown option `-%c`.\n", optopt);
                        return 1;
                }
        }

        if (!counter->count_lines && !counter->count_words &&
                        !counter->count_bytes) {
                counter->count_lines = 1;
                counter->count_words = 1;
                counter->count_bytes = 1;
        }

        int fileno = 0,
            result = 0;

        if (optind == argc) {
                result = iwc_count(counter, STDIN_FILENO);
                if (result != 0) {
                        goto end;
                }
        }

        for (int i=optind; i < argc; i++) {

                fileno = open(argv[i], O_RDONLY);

                if (fileno == -1) {
                        perror("open");
                        return 1;
                }

                result = iwc_count(counter, fileno);
                if (result != 0) {
                        goto end;
                }

                if (close(fileno) == -1) {
                        perror("close");
                        return 1;
                }
        }
        iwc_print_total(counter);

end:
        iwc_destroy_counter(counter);
        return result;
}
