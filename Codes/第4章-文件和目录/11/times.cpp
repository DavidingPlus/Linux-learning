#include <iostream>
using namespace std;
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    // 11.txt文件是之前创建出来的，我们先获取它的信息，然后修改这个文件，然后再修改文件访问和修改时间看是否成功
    struct stat stat_buf;

    int ret = stat("11.txt", &stat_buf);
    if (-1 == ret) {
        perror("stat");
        return -1;
    }

    // 获得两个修改时间
    struct timespec last_visited = stat_buf.st_atim;
    struct timespec last_modified = stat_buf.st_mtim;
    struct timespec times[2] = {last_visited, last_modified};

    // 修改文件，将其截断为0
    int fd = open("11.txt", O_RDWR | O_TRUNC);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    // 修改文件的访问时间和修改时间
    ret = futimens(fd, times);
    if (-1 == ret) {
        perror("futimens");
        return -1;
    }

    return 0;
}
