#include <iostream>

void printSum(int a, int b = 10, int c = 20) {
    std::cout << a + b + c << std::endl;
}

int main() {
    printSum(5);        // 5+10+20 = 35
    printSum(5, 3);     // 5+3+20 = 28
    printSum(5, 3, 1);  // 5+3+1 = 9
    return 0;
}