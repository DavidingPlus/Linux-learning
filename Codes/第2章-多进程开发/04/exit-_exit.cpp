#include <iostream>
using namespace std;
#include <unistd.h>

/*
    #include <stdlib.h>
    void exit(int status);

    #include <unistd.h>
    void _exit(int status);

参数：status：进程退出的时候的一个状态信息，父进程回收子进程资源的时候可以获得他的信息

*/

int main() {
    cout << "hello\n";
    cout << "world";

    exit(0);
    // _exit(0);

    return 0;
}
