#include <iostream>
using namespace std;
#include <unistd.h>

/*
    #include <unistd.h>
    int access(const char *pathname, int mode);
作用：用来判断某个文件是否有某个权限，或者判断文件是否存在
参数：
    pathname：文件路径
    mode：
        R_OK 是否有读权限
        W_OK 是否有写权限
        X_OK 是否有执行权限
        F_OK 文件是否存在
返回值：
    成功 返回0
    失败(没有这个权限) 返回-1，并且修改errno
*/

int main() {
    int ret = access("1.txt", F_OK);
    if (ret == -1) {
        perror("access");
        return -1;
    }

    cout << "file exists." << endl;

    return 0;
}
