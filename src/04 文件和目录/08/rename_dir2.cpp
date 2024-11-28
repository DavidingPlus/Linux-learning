#include <iostream>
using namespace std;

int main() {
    int ret = rename("test", "test2");
    if (-1 == ret) {
        perror("rename");
        return -1;
    }

    return 0;
}
