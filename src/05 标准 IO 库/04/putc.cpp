#include <iostream>
using namespace std;
#include <unistd.h>

int main() {
    // 打开一个文件
    FILE* file_stream = fopen("04.txt", "a+");

    setvbuf(file_stream, nullptr, _IOFBF, 0);

    // 先写入几个字符
    for (int i = 0; i < 10; ++i)
        putc((int)'a' + i, file_stream);

    // 最后写一个换行符
    putc((int)'\n', file_stream);

    sleep(5);

    // 重置文件偏移量指针
    fseek(file_stream, 0, SEEK_SET);

    int ret = EOF;
    while (EOF != (ret = getc(file_stream)))
        cout << (unsigned char)ret;

    return 0;
}
