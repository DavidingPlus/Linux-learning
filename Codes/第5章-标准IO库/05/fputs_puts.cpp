#include <iostream>
using namespace std;
#include <unistd.h>

int main() {
    const char* str = "helloworld";

    fputs(str, stdout);  // 不带换行符

    sleep(3);

    puts(str);  // 自带换行符

    return 0;
}
