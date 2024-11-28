#include <iostream>
using namespace std;
#include <wchar.h>

int main() {
    FILE* file = fopen("01.txt", "r");
    if (nullptr == file) {
        perror("fopen");
        return -1;
    }

    int ret = fwide(file, 100);
    cout << ret << endl;

    // 现在我在已经设置宽定向的基础上再设置一次
    ret = fwide(file, -100);
    cout << ret << endl;

    return 0;
}
