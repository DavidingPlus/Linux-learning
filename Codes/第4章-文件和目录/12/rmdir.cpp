#include <iostream>
using namespace std;
#include <unistd.h>

int main() {
    // 尝试删除test文件夹
    int ret = rmdir("test/");
    if (-1 == ret) {
        perror("rmdir");
        return -1;
    }

    return 0;
}
