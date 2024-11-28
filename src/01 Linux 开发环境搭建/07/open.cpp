#include <iostream>
using namespace std;

// 使用linux系统IO open()函数的头文件
#include <fcntl.h>  //函数的声明文件
#include <sys/stat.h>
#include <sys/types.h>

// 使用 close()函数头文件
#include <unistd.h>

// 打开一个已经存在的文件
// int open(const char *pathname, int flags);

// pathname：文件路径
// flags：对文件的操作权限设置，还有其他的设置 O_RDONLY O_WRONLY O_RDWR 这三个设置是互斥的
// 返回值：返回一个新的文件描述符，如果失败，返回-1

// errno：属于linux系统函数库，库里面一个全局变量，记录错误号，记录的是最近的错误号
// 比如这里open函数失败了，系统会自动把错误号赋值给errno

// void perror(const char *s);
// 打印error对应的错误描述
// s参数：用户描述，比如hello，最终输出的内容是 hello:xxx(实际的错误描述)

// 创建一个新的文件
// int open(const char *pathname, int flags, mode_t mode);

// 关闭函数
// int close(int fd);
// fd：文件的描述符(类似于编号) fd

int main() {
    // 打开
    int fd = open("1.txt", O_RDONLY);

    if (fd == -1)  // 调用错误
        perror("open");

    // 关闭
    close(fd);

    return 0;
}
