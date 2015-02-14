#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 1024

char buf[BUF_SIZE];

int main(int argc, char **argv) {

        int nread = 0;
        int newlines = 0;

        while ((nread = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
                for (int i=0; i<nread; i++) {
                        if (buf[i] == '\n') {
                                newlines += 1;
                        }
                }

                printf("%d\r", newlines);
                fflush(NULL);
        }

        printf("%d\n", newlines);

        return 0;
}
