#include <stdio.h>

void clear_scanf_buffer()
{
    scanf("%*[^\n]");
    scanf("%*c");
}
