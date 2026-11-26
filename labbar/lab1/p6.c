#include <stdio.h>
#include <stdlib.h> // för att kunna konvertera från ASCII till tal
#include <string.h>
#include <math.h>

int main (int argc, char *argv[]) {

    if (argc != 3) {
        printf("Invalid number of arguments\n");
        return 1;
    }

    int orgA = atoi(argv[1]);
    int orgB = atoi(argv[2]);
    int A = orgA;
    int B = orgB;  
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
            printf("gdc(%d,%d) = %d\n", orgA, orgB, gcd);
        }
    }
    return 0;
}