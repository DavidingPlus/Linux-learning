#include <iostream>
using namespace std;

int main() {
    int ret = rename("08_1.txt", "08.txt");
    if (-1 == ret) {
        perror("rename");
        return -1;
    }

    return 0;
}
