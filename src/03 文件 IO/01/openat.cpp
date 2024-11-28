#include <cstring>
#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF1FER_SIZE 1024

int main() {
    // 通过open函数打开项目中的01目录
    int dir_fd = open("./01", O_DIRECTORY);
    if (-1 == dir_fd) {
        perror("open");
        return -1;
    }

    // 通过openat函数使用相对路径打开其他目录(01)的文件
    int fd1 = openat(dir_fd, "01.txt", O_RDONLY);
    if (-1 == fd1) {
        perror("openat");
        return -1;
    }

    char buf1[MAX_BUF1FER_SIZE] = {0};
    int len = read(fd1, buf1, sizeof(buf1) - 1);
    if (-1 == len) {
        perror("read");
        return -1;
    }
    printf("fd1 read : %s", buf1);

    // 通过openat函数使用相对路径打开当前目录的文件
    int fd2 = openat(AT_FDCWD, "01.txt", O_RDONLY);
    if (-1 == fd1) {
        perror("openat");
        return -1;
    }

    char buf2[MAX_BUF1FER_SIZE] = {0};
    len = read(fd2, buf2, sizeof(buf2) - 1);
    if (-1 == len) {
        perror("read");
        return -1;
    }
    printf("fd2 read : %s", buf2);

    close(dir_fd);
    close(fd1);
    close(fd2);

    return 0;
}
