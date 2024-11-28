#include <iostream>
using namespace std;
#include <sys/types.h>
#include <unistd.h>

/*
    #include <sys/types.h>
    #include <unistd.h>

    int truncate(const char *path, off_t length);
作用：缩减或者扩展文件尺寸到达指定的大小
参数:
    path：文件路径
    length：需要最终文件变成的大小
返回值：
    成功 0
    失败 -1

*/

int main() {
    int ret = truncate("b.txt", 5);
    if (ret == -1) {
        perror("truncate");
        return -1;
    }

    return 0;
}
