#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../utils.h"

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main(void)
{
  int x = 7;
  int y = 42;
  swap(&x, &y);
  printf("%d, %d\n", x, y);
  return 0;
}