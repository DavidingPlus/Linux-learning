#include <iostream>
using namespace std;
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>

    int stat(const char *pathname, struct stat *statbuf);
作用：获取一个文件的相关的信息
参数：
    pathname：操作的文件路径
    statbuf：结构体变量，传出参数，用于保存获取到的文件信息
返回值：
    成功 0
    失败 -1，并且修改errno

    int lstat(const char *pathname, struct stat *statbuf);
参数，返回值同上

*/

int main() {
    struct stat statbuf;

    int ret = stat("1.txt", &statbuf);
    if (ret == -1) {
        perror("stat");
        return -1;
    }

    cout << "size: " << statbuf.st_size << endl;

    ret = stat("2.txt", &statbuf);
    if (ret == -1) {
        perror("stat");
        return -1;
    }

    cout << "size: " << statbuf.st_size << endl;

    ret = lstat("2.txt", &statbuf);
    if (ret == -1) {
        perror("stat");
        return -1;
    }

    cout << "size: " << statbuf.st_size << endl;

    return 0;
}
