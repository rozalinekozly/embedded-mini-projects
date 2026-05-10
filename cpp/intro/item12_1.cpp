#include <iostream>
//null is not nullptr even though its 0 but the language do no want it 
void foo()
{
    std::cout << "foo" << std::endl;
}
/*
void foo(int i)
{
    std::cout << "foo int: " << i << std::endl;
}*/

void foo(char d)
{
    std::cout << "foo char: " << d << std::endl;
}

void foo(unsigned char d)
{
    std::cout << "foo unsigned char: " << d << std::endl;
}
void foo(float i)
{
    std::cout << "foo float: " << i << std::endl;
}
void foo(double i)
{
    std::cout << "foo double: " << i << std::endl;
}
void foo(int* i)
{
    std::cout << "foo int*: " << i << std::endl;
}   
int main()
{
    unsigned char c = 255;
    int* ptr = 0;
    foo();
    //foo(5);
    foo('a');
    //foo(5.5);
    //foo(nullptr);
    foo(c);

    return 0;
}