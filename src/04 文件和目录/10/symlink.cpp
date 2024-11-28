#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("./", O_DIRECTORY);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    // 创建一个指向文件存在的
    int ret = symlinkat("10.txt", fd, "10_1.txt");
    if (-1 == ret) {
        perror("symlink");
        return -1;
    }

    // 创建一个指向文件不存在的
    ret = symlink("11.txt", "10_2.txt");
    if (-1 == ret) {
        perror("symlink");
        return -1;
    }

    return 0;
}
