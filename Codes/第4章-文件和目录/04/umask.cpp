#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

// 定义一个权限宏
#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main() {
    // 先将umask置为0
    umask(0);

    // 先正常创建一个文件，这里我用creat函数
    if (0 != access("foo.txt", F_OK))
        unlink("foo.txt");

    int fd1 = creat("foo.txt", RWRWRW);
    if (-1 == fd1) {
        perror("creat");
        return -1;
    }

    // 然后调用umask创建屏蔽字
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (0 != access("bar.txt", F_OK))
        remove("bar.txt");

    int fd2 = creat("bar.txt", RWRWRW);
    if (-1 == fd2) {
        perror("creat");
        return -1;
    }

    return 0;
}
