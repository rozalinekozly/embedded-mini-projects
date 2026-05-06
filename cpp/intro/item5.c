#include <stdio.h>

int main()
{
    float x = boo();
    printf("x: %f\n", x);
    printf("foo(5.5): %d\n", foo(5.5)); 
    return 0;
}

float boo()
{
    printf("boo called\n");
    return 0;
}

int foo(int x)
{
    return x + 1;
}