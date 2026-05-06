#include <iostream>
#include <dlfcn.h>

int main() {
    void *handle1 = dlopen("./libshared.so", RTLD_LAZY);
    void *handle2 = dlopen("./libsharedc.so", RTLD_LAZY);

    void (*func1)() = (void(*)())dlsym(handle1, "sharedFunc");
    void (*func2)() = (void(*)())dlsym(handle2, "fofo");

    func1();
    func2();

    dlclose(handle1);
    dlclose(handle2);
    return 0;
}