#include <iostream>
using namespace std;
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#define _size 1024

/*
    #include <sys/mman.h>

    void *mmap(void *addr, size_t length, int prot, int flags,
                int fd, off_t offset);
作用：将一个文件或者设备的数据映射到内存中
参数：
    void *addr：绝大多数情况传nullptr，表示由内核决定映射的内存地址在哪里
    length：要映射的数据的长度，这个值不能为0，建议使用文件的长度
        获取文件的长度：stat lseek
        这个虚拟地址的应映射是以分页为管理的，如果映射的长度不满页的大小，会自动调整为一个分页的大小
    prot：对我们申请的内存映射区的操作权限
        - PROT_EXEC  可执行的权限
        - PROT_READ  读的权限
        - PROT_WRITE 写的权限
        - PROT_NONE  没有权限
        要操作映射区，必须要有读权限，一般给 读权限 或者 读权限和写权限都有 (按位或)
    flags：
        - MAP_SHARED 映射区的数据会自动和磁盘文件进行同步，进程间通信必须设置这个选项
        - MAP_PROVATE 不同步，内存映射区的数据改变了，对原来的文件不会修改，会重新创建一个新的文件(copy on write)
    fd：需要映射的文件的文件描述符
        通过open()得到，open的是一个磁盘文件
        注意，文件的大小不能为0；open指定的权限不能和prot参数的权限有冲突
        比如，   prot：PROT_READ                    open：只读/读写
                prot：PROT_READ | PROT_WRITE       open：读写
    offset：映射时候的偏移量，必须指定的是4K的整数倍，0表示不偏移
返回值：
    成功 返回创建好的映射区的内存首地址
    失败 返回 MAP_FAILED (void *)-1，并且设置errno

    int munmap(void *addr, size_t length);
功能：释放内存映射
参数：
    addr：要释放的内存映射的首地址
    length：要释放的内存大小，要和mmap()的length参数值一样
*/

/*
使用内存映射实现进程之间通信
1.有关系的进程，父子进程
    在没有子进程的时候，通过唯一的父进程先通过一个大小不是0的磁盘文件创建内存映射区，有了之后再创建子进程，然后父子共享这个内存映射区
2.没有关系的进程间通信
    准备一个大小不是0的磁盘文件
    进程1通过磁盘文件得到一个内存映射区，得到一个操作这个内存的指针
    进程2同理，得到一个指针
    使用内存映射区进行通信

注意：内存映射区通信，不会阻塞
*/

int main() {
    // 打开1个文件
    int fd = open("test.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    // 获取大小
    off_t size = lseek(fd, 0, SEEK_END);

    // 创建内存映射区
    void* ptr = mmap(nullptr, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    // 创建子进程
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid > 0) {   // 父进程
        wait(NULL);  // 等待子进程写入数据然后回收完毕再读取

        // 读数据
        char buf[_size] = {0};
        strcpy(buf, (char*)ptr);
        printf("read data : %s\n", buf);
    } else if (pid == 0) {  // 子进程
        // 写数据，注意是直接操作这个指针，和管道不一样，管道是通过文件描述符操作
        // 我写的字符串后面带有 '\0' 结束符，不用担心会和原来文件的数据冲突，因为我是从头开始覆盖，然后走到尾部自动补上 '\0'，读操作也是一样的
        strcpy((char*)ptr, "nihao");
    }

    // 关闭内存映射区
    int ret = munmap(ptr, size);
    if (ret == -1) {
        perror("munmap");
        return -1;
    }

    // 关闭文件
    close(fd);

    return 0;
}
