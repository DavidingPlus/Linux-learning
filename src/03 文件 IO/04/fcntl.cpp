#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

void set_flag(const int& fd, const int& add_flag) {
    int flag = fcntl(fd, F_GETFL);
    flag |= add_flag;
    fcntl(fd, F_SETFL, flag);
}

void del_flag(const int& fd, const int& add_flag) {
    int flag = fcntl(fd, F_GETFL);
    flag &= ~add_flag;
    fcntl(fd, F_SETFL, flag);
}

int main() {
    // 如果不存在则创建
    if (0 != access("04.txt", F_OK)) {
        int fd1 = open("04.txt", O_RDWR | O_CREAT, 0664);
        if (-1 == fd1) {
            perror("open");
            return -1;
        }

        close(fd1);
    }

    // 打开文件
    int fd = open("04.txt", O_RDWR);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    // 查看文件状态标志
    int flag = fcntl(fd, F_GETFL);
    // 先看必有的三个状态，只读，只写，读写
    if (flag & O_RDONLY)
        cout << "read only" << endl;
    else if (flag & O_WRONLY)
        cout << "write only" << endl;
    else if (flag & O_RDWR)
        cout << "read write" << endl;
    else
        cout << "unknown access mode" << endl;

    // 可以选择再看一些其他的状态标志，这里我就看O_NONBLOCK
    if (flag & O_NONBLOCK)
        cout << "nonblock" << endl;
    else
        cout << "not nonblock" << endl;

    // 设置fd为非阻塞状态标志
    set_flag(fd, O_NONBLOCK);

    // 然后我再看一次
    flag = fcntl(fd, F_GETFL);
    if (flag & O_NONBLOCK)
        cout << "nonblock" << endl;
    else
        cout << "not nonblock" << endl;

    // 把标志删了
    del_flag(fd, O_NONBLOCK);

    // 然后我再看一次
    flag = fcntl(fd, F_GETFL);
    if (flag & O_NONBLOCK)
        cout << "nonblock" << endl;
    else
        cout << "not nonblock" << endl;

    return 0;
}
