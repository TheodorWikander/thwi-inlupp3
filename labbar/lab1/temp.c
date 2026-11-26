#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //isdigit() kollar om ett tecken är en siffra
#include <string.h> // strlen()

// 1 = true

//tar emot en sträng och returnerar true om strängen är
// ett positivt eller netativt tal, och anars false
bool is_number(char *str) {
    if (strlen(str) < 1) { //En tom sträng är inget nummer
        return false;
    } else {
        for (int i = 0; i < strlen(str); ++i) { //För varje element i strängen förutom den sista (null-terminatorn), gör nedanstående och öka i med 1
            if (str[0] == '-') { //Om första elementet i strängen är ett minustecken, kolla på nästa element tills strängen är slut för att avgöra 
                if (!isdigit(str[(++i)])) { //Om elementet efter minusteckent inte är en siffra är strängen inte en siffra
                    return false;
                }
            } else if (!isdigit(str[i])) { //Om första elementet inte är ett minustecken, kolla om det är en siffra
                return false;
            };
        } 
    }
    return true; //Om vi tar oss hit är strängen en siffra/ett tal. 
}

int main (int argc, char *argv[]) {
    if (argc > 1 && is_number(argv[1])) { //Om vi skickar in minst ett argument utöver programnamnet och denna sträng är ett nummer
        printf("%s is a number\n", argv[1]); //Skriv ut att strängen är ett nummer
    } else {
        if (argc > 1) { // Om vi inte skickat in en tom sträng men stängern inte är ett nummer
            printf("%s is not a number\n", argv[1]);
        }
        else {
            printf("Please provide a command line argument!\n"); //Om vi skickat in en tom sträng
        }
    }
    return 0;
}