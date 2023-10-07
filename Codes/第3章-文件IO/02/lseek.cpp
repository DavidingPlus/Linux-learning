#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("02.txt", O_RDWR);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    // 将文件扩展
    off_t offset = lseek(fd, 100, SEEK_END);
    if (-1 == offset) {
        perror("lseek");
        return -1;
    }

    // 需要立即进行一次写的操作，否则修改不会成功
    write(fd, " ", 1);

    close(fd);

    return 0;
}
