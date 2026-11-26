#include <stdio.h>
#include "utils.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int ask_questions_int(char *question) {

    int result = 0;
    int conversations = 0;

    do {
        printf("%s\n", question); // skriv ut vår inputfråga
        conversations = scanf("%d", &result); // spara i result
        
        if (conversations != 1) {
            clear_input_buffer(); // rensa felaktig input  (scanf returnerar 1 om den misslyckas)
        } else {
            clear_input_buffer(); // rensa resten med
        }
    } while (conversations != 1);

    return result;
    
}

// int main (void) {
//     int tal;

//     tal = ask_questions_int("Första talet: ");
//     printf("Du skrev '%d'\n", tal);

//     tal = ask_questions_int("Andra talet: ");
//     printf("Du skrev '%d'\n", tal);

//     return 0;
// }