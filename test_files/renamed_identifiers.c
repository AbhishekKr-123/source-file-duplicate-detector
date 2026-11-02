#include <stdio.h>

int sum(int x, int y)
{
    int result = x + y;
    printf("%d", result);
    return 0;
}

int main()
{
    int p = 10;
    int q = 20;
    sum(p, q);
    return 0;
}
