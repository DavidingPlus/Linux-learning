#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>

/*
    #include <unistd.h>
    int chdir(const char *path);
作用：修改进程的工作目录
    比如在 /home/newcoder 启动了一个可执行程序 a.out，进程的工作目录 /home/newcoder
参数：
    path：需要修改到的工作目录


    #include <unistd.h>
    char *getcwd(char *buf, size_t size);
作用：获取当前工作目录
参数：
    buf：往数组里存，传出参数
    size：数组的大小
返回值：返回的是指向的一块内存，这个数据就是第一个参数buf

*/

int main() {
    // 获取当前工作目录
    char buf[1024] = {0};

    getcwd(buf, sizeof(buf));
    cout << buf << endl;

    // 修改工作目录
    int ret = chdir("./fuckyou");
    if (ret == -1) {
        perror("chdir");
        return -1;
    }

    // 代码执行到这里把工作目录修改之后在该目录之下进行下一步操作
    // 只在程序运行的时候生效，程序结束后终端当中的工作目录不会改变!

    // 创建新的文件
    int fd = open("1.txt", O_RDWR | O_CREAT, 0664);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    close(fd);

    char Null_buf[1024] = {0};
    strncpy(buf, Null_buf, sizeof(buf));

    getcwd(buf, sizeof(buf));
    cout << buf << endl;

    return 0;
}
