#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* const argv[]) {
    if (argc < 2) {
        printf("usage: %s  <filename>\n", argv[0]);
        return -1;
    }

    const char* filename = argv[1];

    // 检测这个文件是否具有读权限
    if (0 == faccessat(AT_FDCWD, filename, R_OK, 0))
        cout << "read access ok" << endl;
    else
        perror("read access error");

    // 看打开这个文件会不会被拒绝
    if (-1 != open(filename, O_RDONLY))
        cout << "open for reading ok" << endl;
    else
        perror("open for reading error");

    return 0;
}
