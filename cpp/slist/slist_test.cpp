#include <iostream>

#include "slist.hpp"
/* forward declaration tests */
void TestEmptyList();
void TestInsertion();
void TestPopFront();

int main()
{
    TestEmptyList();
    TestInsertion();
    TestPopFront();
    return 0;
}

void TestEmptyList()
{
    hrd42::Slist slist;
    bool expected_ = true;
    if(slist.IsEmpty() != expected_)
    {
        std::cout << "TestEmptyList failed" << std::endl;
    }
}
void TestInsertion()
{
    hrd42::Slist slist;
    slist.PushFront(1);
    slist.PushFront(2);
    slist.PushFront(3);
    size_t expected_size = 3;
    if(slist.Size() != expected_size)
    {
        std::cout << "TestInsertion failed: size" << std::endl;
    }
    if(slist.IsEmpty())
    {
        std::cout << "TestInsertion failed: is empty" << std::endl;
    }
}

void TestPopFront()
{
    hrd42::Slist slist;
    slist.PushFront(1);
    slist.PushFront(2);
    slist.PushFront(3);
    int expected_val = 3;
    if(slist.PopFront() != expected_val)
    {
        std::cout << "TestPopFront failed: value" << std::endl;
    }
    size_t expected_size = 2;
    if(slist.Size() != expected_size)
    {
        std::cout << "TestPopFront failed: size" << std::endl;      
    }
}