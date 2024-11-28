#include <iostream>
using namespace std;
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#define _size 1024

int main() {
    // 打开文件
    int fd = open("test.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    // 获取文件大小
    struct stat statbuf;
    int ret = stat("test.txt", &statbuf);
    if (ret == -1) {
        perror("stat");
        return -1;
    }
    off_t size = statbuf.st_size;

    // 创建内存映射区
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    // 操作这块内存
    char buf[_size] = {0};
    fgets(buf, sizeof(buf) - 1, stdin);  // 保证后面留有一个'\0'符号
    // 写数据
    strcpy((char*)ptr, buf);

    // 关闭内存映射区
    munmap(ptr, size);
    // 关闭文件
    close(fd);

    return 0;
}
