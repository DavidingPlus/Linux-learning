#include <iostream>
using namespace std;

int main() {
    while (1) {
        int c = getc(stdin);
        if (EOF == c)  // getc()读取完毕的标志是EOF
            break;
        putc(c, stdout);
    }

    return 0;
}
