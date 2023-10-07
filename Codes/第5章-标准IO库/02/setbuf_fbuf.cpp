#include <iostream>
using namespace std;
#include <unistd.h>

char outbuf[BUFSIZ];  // BUFSIZ是指默认给的缓冲区大小，是8192个字节

int main() {
    setbuf(stdout, outbuf);  // 把缓冲区与流相连

    puts("This is a test of buffered output.");  // puts最后会自带一个换行符
    puts(outbuf);                                // 这里我除了写入自定义字符串，我把outbuf的内容也写一遍，所以会有两个换行符

    sleep(3);

    fflush(stdout);  // 刷新

    puts("This is a test of buffered output.");

    sleep(3);

    return 0;
}
