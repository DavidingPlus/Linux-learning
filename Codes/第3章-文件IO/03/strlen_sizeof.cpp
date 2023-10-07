#include <cstring>
#include <iostream>
using namespace std;

int main() {
    const char* str = "helloworld\n";
    cout << strlen(str) << endl
         << sizeof(str) << endl;

    char buf[1023] = {0};
    cout << strlen(buf) << endl
         << sizeof(buf) << endl;

    return 0;
}
