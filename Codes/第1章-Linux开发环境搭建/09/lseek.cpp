#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
----标准C库的函数
    #include <stdio.h>

    int fseek(FILE *stream, long offset, int whence);

----Linux系统函数
    #include <sys/types.h>
    #include <unistd.h>

    off_t lseek(int fd, off_t offset, int whence);
参数：
    fd：文件描述符，通过open得到，通过这个fd操作某个文件
    offset：off_t(long别名) 偏移量
    whence：
        - SEEK_SET
            设置文件指针的偏移量，从头开始
        - SEEK_CUR
            设置偏移量：当前位置 + 第二参数offset的值
        - SEEK_END
            设置偏移量：文件大小 + 第二参数offset的值

返回值：
    返回文件指针设置之后的位置

作用：
    1.移动文件指针到头部 lseek(fd,0,SEEK_SET);
    2.获取当前文件指针的位置 lseek(fd,0,SEEK_CUR);
    3.获取文件长度 lseek(fd,0,SEEK_END);
    4.拓展文件的长度，当前文件10B，增加100B，增加了100个字节 lseek(fd,100,SEEK_END);
        为什么能扩展？
        因为我们把文件指针移动到了本来不属于文件的位置上去，系统就进行了扩展，
        不过一定要一次写的操作。迅雷等下载工具在下载文件时候先扩展一个空间，然后再下载的。
*/

int main() {
    int fd = open("hello.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    int ret = lseek(fd, 100, SEEK_END);
    if (ret == -1) {
        perror("lseek");
        return -1;
    }

    // 写入一个空数据
    write(fd, " ", 1);

    close(fd);

    return 0;
}
