#include <iostream>
using namespace std;
#include <sys/stat.h>

/*
    #include <sys/stat.h>
    int chmod(const char *pathname, mode_t mode);
作用：修改文件权限
参数：
    pathname：文件路径
    mode：需要修改的权限值，八进制的数
返回值：
    成功返回0
    失败返回-1
*/

int main() {
    int ret = chmod("1.txt", 0777);
    if (ret == -1) {
        perror("chmod");
        return -1;
    }

    return 0;
}
