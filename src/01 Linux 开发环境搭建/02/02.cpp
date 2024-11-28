#include <iostream>
using namespace std;

int main() {
    int a = 10;

    // 用 -D 来表示在编译的时候指定一个宏，从而可以输出这句话!!! -DDEBUG
    // 一个用途是：可以在用于调试的时候输出一些信息来检测程序，对程序整体的运行没有太大作用
#ifdef DEBUG
    cout << "FUCK" << endl;
#endif
    cout << "you" << endl;

    int b, c, d, e;
    b = 10;
    c = b;
    d = c;
    e = d;

    // 使用 -On 进行优化,比如可以优化成如下
    // int b, c, d, e;
    // b = 10;
    // c = 10;
    // d = 10;
    // e = 10;

    return 0;
}
