#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE 1024

/*
    #include <unistd.h>

    ssize_t read(int fd, void *buf, size_t count);
参数：
    fd：文件描述符，通过open得到的，通过文件描述符操作某个文件
    buf：缓冲区，需要读取数据存放的地方，数组的地方(传出参数)
    count：指定的数组的大小
返回值：
    成功 >0 返回实际读取到的字节数
        ==0 文件已经读取完了
    失败 -1 并且修改errno


    ssize_t write(int fd, const void *buf, size_t count);
参数：
    fd：文件描述符，通过open得到，通过文件描述符操作某个文件
    buf：要往磁盘写入的数据
    count：要写入的实际的大小
返回值：
    成功 >0 返回实际写入的字节数
        ==0 文件已经写入完了
    失败 -1 并且修改errno
*/

int main() {
    // 通过open打开english.txt文件
    int fd_src = open("english.txt", O_RDONLY);
    if (fd_src == -1) {
        perror("open");
        return -1;
    }

    // 创建新的文件
    int fd_dest = open("cpy.txt", O_WRONLY | O_CREAT | O_APPEND, 0777);
    if (fd_dest == -1) {
        perror("create");
        return -1;
    }

    // 频繁的读写操作
    char buf[SIZE] = {0};
    int len = 0;
    while ((len = read(fd_src, buf, sizeof(buf))) > 0)
        // 在循环的条件中进行读操作，在循环体中进行写入
        write(fd_dest, buf, len);

    // 关闭文件
    close(fd_src);
    close(fd_dest);

    return 0;
}
