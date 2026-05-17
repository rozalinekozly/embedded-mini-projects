/*------------------------------------------------------------------------------
Description: single-linked list generic class tests
Developer: rozaline
Reviewer: steve
Version: 1
------------------------------------------------------------------------------*/
#include <iostream>  //cout
#include <cassert>   //assert

#include "slist_generic.hpp"
/*--------------------------------------------------------------------------------------------*/
//forward declaration 
void TestInt();
void TestDouble();
void TestChar();
/*--------------------------------------------------------------------------------------------*/
int main()
{
    TestInt();
    TestDouble();
    TestChar();

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
/*--------------------------------------------------------------------------------------------*/
void TestInt()
{
    std::cout << "TestInt: ";
    hrd42::Slist<int> lst;

    assert(lst.IsEmpty());
    assert(0 == lst.Size());

    lst.PushFront(1);
    lst.PushFront(2);
    lst.PushFront(3);

    assert(!lst.IsEmpty());
    assert(3 == lst.Size());

    assert(3 == lst.PopFront());
    assert(2 == lst.PopFront());
    assert(1 == lst.PopFront());

    assert(lst.IsEmpty());

    std::cout << "PASS" << std::endl;
}
/*--------------------------------------------------------------------------------------------*/
void TestDouble()
{
    std::cout << "TestDouble: ";
     hrd42::Slist<double> lst;

    assert(lst.IsEmpty());

    lst.PushFront(1.1);
    lst.PushFront(2.2);
    lst.PushFront(3.3);

    assert(3 == lst.Size());
    assert(3.3 == lst.PopFront());
    assert(2.2 == lst.PopFront());
    assert(1.1 == lst.PopFront());

    assert(lst.IsEmpty());

    std::cout << "PASS" << std::endl;
}
/*--------------------------------------------------------------------------------------------*/
void TestChar()
{
    std::cout << "TestChar: ";
     hrd42::Slist<char> lst;

    assert(lst.IsEmpty());

    lst.PushFront('a');
    lst.PushFront('b');
    lst.PushFront('c');

    assert(3 == lst.Size());
    assert('c' == lst.PopFront());
    assert('b' == lst.PopFront());
    assert('a' == lst.PopFront());

    assert(lst.IsEmpty());

    std::cout << "PASS" << std::endl;
}
/*--------------------------------------------------------------------------------------------*/