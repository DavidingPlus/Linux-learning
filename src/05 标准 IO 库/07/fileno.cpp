#include <iostream>
using namespace std;

int main() {
    FILE* file_stream = fopen("07.txt", "r");
    if (!file_stream) {
        perror("fopen");
        return -1;
    }

    int fd = fileno(file_stream);
    if (-1 == fd) {
        perror("fileno");
        return -1;
    }

    printf("file descriptor: %d\n", fd);

    return 0;
}
