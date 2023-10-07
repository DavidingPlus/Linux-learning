#include <iostream>
using namespace std;

/*
字节序：字节在内存中存储的顺序
小端字节序：数据的高位字节存储在内存的高位地址，低位字节存储在内存的低位地址
大端字节序：数据的高位字节存储在内存的低位地址，低位字节存储在内存的高位地址(与前面的相反)
*/

// 通过代码检测当前主机的字节序

// 联合里面的数据地址的起始位置都是从头开始的，因此指向的内存很大概率是有重复的
union Fuck {
    short value;                // 两个字节
    char bytes[sizeof(short)];  // 两个字节的数组，可以查看两个字节分成一半是怎么存储的
} test;

int main() {
    test.value = 0x0102;

    if (test.bytes[0] == 1 && test.bytes[1] == 2)  // 数据的高位对应字节存储的低位，则为大端字节序
        printf("大端字节序\n");
    else
        printf("小端字节序\n");

    // cout << (char(0x0102) == 0x02 ? "小端字节序" : "大端字节序") << endl;  // 这么写更加简单

    return 0;
}
