#include <iostream>
using namespace std;
#include <sys/stat.h>
#include <sys/types.h>

/*
    #include <sys/stat.h>
    #include <sys/types.h>

    int mkdir(const char *pathname, mode_t mode);
作用：去创建一个目录
参数：
    pathname：目录名称
    mode：权限，八进制数
返回值：
    成功 0
    失败 -1
*/

int main() {
    int ret = mkdir("fuck", 0777);
    if (ret == -1) {
        perror("mkdir");
        return -1;
    }

    return 0;
}
