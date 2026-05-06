#include<iostream>

extern "C" void sharedFunc();
void sharedFunc()
{
    std::cout << "shared func" << std::endl;
}