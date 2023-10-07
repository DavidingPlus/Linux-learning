#include <iostream>
using namespace std;
#include <fcntl.h>

int main() {
    int fd = open("/dev/fd/0", O_RDONLY);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    cout << fd << endl;  // 3 说明是复制了这个文件描述符，他们两个指向同一个文件表项

    return 0;
}
