#include <iostream>
using namespace std;
#include <fcntl.h>

int main() {
    int fd = open("02.txt", O_RDWR);
    if (-1 == fd) {
        perror("open");
        return -1;
    }

    return 0;
}
