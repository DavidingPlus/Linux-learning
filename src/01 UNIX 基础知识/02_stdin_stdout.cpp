#include <cstring>
#include <iostream>
using namespace std;
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    char buf[MAX_BUFFER_SIZE] = {0};

    while (1) {
        bzero(buf, sizeof(buf));

        int len = read(STDIN_FILENO, buf, sizeof(buf) - 1);
        if (-1 == len) {
            perror("read");
            return -1;
        }

        if (0 == len)  // 读取到末尾
            break;

        write(STDOUT_FILENO, buf, strlen(buf));
    }

    return 0;
}
