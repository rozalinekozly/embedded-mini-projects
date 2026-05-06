#include <stdio.h>
enum E 
{
    AA = 2,
    BB,
    CC,
    DD
};
enum E myEnum = 2000;

void foo(enum E e)
{
    printf("yay got %d\n", e);
    printf("my enum is %d\n", myEnum);

}

int main()
{
    /* printf("E: AA =  %d , BB = %d , CC = %d , DD = %d\n", AA, BB, CC, DD);*/
    foo(7);
    return 0;
}
