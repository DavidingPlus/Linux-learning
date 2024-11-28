#include <iostream>
#include <string>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
    #include <unistd.h>
    int dup(int oldfd);
作用：复制一个新的文件描述符，他们是指向同一个文件的，只是用了两个文件描述符
     新的文件描述符会使用空闲的文件描述符表中最小的那个!!!

*/

int main() {
    int fd = open("1.txt", O_RDWR | O_CREAT, 0664);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    int fd1 = dup(fd);
    if (fd1 == -1) {
        perror("dup");
        return -1;
    }

    printf("fd : %d , fd1 : %d\n", fd, fd1);

    // 关闭fd，现在只有fd1指向文件
    close(fd);

    // 通过fd1来写该文件
    string fuck = "hello world";
    int ret = write(fd1, fuck.c_str(), fuck.size());
    if (ret == -1) {
        perror("write");
        return -1;
    }

    return 0;
}
