#include <iostream> 
#include <string.h>

#include "rcstring.hpp"

using namespace hrd42;
using namespace std;

void TestCtor();
void TestCctor();
void TestCopyAssignmentOperator();
void TestEqualAndNonEqualOperator();
void TestConcatOperation();
void TestLength();

// helper function to compare between 2 strings, prints to terminal the results
static void CmpToExpectedString(RCString test_res, RCString expected);

int main()
{
    TestCtor();
    TestCctor();
    TestCopyAssignmentOperator();
    TestEqualAndNonEqualOperator();
    TestConcatOperation();
    TestLength();

    return 0;
}

void TestEqualAndNonEqualOperator()
{
    cout<<"Testing operators == and !="<<endl;
    {
        cout<<"\t diffreniate with 1 letter: ";
        RCString s1 = "hello";
        RCString s2 = "hella";
        
        if(!(s1 == s2) && (s1 != s2))
        {
            cout<<"Passed"<<endl;
        }
        else
        {
            cout<<"Failed"<<endl;
        }
        
    }
}

void TestCtor()
{
    //note: later change this to array of strings and expected, and send to test in loop
    cout << "--- Testing Ctor ---" << endl;
    
    cout << "\tdefault argument (empty string) test: ";
    RCString x1;
    const char* expected1 = "";
    CmpToExpectedString(x1, expected1);

    cout << "\tArgument test: ";

    RCString x2("hello world");
    const char* expected2 = "hello world"; 
    CmpToExpectedString(x2, expected2);
}

void TestCctor()
{
    cout<<"--- Testing cctor ---"<<endl;

    cout<<"\targument defined ctor: ";
    {
        RCString s1 = "Rozaline";
        RCString s2(s1);

        CmpToExpectedString(s1,s2);
    }
}

void TestCopyAssignmentOperator()
{
    cout<<"--- Testing copy assignment operator ---"<<endl;
    cout<<"\t2 argumented strings : ";
    {
        RCString s1 = "Rozaline";
        RCString s2 = "Kozly";

        s1 = s2;

        CmpToExpectedString(s1, s2);
    }
}

static void CmpToExpectedString(RCString test_res, RCString expected)
{
    if(test_res.operator==(expected))
    {
        cout<<"Passed"<<endl;
    }
    else
    {
        cout<<"Failed"<<endl;
    }
}

void TestConcatOperation()
{
    cout << "--- Testing Concat operation ---" << endl;
    RCString s1 = "hello";
    RCString s2 = "world";

    cout << "\t operator +" << endl;
    cout << "\t" << s1 << " + " << s2<< " is " << s1+s2 << endl;

    cout <<"\t operator +="<<endl;
    cout << "\t" << s1 << " += " << s2<< " is " ;
    s1 += s2;
    cout<< s1 << endl;
}

void TestLength()
{
    RCString str = "string with length 21";

    // compare implementation result to the expected C-string literal length
    cout<<"library result is "<< strlen("string with length 21")<<" my implementation result is "<< str.Length() <<endl;

}