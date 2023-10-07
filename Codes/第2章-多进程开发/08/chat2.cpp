#include <cstring>
#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define _size 1024

int main() {
    // 判断有名管道文件1 2是否存在
    int ret = access("/home/lzx0626/FIFO/fifo1", F_OK);
    if (ret == -1) {
        printf("管道1不存在,创建相关的管道文件\n");
        ret = mkfifo("/home/lzx0626/FIFO/fifo1", 0664);
        if (ret == -1) {
            perror("mkfifo");
            return -1;
        }
    }

    ret = access("/home/lzx0626/FIFO/fifo2", F_OK);
    if (ret == -1) {
        printf("管道2不存在,创建相关的管道文件\n");
        ret = mkfifo("/home/lzx0626/FIFO/fifo2", 0664);
        if (ret == -1) {
            perror("mkfifo");
            return -1;
        }
    }

    // 以只读的方式打开管道1
    int fdr = open("/home/lzx0626/FIFO/fifo1", O_RDONLY);
    if (fdr == -1) {
        perror("open");
        return -1;
    }
    printf("打开管道fifo1成功,等待读取...\n");

    // 以只写的方式打开管道2
    int fdw = open("/home/lzx0626/FIFO/fifo2", O_WRONLY);
    if (fdw == -1) {
        perror("open");
        return -1;
    }
    printf("打开管道fifo2成功,等待写入...\n");

    char buf[_size] = {0};
    // 循环的写读数据
    while (1) {
        // 读数据
        bzero(buf, sizeof(buf));
        int len = read(fdr, buf, sizeof(buf));
        if (len == -1) {
            perror("read");
            return -1;
        }
        if (len == 0)
            break;
        printf("buf : %s", buf);

        // 写数据
        bzero(buf, sizeof(buf));
        // 获取标准输入的数据
        fgets(buf, sizeof(buf) - 1, stdin);
        // 写数据
        int ret = write(fdw, buf, sizeof(buf) - 1);
        if (ret == -1) {
            perror("write");
            return -1;
        }
    }

    // 关闭
    close(fdw);
    close(fdr);

    return 0;
}
