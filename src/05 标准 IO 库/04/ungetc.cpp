#include <iostream>
using namespace std;

int main() {
    // 打开一个文件
    FILE* file_stream = fopen("04.txt", "r");

    // 先压入几个字符
    for (int i = 0; i < 10; ++i)
        ungetc((int)'a' + i, file_stream);

    int ret = EOF;
    while (EOF != (ret = getc(file_stream)))
        cout << (unsigned char)ret;

    return 0;
}
