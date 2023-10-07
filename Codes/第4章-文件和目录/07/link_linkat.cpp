#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

int main() {
    // link函数
    int ret = link("07.txt", "07_1.txt");
    if (-1 == ret) {
        perror("link");
        return -1;
    }

    // linkat函数
    int dir_fd = open("./", O_DIRECTORY);
    if (-1 == dir_fd) {
        perror("open");
        return -1;
    }

    ret = linkat(dir_fd, "07.txt", AT_FDCWD, "07_2.txt", 0);
    if (-1 == ret) {
        perror("linkat");
        return -1;
    }

    return 0;
}
