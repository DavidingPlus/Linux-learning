#include <iostream>
using namespace std;
#include <unistd.h>

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);

    puts("This is a test of buffered output.");  // puts最后会自带一个换行符

    sleep(3);

    fflush(stdout);  // 刷新

    puts("This is a test of buffered output.");

    sleep(3);

    return 0;
}
