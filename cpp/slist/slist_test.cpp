#include <iostream>

#include "slist.hpp"

int main()
{
    hrd42::Slist slist;
    std::cout << "IsEmpty: " << slist.IsEmpty() << std::endl;
    slist.PushFront(1);
    slist.PushFront(2);
    slist.PushFront(3);
    std::cout << "Size: " << slist.Size() << std::endl;
    std::cout << "IsEmpty: " << slist.IsEmpty() << std::endl;
    std::cout << "PopFront: " << slist.PopFront() << std::endl;
    std::cout << "Size: " << slist.Size() << std::endl;
    return 0;
}

