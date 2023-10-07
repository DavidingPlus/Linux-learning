#include <iostream>
using namespace std;
#include <arpa/inet.h>

/*
    网络通信时，需要将主机字节序转化为网络字节序(大端)
    另外一端获取到数据以后，根据情况将网络字节序转换为主机字节序

    #include <arpa/inet.h>
    // 转换端口 端口号 0-65535，就是16位，因此是 uint16_t
    uint16_t htons(uint16_t hostshort); // 主机字节序 - 网络字节序
    uint16_t ntohs(uint16_t netshort); // 主机字节序 - 网络字节序
    // 转IP IP地址，IPV4是32位，因此是uint32_t
    uint32_t htonl(uint32_t hostlong); // 主机字节序 - 网络字节序
    uint32_t ntohl(uint32_t netlong); // 主机字节序 - 网络字节序
*/

int main() {
    // htons() 转换端口
    unsigned short a = 0x0102;
    printf("a : %x\n", a);
    unsigned short b = htons(a);
    printf("b : %x\n", b);

    printf("----------------------------------------------\n");

    // htonl() 转换IP
    // 这里用char会报 narrowing convertions 缩窄转换
    // 这是c++11在使用初始化序列时候编译器会自动判断，如果发生缩窄转换就会报错
    unsigned char buf[4] = {192, 168, 1, 100};

    int num = *(int*)buf;
    int ans = htonl(num);

    unsigned char* p = (unsigned char*)&ans;
    printf("%u %u %u %u\n", *p, *(p + 1), *(p + 2), *(p + 3));

    printf("----------------------------------------------\n");

    // ntohl()
    unsigned char buf1[4] = {1, 1, 168, 192};
    int num1 = *(int*)buf1;
    int ans1 = ntohl(num1);
    unsigned char* p1 = (unsigned char*)&ans1;
    printf("%u %u %u %u\n", *p1, *(p1 + 1), *(p1 + 2), *(p1 + 3));

    return 0;
}
