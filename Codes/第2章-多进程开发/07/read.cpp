#include <cstring>
#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>
#define _size 1024

// 向管道中读数据
int main() {
    // 打开管道文件，以只读的方式
    int fd = open("/home/lzx0626/fuck/fifo", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    // 读取数据
    char buf[_size] = {0};
    while (1) {
        int len = read(fd, buf, sizeof(buf));
        if (len == 0) {
            printf("写端断开连接了...\n");
            break;
        }
        printf("recv buf : %s\n", buf);
        bzero(buf, sizeof(buf));
    }

    close(fd);

    return 0;
}
