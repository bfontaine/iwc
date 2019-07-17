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

        char lines_count = 0,
             words_count = 0,
             bytes_count = 0;

        int optch;
        extern int opterr;

        opterr = 1;

        while ((optch = getopt(argc, argv, "hvmclw")) != -1) {
                switch (optch) {
                case 'm':
                case 'c':
                        bytes_count = 1;
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
                case '?':
                        fprintf(stderr, "Unknown option `-%c`.\n", optopt);
                        return 1;
                }
        }

        if (!lines_count && !words_count && !bytes_count) {
                lines_count = 1;
                words_count = 1;
                bytes_count = 1;
        }

        int file_no = 0;

        counter_t lines = 0,
                  words = 0,
                  bytes = 0,

                  *lc = lines_count ? &lines : NULL,
                  *wc = words_count ? &words : NULL,
                  *bc = bytes_count ? &bytes : NULL;


        if (optind == argc) {
                if (iwc_counts(STDIN_FILENO, lc, wc, bc) == -1) {
                        perror("read");
                        return 1;
                }
        }

        for (int i=optind; i < argc; i++) {
                file_no = open(argv[i], O_RDONLY);

                if (file_no == -1) {
                        perror("open");
                        return 1;
                }

                if (iwc_counts(file_no, lc, wc, bc) == -1) {
                        perror("read");
                        return 1;
                }

                if (close(file_no) == -1) {
                        perror("close");
                        return 1;
                }
        }
        iwc_print_total_counter(lc, wc, bc);

        return 0;
}
