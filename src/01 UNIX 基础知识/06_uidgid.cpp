#include <iostream>
using namespace std;
#include <unistd.h>

int main() {
    printf("uid = %d , gid = %d\n", getuid(), getgid());

    return 0;
}
