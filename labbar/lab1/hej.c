#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int ret = strlen(argv[1]);
    printf("%d\n", ret);
    return 0;
}