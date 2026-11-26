#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../utils.h"

// void print(char *str) {
//     for(int i = 0; i < string_lenght(str); ++i) {
//         putchar(str[i]);
//     }
// }

void print(char *str) {
    while (*str != '\0') {
        putchar(*str);
        ++str;
    }
}

int main(void)
{
    printf("=== TEST 1: print() [ska skrivas ut på SAMMA rad] ===\n");
    print("Detta ");
    print("är ");
    print("tre ");
    print("print()-anrop.");
    printf("\n\n"); // Radbrytning för att separera testerna

    return 0;
}
