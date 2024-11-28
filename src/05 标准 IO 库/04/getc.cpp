#include <iostream>
using namespace std;

int main() {
    // 复习一下，将标准输入重定向
    freopen("04.txt", "r", stdin);
    // freopen("05.txt", "r", stdin);

    int ret;
    while (EOF != (ret = getc(stdin)))
        cout << (unsigned char)ret;

    // 我们来看一下EOF对应的是哪个状态
    if (ferror(stdin))
        cout << "ferror: read error" << endl;
    if (feof(stdin))
        cout << "feof: end of file" << endl;

    // 关闭
    fclose(stdin);

    return 0;
}
