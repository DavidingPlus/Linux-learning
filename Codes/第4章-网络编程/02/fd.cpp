#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXSIZE 1024

int main() {
    int fd = open("a.txt", O_RDONLY);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    char buf[MAXSIZE] = {0};

    pid_t pid = fork();
    if (-1 == pid) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        read(fd, buf, 1);  // 我给buf中读进去1个字节内容
        printf("buf = %s\n", buf);
    } else if (pid > 0) {
        sleep(3);          // 保证子进程可以先读文件
        read(fd, buf, 2);  // 父进程中读取2个字节

        printf("buf = %s\n", buf);
        wait(nullptr);
    }

    close(fd);
    return 0;
}

// 执行结果：
// buf = h
// buf = el
