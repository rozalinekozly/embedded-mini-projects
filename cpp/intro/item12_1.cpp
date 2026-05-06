#include <iostream>

void foo()
{
    std::cout << "foo" << std::endl;
}
void foo(int i)
{
    std::cout << "foo int: " << i << std::endl;
}

void foo(char d)
{
    std::cout << "foo char: " << d << std::endl;
}

int main()
{
    unsigned char c = 255;
    foo();
    foo(5);
    foo('a');
    //foo(5.5);
    //foo(nullptr);
    foo(c);

    return 0;
}