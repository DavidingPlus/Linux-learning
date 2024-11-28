#include <iostream>
using namespace std;

/*
    #include <stdio.h>

    int rename(const char *oldpath, const char *newpath);
一看就懂了
*/

int main() {
    int ret = rename("fuck", "fuckyou");
    if (ret == -1) {
        perror("rename");
        return -1;
    }

    return 0;
}
