#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>

int main() {
    int dir_fd = open("../08/", O_DIRECTORY);
    if (-1 == dir_fd) {
        perror("open");
        return -1;
    }

    // int ret = renameat(dir_fd, "./test", AT_FDCWD, "./test2/");
    int ret = renameat(dir_fd, "./test", AT_FDCWD, "./test/test2/");
    if (-1 == ret) {
        perror("rename");
        return -1;
    }

    close(dir_fd);

    return 0;
}
