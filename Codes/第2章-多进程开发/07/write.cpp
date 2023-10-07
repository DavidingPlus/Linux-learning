#include <cstring>
#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define _size 1024

// 向管道中写数据
int main() {
    // 判断管道是否存在，不存在则创建
    int ret = access("/home/lzx0626/fuck/fifo", F_OK);
    if (ret == -1) {
        printf("管道不存在，创建管道\n");

        ret = mkfifo("/home/lzx0626/fuck/fifo", 0664);
        if (ret == -1) {
            perror("mkfifo");
            return -1;
        }
    }

    // 打开管道，以只写的方式
    int fd = open("/home/lzx0626/fuck/fifo", O_WRONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    // 写数据
    for (int i = 0; i < 100; ++i) {
        char buf[_size];
        sprintf(buf, "hello, %d", i);
        printf("write data : %s\n", buf);
        write(fd, buf, strlen(buf));
        sleep(1);
    }

    close(fd);

    return 0;
}
