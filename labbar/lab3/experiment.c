#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../utils.h"

extern char *strdup(const char *);

// typedef union {
//     int int_value;
//     float float_value;
//     char *string_value;
// } answer_t;

typedef bool check_func(char *);
//typedef answer_t convert_func(char *);

// bool not_empty(char *str) {
//     if (strlen(str) > 0) {
//         return true;
//     } 
//     return false;
// }

//Tagen från gemini 2026.09.14 då jag inte gjort en is_float i någon tidigare labb då den var frivillig
// och uppgiften krävde en. 
// bool is_float(char *str)
// {
//     int len = strlen(str);
//     if (len == 0) return false;

//     int start = 0;
//     // Hantera valfritt minustecken eller plustecken i början
//     if (str[0] == '-' || str[0] == '+')
//     {
//         if (len == 1) return false; // Bara "-" eller "+" är inte ett tal
//         start = 1;
//     }

//     bool has_dot = false;
//     int digits = 0;

//     for (int i = start; i < len; ++i)
//     {
//         if (str[i] == '.')
//         {
//             // Om vi redan stött på en punkt tidigare är det ogiltigt (t.ex. "1.2.3")
//             if (has_dot) return false;
//             has_dot = true;
//         }
//         else if (isdigit(str[i]))
//         {
//             digits++;
//         }
//         else
//         {
//             // Om det är något annat tecken än siffra eller punkt
//             return false;
//         }
//     }

//     // Kräver att det finns minst en siffra (t.ex. är enbart "." inte ett giltigt tal)
//     return digits > 0;
// }

// answer_t ask_question(char *question, check_func *check, convert_func *convert) {
//     int buf_size = 255;
//     char buf[buf_size]; 

//     do {
//         printf("%s\n", question);
//         read_string(buf, buf_size);
//     } while (!check(buf));

//     answer_t result = convert(buf);
//     return result;
// }

// int ask_question_int(char *question) {
//     answer_t answer = ask_question(question, is_number, (convert_func *) atoi);
//     return answer.int_value; //returnera som heltal
// }

// answer_t make_float(char *str)
// {
//   return (answer_t) { .float_value = atof(str) };
// }

// double ask_question_float(char *question) {
//     return ask_question(question, is_float, make_float).float_value;
// }

// char *ask_question_string(char *question) {
//     return ask_question(question, not_empty, (convert_func *) strdup).string_value;
// }

int main(void) {
    printf("--- TEST 1: Heltal ---\n");
    int age = ask_question_int("Skriv in din ålder:");
    printf("Du är %d år gammal.\n\n", age);

    printf("--- TEST 2: Sträng ---\n");
    char *name = ask_question_string("Vad heter du?");
    printf("Hej %s!\n\n", name);

    printf("--- TEST 3: Decimaltal ---\n");
    double height = ask_question_float("Hur lång är du i meter (t.ex. 1.75)?");
    printf("Du är %.2f meter lång.\n", height);

    return 0;
}

