#include <iostream>
using namespace std;
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#define _size 1024

/*
匿名映射：不需要文件实体进行一个内存映射，只能在父子和有关系的进程之间通信，因为没有办法通过文件进行关联
- 修改flags参数，做匿名映射需要传入 MAP_ANONYMOUS，这样会忽略掉fd参数，一般我们传入-1
- flags参数当中，MAP_SHARED和MAP_PRIVATE参数是必选一个，然后其他的是可选项，MAP_ANONYMOUS就是一个可选项
*/

int main() {
    // 创建匿名内存映射区
    int length = 4096;
    void* ptr = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    // 父子进程间通信
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid > 0) {  // 父进程
        strcpy((char*)ptr, "helloworld");
        wait(nullptr);
    } else if (pid == 0) {  // 子进程
        sleep(1);           // 非阻塞，先睡一秒让父进程执行完写入操作
        printf("%s\n", (char*)ptr);
    }

    // 关闭内存映射区
    int ret = munmap(ptr, length);
    if (ret == -1) {
        perror("munmap");
        return -1;
    }

    return 0;
}
