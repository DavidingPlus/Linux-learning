#include <cstring>
#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

int main() {
    // 打开一个已经存在的文件，带或者不带O_APPEND，然后看是从哪里开始写的
    // int fd = open("03.txt", O_RDWR);
    int fd = open("03.txt", O_RDWR | O_APPEND);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    // 开始写入
    const char* buf = "my name is your father.\n";
    write(fd, buf, strlen(buf));
    close(fd);

    return 0;
}
