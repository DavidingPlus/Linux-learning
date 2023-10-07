#include <cstring>
#include <iostream>
using namespace std;

#define MAX_BUFFER_SIZE 1024

int main() {
    // 从06.txt中读取数据，然后输出到屏幕上
    FILE* file_stream = fopen("06.txt", "r");
    if (!file_stream) {
        perror("fopen");
        return -1;
    }

    char buf[MAX_BUFFER_SIZE] = {0};
    // 读到buf中
    fread(buf, 1, sizeof(buf), file_stream);

    // 标准输出
    int ret = fwrite(buf, 1, strlen(buf), stdout);
    if (ret != strlen(buf)) {
        perror("fwrite");
        return -1;
    }

    return 0;
}
