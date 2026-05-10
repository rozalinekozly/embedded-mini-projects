#include <iostream>
const int i = 3;

int main()
{
    int* ip = (int*)&i;
    *ip = 5;
    int arr[i];

    printf("%d %d\n",i, *ip);
    printf("*(&i) is %d \n", *(&i));
    return 0;
}