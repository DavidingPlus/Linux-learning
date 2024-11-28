#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

int main() {
    // 使用truncate函数缩小空间
    int ret = truncate("05.txt", 3);
    if (-1 == ret) {
        perror("truncate");
        return -1;
    }

    // 使用ftruncate函数扩充空间
    int fd = open("05_1.txt", O_RDWR);

    ret = ftruncate(fd, 100);
    if (-1 == ret) {
        perror("ftruncate");
        return -1;
    }

    return 0;
}
