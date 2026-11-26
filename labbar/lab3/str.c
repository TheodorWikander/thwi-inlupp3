#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int string_lenght(char *argv) {
    int count = 0;

    for (int i = 0; argv[i] != '\0'; ++i) {
        count = count + 1;
    }

    return count;

}

void print(char *str) {
    for(int i = 0; i < string_lenght(str); ++i) {
        putchar(str[i]);
    }
}

void println( char *str) {
    print(str);
    print("\n");
}



// int main(int argc, char *argv[]) {
//     if (argc < 2) {
//         printf("Usage: %s words or string", argv[0]);
//     } 
//     else {
//         for (int i = 1; i < argc; ++i) {
//             int expected = strlen(argv[i]);
//             int actual = string_lenght(argv[i]);
//             printf("strlen(\"%s\")=%d\t\tstring_lenght(\"%s\")=%d\n",
//                     argv[i], expected, argv[i], actual);
//         }
//     }
//     return 0;
// }

int main(int argc, char *argv[]) {
    printf("=== TEST 1: print() [ska skrivas ut på SAMMA rad] ===\n");
    print("Detta ");
    print("är ");
    print("tre ");
    print("print()-anrop.");
    printf("\n\n"); // Radbrytning för att separera testerna

    printf("=== TEST 2: Jämförelse mellan println() och puts() ===\n");
    printf("Utskrift med println():\n");
    println("Rad 1 med println");
    println("Rad 2 med println");

    printf("\nUtskrift med puts():\n");
    puts("Rad 1 med puts");
    puts("Rad 2 med puts");

    printf("\n=== TEST 3: Tom sträng ===\n");
    print("Start [");
    println(""); // Ska endast skapa en ny rad
    print("] Slut\n");

    return 0;
}