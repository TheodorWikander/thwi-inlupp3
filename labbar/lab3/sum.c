#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef int int_fold_func(int, int);

int add(int a, int b) {
    return a + b;

}

int_fold_func *name = add;

int foldl_int_int(int numbers[], int numbers_size, int_fold_func *f) {
    int result = 0;

    for (int i = 0; i < numbers_size; ++i) {
        result = f(result, numbers[i]);
    }

    return result;
}

long sum (int numbers[], int numbers_size) {
    int result = 0;
    result = foldl_int_int(numbers, numbers_size, add);
    return result;
}

int main (void) {
    int my_numbers[] = {1, 2, 3, 4, 5};
    int size = 5;

    long total = sum(my_numbers, size);

    printf("Arrayens element: ");
    for (int i = 0; i < size; ++i) {
        printf("%d ", my_numbers[i]);
    }
    printf("\n");

    printf("Summan av alla element är: %ld\n", total);

    if (total == 15) {
        printf("Testet lyckades! Bingo!\n");
    } else {
        printf("Testet misslyckades. Fick %ld, förväntade 15 \n", total);
    }
    return 0;
}