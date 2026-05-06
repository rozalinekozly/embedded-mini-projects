#include <iostream>
enum E {AA, BB, CC, DD};
enum E2 {TT, KK};
enum E e = E(2);
enum E2 e2 = TT;
E2 e22 = KK;
//++e;
enum E k;

int main()
{
    enum E b;
    std::cout << "b: " << b << std::endl;
    std::cout << "k: " << k << std::endl;
     return 0;
}

