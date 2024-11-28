#include <iostream>
using namespace std;
#include <dirent.h>

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("usage : %s  <pathname>\n", argv[0]);
        return -1;
    }

    // 打开文件目录
    const char *pathname = argv[1];

    DIR *_dir = opendir(pathname);
    if (nullptr == _dir) {
        perror("opendir");
        return -1;
    }

    // 开始循环读取，因为一次只能读一个
    while (1) {
        struct dirent *_dirent = readdir(_dir);
        if (nullptr == _dirent)
            break;

        cout << _dirent->d_name << '\t';
    }

    // 关闭文件目录
    closedir(_dir);

    cout << endl;

    return 0;
}
