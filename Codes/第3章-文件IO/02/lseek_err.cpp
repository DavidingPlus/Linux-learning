#include <iostream>
using namespace std;
#include <unistd.h>

int main() {
    // 我们看能否修改标准输入的偏移量
    off_t offset = lseek(STDIN_FILENO, 100, SEEK_CUR);
    if (-1 == offset)
        perror("lseek");
    else
        printf("lseek ok.\n");

    return 0;
}
