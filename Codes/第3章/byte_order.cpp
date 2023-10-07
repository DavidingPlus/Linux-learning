#include <iostream>
using namespace std;

int main() {
    // 给一个双字节的数据
    short num = 0x1223;

    char* ch = (char*)&num;

    cout << ((*ch == 0x23) ? "小端字节序" : "大端字节序") << endl;

    return 0;
}
