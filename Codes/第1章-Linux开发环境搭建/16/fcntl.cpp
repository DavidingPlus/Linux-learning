#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
    #include <fcntl.h>
    #include <unistd.h>

    int fcntl(int fd, int cmd, ...); ...当中是可变参数
参数：
    fd：需要操作的文件描述符
    cmd：表示对文件描述符进行如何操作
        F_DUPFD 复制文件描述符，复制的是第一个参数，得到一个新的文件描述符(返回值)
            int ret = fcntl(fd,F_DUPFD);
        F_GETFL 获取指定文件描述符的文件状态flag
            获取的flag和我们通过open函数传递的flag是一个东西
        F_SETFL 设置文件描述符的文件状态flag
            必选项：O_RDONLY O_WRONLY O_RDWR 不可以被修改
            可选项：O_APPEND O_NONBLOCK
                O_APPEND 表示追加数据
                O_NONBLOCK 设置成非阻塞
                    阻塞和非阻塞：描述的是函数调用的行为
*/

int main() {
    // 1.复制文件描述符
    // int fd = open("1.txt", O_RDONLY);
    // if (fd == -1) {
    //     perror("open");
    //     return -1;
    // }

    // int ret = fcntl(fd, F_DUPFD);
    // if (ret == -1) {
    //     perror("fcntl");
    //     return -1;
    // }

    // 2.修改或者获取文件描述符的文件状态flag
    // 这里必须读写权限都要有才行
    int fd = open("1.txt", O_RDWR, 0664);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    // 修改文件描述符的flag，加入O_APPEND这个标记
    // 首先获得
    int _flag = fcntl(fd, F_GETFL);
    if (_flag == -1) {
        perror("fcntl");
        return -1;
    }

    // 然后修改
    int ret = fcntl(fd, F_SETFL, _flag | O_APPEND);
    if (ret == -1) {
        perror("fcntl");
        return -1;
    }

    // 然后进行追加
    string fuck = "你好";
    ret = write(fd, fuck.c_str(), fuck.size());
    if (ret == -1) {
        perror("write");
        return -1;
    }

    close(fd);

    return 0;
}
