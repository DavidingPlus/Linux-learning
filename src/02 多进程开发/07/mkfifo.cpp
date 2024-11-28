#include <iostream>
using namespace std;
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
创建有名管道 fifo文件
- 通过命令 mkfifo 名字

- 通过函数
    #include <sys/stat.h>
    #include <sys/types.h>

    int mkfifo(const char *pathname, mode_t mode);
参数：
    pathname：管道名称的路径
    mode：文件的权限 和 open 的 mode 一样，八进制数
返回值：
    成功 返回0
    失败 返回-1，并设施errno
*/

int main() {
    // 判断文件是否存在
    int ret = access("/home/lzx0626/fuck/fifo", F_OK);
    if (ret == -1) {
        printf("管道不存在，创建管道\n");

        ret = mkfifo("/home/lzx0626/fuck/fifo", 0664);
        if (ret == -1) {
            perror("mkfifo");
            return -1;
        }
    }

    return 0;
}
