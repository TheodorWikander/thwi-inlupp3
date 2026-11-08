#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.h"

void clear_input_buffer() {
    int c; //initiera varaiabel
    do {
        c = getchar(); //reads a character from input
    }
    while (c != '\n' && c != EOF); //Keeps reading as long as c is not end of file or null terminator. 
}

bool is_number(char *str) {
    if (strlen(str) < 1) { //En tom sträng är inget nummer
        return false;
    } else {
        for (size_t i = 0; i < strlen(str); ++i) { //För varje element i strängen förutom den sista (null-terminatorn), gör nedanstående och öka i med 1
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

int read_string(char *buf, int buf_size) {
    
    int c;
    int count = 0; // initializing varaibels

    while (count < buf_size - 1) { //For every character that fits inside our buffer (last element need to be null terminator so we need to "save" one spot)
        c = getchar(); //Get the character

        if (c == '\n' || c == EOF) {  //Check wheter the character is the ned of file or the null terminator
            buf[count] = '\0'; //If that is the case add null terminator to our string to mark the string ending
            return count; //return number of characters before null terminator
        }
        buf[count] = c; //If charater. is not EOF or null terminator
        count++; //Increase count by one
    }

    buf[count] = '\0'; // When we have reached the end of our string or our buffer is full set the last element to null terminator
    clear_input_buffer(); //Clear the buffert if we have not reached end of string

    return count; 

}

//Tagen från gemini 2026.09.14 då jag inte gjort en is_float i någon tidigare labb
// och uppgiften krävde en. 
bool is_float(char *str)
{
    int len = strlen(str);
    if (len == 0) return false;

    int start = 0;
    // Hantera valfritt minustecken eller plustecken i början
    if (str[0] == '-' || str[0] == '+')
    {
        if (len == 1) return false; // Bara "-" eller "+" är inte ett tal
        start = 1;
    }

    bool has_dot = false;
    int digits = 0;

    for (int i = start; i < len; ++i)
    {
        if (str[i] == '.')
        {
            // Om vi redan stött på en punkt tidigare är det ogiltigt (t.ex. "1.2.3")
            if (has_dot) return false;
            has_dot = true;
        }
        else if (isdigit(str[i]))
        {
            digits++;
        }
        else
        {
            // Om det är något annat tecken än siffra eller punkt
            return false;
        }
    }

    // Kräver att det finns minst en siffra (t.ex. är enbart "." inte ett giltigt tal)
    return digits > 0;
}

bool not_empty(char *str) {
    if (strlen(str) > 0) {
        return true;
    } 
    return false;
}

answer_t make_float(char *str)
{
  return (answer_t) { .float_value = atof(str) };
}

answer_t ask_question(char *question, check_func *check, convert_func *convert) {
    int buf_size = 255;
    char buf[buf_size]; 

    do {
        printf("%s\n", question);
        read_string(buf, buf_size);
    } while (!check(buf));

    answer_t result = convert(buf);
    return result;
}

static answer_t make_int(char *str) {
    return (answer_t) {.int_value = atoi(str)};
}

static answer_t make_string(char *str) {
    return (answer_t) {.string_value = strdup(str)};
}

int ask_question_int(char *question) {
    answer_t answer = ask_question(question, is_number, make_int);
    return answer.int_value; //returnera som heltal
}

double ask_question_float(char *question) {
    return ask_question(question, is_float, make_float).float_value;
}

char *ask_question_string(char *question) {
    return ask_question(question, not_empty, make_string).string_value;
}

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
}

size_t string_hash(elem_t key) {
    size_t h = 5381;
    for (char *p = key.s; *p != '\0'; ++p) {
        h = h *33 + (unsigned char) *p;
    }
    return h;
}

bool string_eq(elem_t a, elem_t b) {
    return strcmp(a.s, b.s) == 0;
}

size_t int_hash(elem_t key) {
    return (size_t) (unsigned int) key.i;
}

bool int_eq(elem_t a, elem_t b) {
    return a.i == b.i;
}