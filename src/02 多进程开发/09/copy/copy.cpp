#include <cstring>
#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

// 将english.txt文件拷贝一份为cpy.txt，保存在当前目录
int main() {
    // 打开english.txt
    int fd_src = open("english.txt", O_RDONLY);
    if (fd_src == -1) {
        perror("open");
        return -1;
    }

    // 获取english.txt的大小
    off_t size = lseek(fd_src, 0, SEEK_END);

    // 创建内存映射区
    void* ptr_src = mmap(nullptr, size, PROT_READ, MAP_SHARED, fd_src, 0);
    if (ptr_src == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    // 创建cpy.txt
    int ret = access("copy.txt", F_OK);
    if (ret == 0)  // 存在把他删除
        unlink("cpy.txt");
    // 创建
    int fd_dest = open("cpy.txt", O_RDWR | O_CREAT, 0664);
    if (fd_dest == -1) {
        perror("open");
        return -1;
    }
    // 将空文件的大小修改为源文件的大小，防止出现空文件
    ret = truncate("cpy.txt", size);
    // ret = lseek(fd_dest, size, SEEK_END);
    // write(fd_dest, " ", 1);  // lseek扩展文件需要进行一次写的操作，truncate不需要!!!
    if (ret == -1) {
        perror("truncate");
        return -1;
    }

    // 创建内存映射区
    void* ptr_dest = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dest, 0);
    if (ptr_dest == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    // 拷贝
    strncpy((char*)ptr_dest, (char*)ptr_src, size);

    // 关闭内存映射区
    munmap(ptr_src, size);
    munmap(ptr_dest, size);
    // 关闭文件
    close(fd_src);
    close(fd_dest);

    return 0;
}
