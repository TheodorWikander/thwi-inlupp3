#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h> //isdigit() kollar om ett tecken är en siffra
#include <string.h>

void print_number(int argc) {
    int T = argc;
    if (T % 5 == 0 && T % 3 == 0) {
        printf("Fizz Buzz");
    } else if (T % 5 == 0) {
        printf("Buzz");
    } else if (T % 3 == 0) {
        printf ("Fizz");
    } else {
        printf("%d", T);
    }
}

int main(int argc, char *argv[]) {
    int J = atoi(argv[1]);
    for (int i = 1; i <= J; ++i) {
        print_number(i);
        if (i != J) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
    return 0;
}