#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "iwc.h"
#include "version.h"

// this is the size `wc` is using
#define BUF_SIZE (16 * 1024)

counter_t *iwc_make_counter(void) {
        counter_t *c = (counter_t*)malloc(sizeof(counter_t));

        if (c == NULL) {
                perror("malloc");
                return NULL;
        }

        memset(c, 0, sizeof(*c));

        c->buf = (char*)malloc(sizeof(char) * BUF_SIZE);

        if (c->buf == NULL) {
                perror("malloc");
                goto error;
        }

        return c;
error:
        iwc_destroy_counter(c);
        return NULL;
}

void iwc_destroy_counter(counter_t *c) {
        if (c == NULL) {
                return;
        }
        free(c->buf);
        free(c);
}

void iwc_update_lines_count(counter_t *c) {
        int _lines = 0;
        char *p = c->buf;

        if (!c->count_lines || c->buflen == EOF) { return; }

        while ((p = memchr(p, '\n', (c->buf + c->buflen) - p))) {
                ++p;
                ++_lines;
        }

        c->lines += _lines;
}

void iwc_update_words_count(counter_t *c) {
        int _words = 0;

        if (!c->count_words) { return; }

        if (c->buflen == EOF) {
                if (!c->last_char_was_a_space && !c->pristine) {
                        ++(c->words);
                }
                return;
        }

        for (int i=0; i<c->buflen; ++i) {
                char ch = c->buf[i];

                if (ch == '\n' ||
                    ch == '\r' ||
                    ch == '\t' ||
                    ch == '\v' ||
                    ch == '\f' ||
                    ch == ' ') {

                        if (c->last_char_was_a_space) { continue; }

                        c->last_char_was_a_space = 1;

                        if (!c->pristine) { ++_words; }
                } else {
                        c->last_char_was_a_space = 0;
                }
        }

        c->words += _words;
}

void iwc_update_bytes_count(counter_t *c) {
        if (c->count_bytes) {
                c->bytes += c->buflen;
        }
}

void iwc_print_counter(counter_t *c, char eol) {
        if (c->count_lines) { printf(COUNTER_FMT, c->lines); }
        if (c->count_words) { printf(COUNTER_FMT, c->words); }
        if (c->count_bytes) { printf(COUNTER_FMT, c->bytes); }

        putchar(eol);
        fflush(NULL);
}

int iwc_count(counter_t *c, int fileno) {
        int ret = 0;

        while ((c->buflen = read(fileno, c->buf, BUF_SIZE)) >= 0) {
                iwc_update_bytes_count(c);
                iwc_update_words_count(c);
                iwc_update_lines_count(c);

                c->pristine = 0;

                iwc_print_counter(c, '\r');

                if (c->buflen == 0) {
                        break;
                }
        }

        ret = c->buflen;
        c->buflen = 0;
        return ret;
}

void iwc_print_total(counter_t *c) {
        iwc_print_counter(c, '\n');
}
