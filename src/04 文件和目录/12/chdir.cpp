#include <cstring>
#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    chdir("./test");

    char buf[MAX_BUFFER_SIZE] = {0};

    int fd = open("12.txt", O_RDWR | O_TRUNC);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    while (1) {
        static int count = 0;

        bzero(buf, sizeof(buf));
        sprintf(buf, "hello, count: %d\n", count++);

        write(fd, buf, strlen(buf));
        sleep(1);
    }

    return 0;
}
