#include <iostream>
using namespace std;

#define MAX_BUFFER_SIZE 1024

int main() {
    FILE* file_stream = fopen("07.txt", "r");
    if (!file_stream) {
        perror("fopen");
        return -1;
    }

    long pos = ftell(file_stream);
    cout << pos << endl;

    // 现在我读取一个字符
    char buf[MAX_BUFFER_SIZE] = {0};
    fread(buf, 1, 1, file_stream);

    cout << buf << endl;
    pos = ftell(file_stream);
    cout << pos << endl;

    // 设置文件偏移指针到开头
    // rewind(file_stream);
    fseek(file_stream, 0, SEEK_SET);

    fread(buf, 1, 2, file_stream);  // 这里第二次读从buf的地址位置开始写入，所以之前的数据会被覆盖

    cout << buf << endl;
    pos = ftell(file_stream);
    cout << pos << endl;

    return 0;
}
