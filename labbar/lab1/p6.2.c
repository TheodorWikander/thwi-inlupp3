#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //isdigit() kollar om ett tecken är en siffra
#include <string.h> // strlen()

bool is_number(char *str) {
    if (strlen(str) < 1) {
        return false;
    } else {
        for (int i = 0; i < strlen(str); ++i) {
            if (str[0] == '-') {
                if (!isdigit(str[(++i)])) {
                    return false;
                }
            } else if (!isdigit(str[i])) {
                return false;
            };
        } 
    }
}

int main (int argc, char *argv[]) {

    if (argc != 3) {
        printf("Invalid number of arguments\n");
        return 1;
    }

    if (is_number(argv[1]) && is_number(argv[2])) {

        int arg1 = atoi(argv[1]);
        int arg2 = atoi(argv[2]);

        if (arg1 >= 1 && arg2 >= 1) {

        //int orgA = atoi(argv[1]);
        //int orgB = atoi(argv[2]);
            int A = arg1;
            int B = arg2;
            int gcd = 0;

            while (gcd != A && gcd != B) {
                if (A > B) {
                    A = (A - B);
                    continue;
                } else if (B > A) {
                    B = (B - A);
                    continue;
                } else if (A == B) {
                    gcd = A;
                    printf("gdc(%d,%d) = %d\n", arg1, arg2, gcd);
                }   
            }
        } else {
            printf("Please use non-negative integers\n");
        }
        } else {
            printf("Input not numbers\n");
        } 
    return 0;
}
