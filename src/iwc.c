#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "iwc.h"
#include "version.h"

#define BUF_SIZE 1024

char buf[BUF_SIZE];

void iwc_count_chars(int buflen, int *chars) {
        if (chars != NULL) {
                *chars += buflen;
        }
}

void iwc_count_lines(int buflen, int *lines) {
        int _lines = 0;

        if (lines == NULL) { return; }

        for (int i=0; i<buflen; i++) {
                if (buf[i] == '\n') {
                        _lines += 1;
                }
        }

        *lines += _lines;
}

void iwc_count_words(int buflen, int *words) {
        int _words = 0;
        char last_was_a_space = 0;

        if (words == NULL) { return; }

        for (int i=0; i<buflen; i++) {
                char c = buf[i];

                // really simple, just for testing
                if (!last_was_a_space && (c == '\n' || c == '\r' || c == ' ')) {
                        last_was_a_space = 1;
                        _words += 1;
                } else {
                        last_was_a_space = 0;
                }
        }

        *words += _words;
}

void iwc_print_counter(int *chars, int *lines, int *words, char eol) {
        char printed = 0;
        if (chars != NULL) { printf("%d ", *chars); printed = 1; }
        if (lines != NULL) { printf("%d ", *lines); printed = 1; }
        if (words != NULL) { printf("%d ", *words); printed = 1; }

        if (printed) {
                printf("%c", eol);
                fflush(NULL);
        }
}

int iwc_counts(int fileno, int *chars, int *lines, int *words) {

        int nread = 0;

        while ((nread = read(fileno, buf, BUF_SIZE)) > 0) {
                iwc_count_chars(nread, chars);
                iwc_count_lines(nread, lines);
                iwc_count_words(nread, words);

                iwc_print_counter(chars, lines, words, '\r');
        }

        iwc_print_counter(chars, lines, words, '\n');

        return nread;
}
