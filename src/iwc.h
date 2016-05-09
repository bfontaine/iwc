#ifndef _IWC_H
#define _IWC_H

#define counter_t unsigned long long
#define COUNTER_FMT " %7llu"

void iwc_count_bytes(int buflen, counter_t *bytes);
void iwc_count_lines(int buflen, counter_t *lines);
void iwc_count_words(int buflen, counter_t *words);

int iwc_counts(int fileno, counter_t *lines, counter_t *words,
                counter_t *bytes);

void iwc_print_counter(counter_t *lines, counter_t *words, counter_t *bytes,
                char eol);
void iwc_print_total_counter(counter_t *lines, counter_t *words,
                counter_t *bytes);

#endif
