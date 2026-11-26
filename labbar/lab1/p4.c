#include <stdio.h>
#include <stdlib.h> // för att kunna konvertera från ASCII till tal
#include <string.h>

int main(int argc, char *argv[]) {

     if (argc != 3) {
        puts("Invalid number of arguments");
        return 1;
     }

     int rows = atoi(argv[1]);
     int growth = atoi(argv[2]);
     int total = 0;

     for (int i = 1; i <= rows; ++i) {
        int stars = i * growth;
        for (int j = 0; j < stars; ++j) {
            printf("*");
        }
    
        printf("\n");
        total += stars;
     }

        printf("Totalt: %d\n", total);

    return 0;
}

