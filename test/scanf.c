#include <stdio.h>

int main(int argc, char const *argv[])
{
    char c;
    int n;
    printf("please input a number: ");
    scanf("%d", &n);
    scanf("%*[^\n]");
    scanf("%*c");
    printf("please input a char: ");
    scanf("%c", &c);
    printf("your number: %d, your char: %c\n", n, c);
    return 0;
}
