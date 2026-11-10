#include <stdbool.h>
#include <string.h>
#include "../generic_structures/common.h"

/**
 * @file utils.h
 * @brief Generic utilities: reading validated input from stdin,
 *        simple printing and hash/equality functions for use with the hash table
 * 
 * Knows nothing about the webstore. Functions that read input print the
 * question and ask again until the input passes a check function.
 */


#ifndef __UTILS_H__
#define __UTILS_H__

/// @brief The result of a question. Only the field matching the question type is valid
typedef union
{
  int int_value;
  float float_value;
  char *string_value;
  char char_value;
} answer_t;

/// @brief A check of a line of input
/// @return true if the input is acceptable
typedef bool check_func(char *);

/// @brief A conversion of an accepted line of input into an answer_t
typedef answer_t convert_func(char *);

extern char *strdup(const char *);

/// @brief Convers a stirng to an answer_t by duplicating it. 
/// @return an answer_t where .string_value points to the string. Allocates memory by strdup and caller must free.  
answer_t to_string(char *str);


// ---------- I N P U T ----------

/// @brief Reads one line from stdin into buf, without the newline.
///        Always null-terminates. If the line is longer than buf_siz - 1
///        characters the rest of the line is read and thrown away.
/// @param buf the buffer to fill
/// @param buf_siz size of buf in bytes, must be >= 1
/// @return the number of characters stored in buf (excluding the null terminator)
int read_string(char *buf, int buf_siz);

/// @brief Checks if a string is an integer: optional leading '-' followed by
///        at least one digit and nothing else. Does not check for overflow.
/// @return true if str is an integer, false otherwise (also for "" and "-")
bool is_number(char *str);

/// @brief Checks that a string is not empty
/// @return true if str has at least one character
bool not_empty(char *str);

/// @brief Asks a question and reads lines until check accepts one.
///        The question is printed before every attempt. 
///        Lines are limited to 254 characters; longer input is cut.
///        If input ends before an acceptable line was read, an error message is
///        printed to stderr and the program exits, this will leak memory under valgrind. 
/// @param question the text to print
/// @param check decides if a line is acceptable
/// @param convert turns the accepted line into the returned answer
/// @return the converted answer
answer_t ask_question(char *question, check_func *check, convert_func *convert);

/// @brief Asks for an integer, repeating until is_number accepts the input
/// @param question the text to print
/// @return the integer
int ask_question_int(char *question);

/// @brief Asks for a non-empty string
/// @param question the text to print
/// @return a newly allocated string (strdup). The caller must free it.
char *ask_question_string(char *question);

/// @brief Asks a question and returns the first character of the accepted line.
///        The rest of the line is ignored.
/// @param question the text to print
/// @param check decides if a line is acceptable (it receives the whole line)
/// @return the first character of the line
/// @pre check never accepts an empty line (otherwise the result is '\0')
char ask_question_char(char *question, check_func *check);


// --------- O U T P U T ----------


/// @brief Prints a string to stdout, without a trailing newline
void print(char *str);

/// @brief Prints a string to stdout followed by a newline
void println(char *str);


// ---------- H A S H  &. E Q A L I T Y ----------


/// @brief Hash funcqtion for string keys ( uses elem_t.s) depends 
///        only on the characters of the string, not on its adress
/// @pre key.s is not NULL
size_t string_hash(elem_t key);

/// @brief Equality for string keys (uses elem_t.s), compares the characters
/// @pre a.s and b.s are not NULL
/// @return true if the strings are equal
bool string_eq(elem_t a, elem_t b);

/// @brief Hash function for int keys (uses elem_t.i)
size_t int_hash(elem_t key);

/// @brief Equality for int keys (uses elem_t.i)
/// @return true if the ints are equal
bool int_eq(elem_t a, elem_t b);

#endif