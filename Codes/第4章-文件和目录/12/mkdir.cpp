#include <iostream>
using namespace std;
#include <sys/stat.h>
#include <unistd.h>

int main() {
    // 创建一个新目录，这次我用绝对路径
    int ret = mkdirat(-1, "/mnt/d/Code/Cpp/为了工作/Linux/Unix环境高级编程/第4章-文件和目录/12/test", 0664);
    if (-1 == ret) {
        perror("mkdirat");
        return -1;
    }

    // 调用exec函数族在目录里面创建一个文件
    execlp("touch", "touch", "./test/12.txt", nullptr);

    return 0;
}
