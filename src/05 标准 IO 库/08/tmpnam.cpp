#include <iostream>
using namespace std;

int main() {
    char name[1024] = {0};

    // c++使用这个函数会被警告说不安全，tmpnam函数返回一个临时文件的路径名，这个路径名中对应的文件名和本文件不相同
    const char* tmp_path = tmpnam(name);
    if (!tmp_path) {
        perror("tmpnam");
        return -1;
    }

    cout << TMP_MAX << endl;

    cout << tmp_path << endl;
    cout << name << endl;

    return 0;
}
