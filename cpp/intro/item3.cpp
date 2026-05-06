#include <iostream>
enum E {AA = 1, BB=2, CC=3, DD=4};
enum E g_enum;
bool g_bool;
int main()
{
    bool b1 = true;
    bool b2; //trash? 
    int i = b1; //true
    std::cout << "i: " << i << std::endl;
    i = 0;
    b2 = i;
    char c = i;
    std::cout << "g_bool: " << g_bool << std::endl;
    std::cout << "g_enum: " << g_enum << std::endl;
    std::cout << "c: " << c << std::endl;
    printf("%b\n", b2);
    printf("true in language is %d\n", true);
    printf("false in language is %d\n", false);
    std::cout << "Size of bool: " << sizeof(b1) << std::endl;
    return 0;
}