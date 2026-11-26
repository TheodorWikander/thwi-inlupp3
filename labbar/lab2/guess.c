#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../utils.h"

int main () {
    int randint = random() % 1024;

    int buf_size = 255;  
    int guess_count = 0;
    int last_guess = 0;
    char buf_username[buf_size]; //array med 255 platser där användarnamn ska spras
    char *question1 = "kan du gissa vilket?"; //skickas med ask_questio_int som argument för att kalla på funktionen 

    strcpy(buf_username, ask_question_string("Skriv in ditt namn: ")); // Fråga o namn och kopiera över svaret till arrayen buf_username mha strcpy
    //Alternativt sätt char *username = ask_question_string direkt och ta bort buf_username och buf_size då de inte längre används samt uppdatera från
    //buf_username till username på samtliga platser om vi ej vill använda strcpy

    printf("Du %s, jag tänker på ett tal ... ", buf_username);

    do {    //Gör minst en gång, upprepa sålänge vilkoret är uppfyllt (dvs upprepa till vi har rätt eller slut på gissningar)
        last_guess = ask_question_int(question1); //fråga om gissning
        if (last_guess == randint) { //Klar om vi gissar rätt
            guess_count++; //öka count för att hålla koll på antal gissningar
            printf("Bingo!\nDet tog %s %d gissningar att komma fram till %d\n", buf_username, guess_count, randint);
            return 0;
        } else if (last_guess < randint) { //Öka count vid varje gissning, ge ledtråd och försök igen så länge vi har gissningar kvar och inte gissat rätt
            guess_count++;
            printf("För litet!\n");
            continue;
        } else if (last_guess > randint) {
            guess_count++;
            printf("För stort!\n");
            continue;
        }
    } while (last_guess != randint && guess_count < 15); 
    
    //Hit kommer vi bara om vi har slut på gissningar och inte lyckats gissa rätt
    printf("Nu har du slut på gissningar! Jag tänkte på %d\n", randint);
    return 0;
}