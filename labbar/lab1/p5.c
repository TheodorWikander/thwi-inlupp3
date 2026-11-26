#include <stdio.h>
#include <stdlib.h> // för att kunna konvertera från ASCII till tal
#include <string.h>
#include <math.h> // För att kunna använda sqrt() funktionen

int main(int argc, char *argv[]) {

     if (argc != 2) {
        puts("Invalid number of arguments");
        return 1;
     }
    int N = atoi(argv[1]);
    int is_prime = 1; //Hålla koll på om primtal eller inte
                      // Där 1 = primtal

    if (N <= 1) {
        is_prime = 0;  
    } else {
        int limit = floor(sqrt(N)) + 1;
        for (int x = 2; x < limit; ++x) {
            for (int y = 2; y <= N; ++y) {
                if (x * y == N) {
                is_prime = 0;
                break;
                }
            }
        

            if (!is_prime) {
                break;
            }
        }
    }
     
    if (is_prime == 1) {
        printf("%d is a prime number\n", N);
    } else {
        printf("%d is not a prime number\n", N);
    }
}
