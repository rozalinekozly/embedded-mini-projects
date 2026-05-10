#include <iostream>
using namespace std;
int i = 8;
int& ip = i;

int main() {

    cout << sizeof(i) << endl;   // 4
    cout << sizeof(ip) << endl;  // 4
    cout << &i << endl;
    cout << &ip << endl;
}