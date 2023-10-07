#include <iostream>
using namespace std;
#include <unistd.h>

int main() {
    int ret = unlink("07.txt");
    if (-1 == ret) {
        perror("unlink");
        return -1;
    }

    return 0;
}
