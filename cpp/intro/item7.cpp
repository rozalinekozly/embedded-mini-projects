#include <iostream>

int main() {
    // mixing malloc with delete
    int *ptr = (int*)malloc(1000 * sizeof(int));
    delete ptr;

    // mixing new with free
    int *ptr2 =  new int[1000];
    int* ptr = new
        //free(ptr2);

    return 0;
}