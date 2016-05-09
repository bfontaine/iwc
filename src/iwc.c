#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "iwc.h"
#include "version.h"

// this is the size `wc` is using
#define BUF_SIZE (16 * 1024)

char buf[BUF_SIZE];

void iwc_count_lines(int buflen, counter_t *lines) {
        int _lines = 0;
        char *p = buf;

        if (lines == NULL) { return; }

        while ((p = memchr(p, '\n', (buf + buflen) - p))) {
                ++p;
                ++_lines;
        }

        *lines += _lines;
}

void iwc_count_words(int buflen, counter_t *words) {
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

void iwc_count_bytes(int buflen, counter_t *bytes) {
        if (bytes != NULL) {
                *bytes += buflen;
        }
}

void iwc_print_counter(counter_t *lines, counter_t *words, counter_t *bytes,
                char eol) {
        char printed = 0;
        if (lines != NULL) { printf(COUNTER_FMT, *lines); printed = 1; }
        if (words != NULL) { printf(COUNTER_FMT, *words); printed = 1; }
        if (bytes != NULL) { printf(COUNTER_FMT, *bytes); printed = 1; }

        if (printed) {
                printf("%c", eol);
                fflush(NULL);
        }
}

void iwc_print_total_counter(counter_t *lines, counter_t *words,
                counter_t *bytes) {
        iwc_print_counter(lines, words, bytes, '\n');
}

int iwc_counts(int fileno, counter_t *lines, counter_t *words,
                counter_t *bytes) {

        int nread = 0;

        while ((nread = read(fileno, buf, BUF_SIZE)) > 0) {
                iwc_count_lines(nread, lines);
                iwc_count_words(nread, words);
                iwc_count_bytes(nread, bytes);

                iwc_print_counter(lines, words, bytes, '\r');
        }

        return nread;
}
