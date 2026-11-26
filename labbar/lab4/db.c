#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../utils.h"

struct item {
    char *name;
    char *description;
    int price;
    char *shelf;
};

typedef struct item item_t;

void print_item(item_t *item) {
    printf("Name: %s\n", item->name);
    printf("Description: %s\n", item->description);
    printf("Price: %d.%02d SEK\n",item->price/ 100, item->price % 100);
    printf("Shelf: %s\n", item->shelf);
}

//Hjälpfunktion till input item senare
item_t make_item(char *name, char *description, int price, char *shelf) {
    item_t item = {
                    .name = name, .description = description,
                    .price = price, .shelf = shelf
                };
    return item;
}

bool shelf_valid(char *str) {
    int len = strlen(str);
    
    //för att en hylla ska vara valid krävs minst 
    //en siffra och en bokstav. 
    if (len < 2) {
        return false;
    } 

    //Om första tecknet inte är en bokstav är hyllan invalid
    if (!isalpha(str[0])) {
        return false;
    }

    //Resten av eementen måste vara siffror anars invalid
    for (int i = 1; i < len; ++i) {
        if(!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}


char *ask_question_shelf(char *question) {
    //ask_question tar emot en fråga (sträng) en valideringsfunktione, shefl_valid som körs om och om igen tills det vi får är valid. 
    //oche n konverteringsfunktion i detta fall strdup som som kopierar användarens svar och sparar det på heapen så att svaraet sparas
    //även när aks_question har kört klart. Måste frigöra när vi vet att vi är klar med svaret.  (convert_func *) säger att vi ska behandla
    //strdup som en valid konverteringsfuntkion. Ask_question returnerar answer_t men ask_question_shelf ska returnera en sträng så vi plockar ut vårt sträng från answe_t
    return ask_question(question, shelf_valid, (convert_func *) strdup).string_value;
}


item_t input_item(void) {
    char *name = ask_question_string("Vad heter varan? ");
    char *desc = ask_question_string("Beskriv varan: ");
    int price = ask_question_int("Vad kostar varan? ");
    char *shelf = ask_question_shelf("Vart finns varan? ");

    return make_item(name, desc, price, shelf);
}

char *magick(char *arr1[], char *arr2[], char *arr3[], int len) {
    char buf[255];

    int randint1 = random() % len;
    int randint2 = random() % len;
    int randint3 = random() % len;

    strcpy(buf, arr1[randint1]); //strcpy kopierar till buf dvs försvinenr allt som varit där innan och byts ut med bara arr1[randint1]
    strcat(buf, "-");   //strcat läggr till i slutet av buf
    strcat(buf, arr2[randint2]);
    strcat(buf, " ");
    strcat(buf, arr3[randint3]); //strcat lägge till \0 själv så vi behöver inte lägga till det manuellt

    return strdup(buf);
}

void list_db(item_t *items, int no_items) {
    for (int i = 0; i < no_items; ++i) {
        printf("%d. %s \n", i + 1, items[i].name); //i + 1 då i på första elementet är 0 och vi vill att det står 1. 
    }
}

void edit_db(item_t *items, int *no_items) {
    int item_no = ask_question_int("\nVilken vara vill du ändra?"); //tar emot numret på aran vi ill ändra från vår öista
    if (item_no < 0 || item_no > *no_items) { //Om vi skickar in ett tomt svar eller ex 20 om vår lista gpr till 16
        printf ("Ogiltig vara\n");
    } else {
        print_item(&items[item_no-1]); //Anasr skriver ut den vaar vi avlt att ändra
        printf("\nAnge ny vara: \n"); 
        item_t new_item = input_item(); //skapar ny vara
        items[item_no - 1] = new_item; //då utskriva siffran 1. motsvara index 0
    }
}


// Utkommenterad för att kunna köra mainfilen i cat.c då jag importerat från db.c och kopilatorn ser att det finns två olika mainfiler 
// i db.c och cat.. Går att lösa genom att bara kopiera in frunktioner från db.c till cat.c och ta bort includen. ALternativt lägga de i en egen fil och 
// importera den filen till cat.c

// int main(int argc, char *argv[])
// {
//   char *array1[] = { "fisk", "gås", "hund" }; 
//   char *array2[] = { "hav", "himmel", "land" }; 
//   char *array3[] = { "hammare", "spik", "skruv" }; 

//   if (argc < 2)
//   {
//     printf("Usage: %s number\n", argv[0]);
//   }
//   else
//   {
//     item_t db[16]; // Array med plats för 16 varor
//     int db_siz = 0; // Antalet varor i arrayen just nu

//     int items = atoi(argv[1]); // Antalet varor som skall skapas

//     if (items > 0 && items <= 16)
//     {
//       for (int i = 0; i < items; ++i)
//       {
//         // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
//         item_t item = input_item();
//         db[db_siz] = item;
//         ++db_siz;
//       }
//     }
//     else
//     {
//       puts("Sorry, must have [1-16] items in database.");
//       return 1; // Avslutar programmet!
//     }

//     for (int i = db_siz; i < 16; ++i)
//       {
//         char *name = magick(array1, array2, array3, 3); 
//         char *desc = magick(array1, array2, array3, 3); 
//         int price = random() % 200000;
//         char shelf[] = { random() % ('Z'-'A') + 'A',
//                          random() % 10 + '0',
//                          random() % 10 + '0',
//                          '\0' };
//         item_t item = make_item(name, desc, price, strdup(shelf));

//         db[db_siz] = item;
//         ++db_siz;
//       }

//      // Skriv ut innehållet
//      list_db(db, db_siz);
//     //  for (int i = 0; i < db_siz; ++i)
//     //  {
//     //     printf("\n");
//     //     print_item(&db[i]);
//     //  }

//     edit_db(db, &db_siz);
//     list_db(db, db_siz);

//   }
//   return 0;
// }