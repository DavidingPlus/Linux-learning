#include <iostream>
using namespace std;
#include <arpa/inet.h>

/*
    #include <arpa/inet.h>
        p:点分十进制的IP字符串，n:表示network，网络字节序的整数
    int inet_pton(int af, const char *src, void *dst);
        af:地址族： AF_INET AF_INET6
        src:需要转换的点分十进制的IP字符串
        dst:转换后的结果保存在这个里面，是一个传出参数
        将网络字节序的整数，转换成点分十进制的IP地址字符串
        返回值：1 成功 ；失败 0 或者 -1，0表示传入的点分制字符串不合理invalid，不设置errno；-1表示错误(比如地址族内容不合理)，并且设置errno
    const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
        af:地址族： AF_INET AF_INET6
        src: 要转换的ip的整数的地址
        dst: 转换成IP地址字符串保存的地方
        size：第三个参数的大小（数组的大小）
        返回值：返回转换后的数据的地址（字符串），和 dst 是一样的
*/

int main() {
    // 创建一个IP字符串
    const char* buf = "192.168.1.4";

    in_addr_t num;
    // 其实这里给包装他的结构体 in_addr 也是可以的，因为函数要求传入的是指针，还是 void* ，脏活系统API帮我们干完了都

    // 将点分十进制的IP字符串转换为网络字节序的整数
    inet_pton(AF_INET, buf, &num);

    unsigned char* p = (unsigned char*)&num;
    // 转化之后的结果应该是数字高位192存放在地址低位，就是大端字节序，所以结果应该是192.168.1.4
    printf("%u %u %u %u\n", *p, *(p + 1), *(p + 2), *(p + 3));

    // 将网络字节序的IP整数转换为字符串形式
    // 字符串形式的IP地址最多多少个字节，一个字符一个字节，然后数字每个最多3位，3个点，加起来就是15，然后\0符，定义16就行
    char ip_str[16] = {0};
    const char* ans = inet_ntop(AF_INET, &num, ip_str, sizeof(ip_str) - 1);

    printf("ans : %s\n", ans);
    printf("ip_str : %s\n", ip_str);
    printf("%d\n", ip_str == ans);

    return 0;
}
