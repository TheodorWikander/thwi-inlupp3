#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.h"

void clear_input_buffer(void)
{
  int c;
  do
  {
    c = getchar();
  }
  while (c != '\n' && c != EOF);
}

bool is_number(char *str)
{
  int i = 0;
  if (str[0] == '-')
  {
    i = 1;
  }
  if (str[i] == '\0')
  {
    return false;
  }
  while (str[i] != '\0')
  {
    if (!isdigit(str[i]))
    {
      return false;
    }
    i++;
  }
  return true;
}

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

bool is_shelf(char *str)
{
  if (strlen(str) != 3)
  {
    return false;
  }
  if (str[0] < 'A' || str[0] > 'Z')
  {
    return false;
  }
  return isdigit(str[1]) && isdigit(str[2]);
}

int read_string(char *buf, int buf_siz)
{
  int i = 0;
  int c = getchar();
  while (c != '\n' && c != EOF && i < buf_siz - 1)
  {
    buf[i] = c;
    i++;
    c = getchar();
  }
  buf[i] = '\0';
  if (c != '\n' && c != EOF)
  {
    clear_input_buffer();
  }
  return i;
}

answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
  int buf_siz = 255;
  char buf[buf_siz];
  do
  {
    printf("%s\n", question);
    read_string(buf, buf_siz);
  }
  while (!check(buf));
  return convert(buf);
}

static answer_t to_int(char *str)
{
  return (answer_t) { .int_value = atoi(str) };
}

static answer_t to_string(char *str)
{
  return (answer_t) { .string_value = strdup(str) };
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, to_int);
  return answer.int_value;
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, to_string).string_value;
}

char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, to_string).string_value;
}

static answer_t to_char(char *str)
{
  return (answer_t) { .char_value = str[0] };
}

char ask_question_char(char *question, check_func *check)
{
  return ask_question(question, check, to_char).char_value;
}

void print(char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    putchar(str[i]);
    i++;
  }
}

void println(char *str)
{
  print(str);
  putchar('\n');
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