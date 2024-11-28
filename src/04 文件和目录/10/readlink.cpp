#include <iostream>
using namespace std;
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    char buf[MAX_BUFFER_SIZE] = {0};

    int len = readlink("10_1.txt", buf, sizeof(buf) - 1);
    if (-1 == len) {
        perror("readlink");
        return -1;
    }
    printf("%s\n", buf);

    return 0;
}
