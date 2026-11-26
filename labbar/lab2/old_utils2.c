#include <stdio.h>
#include "utils.h"


void clear_input_buffer() {
    int c; //initiera varaiabel
    do {
        c = getchar(); //reads a character from input
    }
    while (c != '\n' && c != EOF); //Keeps reading as long as c is not end of file or null terminator. 
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

char *ask_questions_string(char *question, char *buf, int buf_size) {
    int lenght = 0;

    do { // do at least once
        printf("%s\n", question);    //print our input question
        lenght = read_string(buf, buf_size); // call read_string to find out the lenght of our string buf
    }
    while (lenght == 0); // print question again if input was an empty string

    return buf; //return string
}

// int main(void) {
//     int buf_size = 255;
//     int read = 0;
//     char buf[buf_size];

//     puts("Läs in en sträng: ");
//     read = read_string(buf, buf_size);
//     printf("'%s' (%d tecken)\n", buf, read);

//     puts("Läs in en sträng till: ");
//     read = read_string(buf, buf_size);
//     printf("'%s' (%d tecken)\n", buf, read);

//     return 0;
// }
