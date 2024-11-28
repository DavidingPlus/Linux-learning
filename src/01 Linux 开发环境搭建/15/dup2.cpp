#include <iostream>
#include <string>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
    #include <unistd.h>

    int dup2(int oldfd, int newfd);
作用：重定向文件描述符
    oldfd指向a.txt，newfd指向b.txt
    调用函数成功后，oldfd和b.txt的连接做close(oldfd仍存在，只是不指向b.txt了)，newfd指向a.txt
    oldfd必须是一个有效的文件描述符
    如果相同则相当于什么都没做
返回值：
    newfd，他们都指向的是oldfd之前指向的文件

*/

int main() {
    int fd = open("2.txt", O_RDWR | O_CREAT, 0664);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    int fd1 = open("3.txt", O_RDWR | O_CREAT, 0664);
    if (fd1 == -1) {
        perror("open");
        return -1;
    }

    printf("fd : %d , fd1 : %d\n", fd, fd1);

    int fd2 = dup2(fd, fd1);
    // 现在fd本来指向 2.txt ，现在fd不指向2.txt了，相当于连接释放了
    // fd1本来 3.txt，现在指向到了 2.txt
    if (fd2 == -1) {
        perror("dup2");
        return -1;
    }

    // 通过fd1去写数据，实际操作的是1.txt
    string fuck = "hello world";
    int ret = write(fd1, fuck.c_str(), fuck.size());
    if (ret == -1) {
        perror("write");
        return -1;
    }

    printf("fd : %d , fd1 : %d , fd2: %d\n", fd, fd1, fd2);

    return 0;
}
