#include <iostream>
using namespace std;
#include <unistd.h>

int main() {
    FILE* file_stream = fopen("05.txt", "r");
    if (!file_stream) {
        perror("fopen");
        return -1;
    }

    char buf[1024] = {0};

    fgets(buf, 5, file_stream);  // 如果读的数据超过缓冲区大小，那么最后一个字符会留为'\0'，然后冲洗缓冲区到buf当中

    cout << buf;

    sleep(5);

    return 0;
}
