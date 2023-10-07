#include <cstring>
#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define _size 1024

int main() {
    // 父进程写，子进程读
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid > 0) {
        // 负责写，绑定管道fifo2
        int ret = access("/home/lzx0626/FIFO/fifo2", F_OK);
        if (ret == -1) {
            // 不存在则创建
            printf("管道文件fifo2不存在,正在创建...\n");
            ret = mkfifo("/home/lzx0626/FIFO/fifo2", 0664);
            if (ret == -1) {
                perror("mkfifo");
                return -1;
            }
        }

        // 打开管道
        int fdw = open("/home/lzx0626/FIFO/fifo2", O_WRONLY);
        if (fdw == -1) {
            perror("open");
            return -1;
        }
        printf("父进程管道已打开,等待写入...\n");

        // 写数据
        char buf[_size] = {0};
        while (1) {
            bzero(buf, sizeof(buf));
            fgets(buf, sizeof(buf) - 1, stdin);
            printf("send : %s", buf);
            int len = write(fdw, buf, sizeof(buf) - 1);
            if (len == -1) {
                perror("write");
                return -1;
            }
        }
        // 关闭
        close(fdw);
        // 父进程回收子进程
        wait(NULL);
    } else if (pid == 0) {
        // 负责读，绑定管道fifo2
        int ret = access("/home/lzx0626/FIFO/fifo1", F_OK);
        if (ret == -1) {
            // 不存在则创建
            printf("管道文件fifo1不存在,正在创建...\n");
            ret = mkfifo("/home/lzx0626/FIFO/fifo1", 0664);
            if (ret == -1) {
                perror("mkfifo");
                return -1;
            }
        }

        // 打开管道
        int fdr = open("/home/lzx0626/FIFO/fifo1", O_RDONLY);
        if (fdr == -1) {
            perror("open");
            return -1;
        }
        printf("子进程管道已打开,等待读取...\n");

        // 读数据
        char buf[_size] = {0};
        while (1) {
            bzero(buf, sizeof(buf));
            int len = read(fdr, buf, sizeof(buf));
            if (len == -1) {
                perror("read");
                return -1;
            }
            if (len == 0)  // 读端全部关闭，相当于读到文件末尾
                break;
            printf("recv : %s", buf);
        }
        // 关闭
        close(fdr);
    }

    return 0;
}
