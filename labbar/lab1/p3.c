int main(void) {
    char result[11] = "";
    int count = 0;
    for (int i = 1; i <= 10; i = i + 1) {
        strcat(result, "*");
        count = count + strlen(result);
        printf("%s\n", result);
    }
    printf("Totalt: %d\n", count);
    return 0;
}