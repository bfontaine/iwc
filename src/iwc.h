#ifndef _IWC_H
#define _IWC_H

#define count_t unsigned long long
#define COUNTER_FMT " %7llu"

typedef struct {
        char* buf;
        int buflen;
        count_t lines, words, bytes;

        char count_lines,
             count_words,
             count_bytes;

        // This is used when counting words
        char last_char_was_a_space;
        char pristine;
} counter_t;

counter_t *iwc_make_counter(void);
int iwc_count(counter_t *c, int fileno);
void iwc_print_total(counter_t *c);
void iwc_destroy_counter(counter_t *c);

#endif
