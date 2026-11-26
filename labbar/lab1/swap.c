int main(void)
{
    int x = 1;
    int y = 2;
    printf("x = %d\n", x);
    printf("y = %d\n", y);
    puts("=====");

    int a = x;
    x = y;
    y = a;

    printf("x = %d\n", x);
    printf("y = %d\n", y);

    return 0;
} 