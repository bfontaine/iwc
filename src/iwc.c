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
                double multiplier,
                char eol) {

        char printed = 0;

        if (multiplier < 1) { multiplier = 1; }

        if (lines != NULL) {
                printf(COUNTER_FMT, (counter_t)(*lines * multiplier));
                printed = 1;
        }

        if (words != NULL) {
                printf(COUNTER_FMT, (counter_t)(*words * multiplier));
                printed = 1;
        }

        if (bytes != NULL) { printf(COUNTER_FMT, *bytes); printed = 1; }

        if (printed) {
                printf("%c", eol);
                fflush(NULL);
        }
}

void iwc_print_total_counter(counter_t *lines, counter_t *words,
                counter_t *bytes) {
        iwc_print_counter(lines, words, bytes, 1, '\n');
}

int iwc_counts(int fileno, counter_t *lines, counter_t *words,
                counter_t *bytes) {

        int nread = 0;
        off_t size = -1;

        counter_t total_read = 0;

        double multiplier = 1,
               known_size = 0;

        size = lseek(fileno, 0, SEEK_END);

        if (size > 0) {
                if (bytes != NULL) {
                        *bytes += size;

                        // shortcut for `iwc -c somefile`
                        if (lines == NULL && words == NULL) {
                                return 0;
                        }
                }

                known_size += size;

                if (lseek(fileno, 0, SEEK_SET) != 0) {
                        return -2;
                }
        }

        while ((nread = read(fileno, buf, BUF_SIZE)) > 0) {
                iwc_count_lines(nread, lines);
                iwc_count_words(nread, words);

                if (!known_size) {
                        iwc_count_bytes(nread, bytes);
                } else {
                        total_read += nread;
                        multiplier = known_size / total_read ;
                }

                iwc_print_counter(lines, words, bytes, multiplier, '\r');
        }

        return nread;
}

int iwc_error(int res) {
        switch(res) {
        case -1:
                perror("read");
                return -1;
        case -2:
                perror("lseek");
                return -1;
        default:
                return 0;
        }
}
