#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../generic_structures/common.h"

#ifndef __UTILS_H__
#define __UTILS_H__



extern char *strdup(const char *);

typedef union {
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool check_func(char *);
typedef answer_t convert_func(char *);

void clear_input_buffer(void);
int read_string(char *buf, int buf_size);

bool is_number(char *str);
bool is_float(char *str);
bool not_empty(char *str);

answer_t make_float(char *str);

answer_t ask_question(char *question, check_func *check, convert_func *convert);
char *ask_question_string(char *question);
int ask_question_int(char *question);
double ask_question_float(char *question);

int string_lenght(char *argv);
void print(char *str);
void println(char *str);

///@brief Hash function for string keys, uses elem_t
size_t string_hash(elem_t key);

///@brief Equality function for string keys, uses elem_t
bool string_eq(elem_t a, elem_t b);

///@brief Hash funcion for int keys, uses elem_t
size_t int_hash(elem_t key);

///@brief Equality function fro int keys, uses elem_t
bool int_eq(elem_t a, elem_t b);


#endif 