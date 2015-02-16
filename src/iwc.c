#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "iwc.h"
#include "version.h"

// this is the size `wc` is using
#define BUF_SIZE (16 * 1024)

char buf[BUF_SIZE];

void iwc_count_lines(int buflen, int *lines) {
        int _lines = 0;
        char *p = buf;

        if (lines == NULL) { return; }

        while ((p = memchr(p, '\n', (buf + buflen) - p))) {
                ++p;
                ++_lines;
        }

        *lines += _lines;
}

void iwc_count_words(int buflen, int *words) {
        int _words = 0;
        char last_was_a_space = 0;

        if (words == NULL) { return; }

        for (int i=0; i<buflen; ++i) {
                char c = buf[i];

                if (c == '\n' ||
                    c == '\r' ||
                    c == '\t' ||
                    c == '\v' ||
                    c == '\f' ||
                    c == ' ') {

                        if (last_was_a_space) { continue; }

                        last_was_a_space = 1;

                        ++_words;
                } else {
                        last_was_a_space = 0;
                }
        }

        *words += _words;
}

void iwc_count_bytes(int buflen, int *bytes) {
        if (bytes != NULL) {
                *bytes += buflen;
        }
}

void iwc_print_counter(int *lines, int *words, int *bytes, char eol) {
        char printed = 0;
        if (lines != NULL) { printf(" %7d", *lines); printed = 1; }
        if (words != NULL) { printf(" %7d", *words); printed = 1; }
        if (bytes != NULL) { printf(" %7d", *bytes); printed = 1; }

        if (printed) {
                printf("%c", eol);
                fflush(NULL);
        }
}

void iwc_print_total_counter(int *lines, int *words, int *bytes) {
        iwc_print_counter(lines, words, bytes, '\n');
}

int iwc_counts(int fileno, int *lines, int *words, int *bytes) {

        int nread = 0;

        while ((nread = read(fileno, buf, BUF_SIZE)) > 0) {
                iwc_count_lines(nread, lines);
                iwc_count_words(nread, words);
                iwc_count_bytes(nread, bytes);

                iwc_print_counter(lines, words, bytes, '\r');
        }

        return nread;
}
