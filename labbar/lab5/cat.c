#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "../utils.h"
#include <stdlib.h>
#include "/home/noed5571/ioopm/labbar/lab4/db.c"

void cat (char *filename) {
    FILE *f = fopen(filename, "r");
    int c = fgetc(f);
    int count = 1; 

    printf("%d. ", count);
    while (c != EOF) {
        fputc(c, stdout);
        if (c == '\n') {
            ++count;
            printf("%d. ", count);
        }
        c = fgetc(f);
    }
    fclose(f);
}

void print_menu() {
    printf("[L]ägga till en vara \n"
            "[T]a bort en vara \n"
            "[R]edigera en vara\n"
            "Ån[g]ra senaste ändringen\n"
            "Lista [h]ela katalogen\n"
            "[A]vsluta\n ");
}

bool menu_valid(char *str) {

    if (strlen(str) != 1) { //Om vi skickar in mer än ett tecken kan det vi skickat in ej vara giltigt
        return false;
    }
    if (strchr("LlTtRrGgHhAa", str[0]) == NULL) { //Om det vi skickat in inte är ett av meny-alternatuven är det inte giltigt
        return false;
    }
    return true;
}

char ask_question_menu() {
    print_menu();

    char *answer = ask_question_string("Ditt val: "); 

    char choice = toupper(answer[0]); //Ändra så allt skrivs i stora bokstäver och vi inte måste jämföra a och A för sig. 

    free(answer); //ask_question_string allokerar minne mha av strdup i 
    //Ask question string. Detta minne måset frigöras manuellt för att inte orsaka memeory leaks
    //då variabeln answer (pekare) avslutas när funktionen returnerar choice men värdet answe pekar på
    //ligger kvar på heapen om vi inte friar. 

    return choice;
}

void add_item_to_db(item_t db[], int *db_siz) {
    item_t new_item = input_item(); //Skapa det nya itemet
    db[*db_siz] = new_item; //Lägg in nya i slutet av arrayen //stod att vi kunde anta att det fanns plats anars måste man skriva in en kontroll
    *db_siz = *db_siz + 1; //Öka storkelen på arrayen med 1. 
}


void remove_item_from_db(item_t *db, int *db_siz) { //db_siz måset vara en pekare för att funktionen ska kunna ändra 
    //på orginalvariabeln i minnet istället för att bara ändra en kopia lokalt. Samma i add och edit. Behövs ej i list då list aldrig ska ändra något
    list_db(db, *db_siz);
    int item_no = ask_question_int("\nVilken vara vill du ta bort?");
    if (item_no < 0 || item_no > *db_siz) {
        printf ("Ogiltig vara\n");
    } else {
        int index = item_no + 1;

        for (int i = index; i < *db_siz - 1; ++i) {
            (db[i] = db[i+1]);
        }
        *db_siz = *db_siz - 1;
    }
}

void event_loop(item_t *db, int *db_siz) { //Redovisas lab 5
    while (true) {
        char choice = ask_question_menu(); //Spara input som choice

        if (choice == 'L') {
            add_item_to_db(db, db_siz);
        }
        else if (choice == 'T') {
            remove_item_from_db(db, db_siz); 
        } 
        else if (choice == 'R') {
            edit_db(db, db_siz);
        }
        else if (choice == 'G') {
            printf("not yet implemented!\n");
        }
        else if (choice == 'H') {
            list_db(db, *db_siz);
        }
        else if (choice == 'A') {
            break;     
        }
    }
}       


int main(void) {
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0; // Antalet varor i arrayen just nu
    event_loop(db, &db_siz);
    return 0;
}
