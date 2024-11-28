#include <iostream>
using namespace std;
#include <unistd.h>

/*
    #include <unistd.h>

    long fpathconf(int fd, int name);
// 作用：获取管道的大小
// 参数：fd 管道的文件描述符(两个填一个即可)
//     name：宏值
//         获取大小使用 _PC_PIPE_BUF
*/

int main() {
    int pipefd[2];

    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        return -1;
    }

    // 获取管道大小
    long size = fpathconf(pipefd[0], _PC_PIPE_BUF);
    printf("pipe size : %ld\n", size);  // 4096

    return 0;
}
