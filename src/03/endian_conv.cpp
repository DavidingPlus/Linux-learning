#include <iostream>
using namespace std;
#include <arpa/inet.h>

int main() {
    unsigned short host_port = 0x1234;
    unsigned long host_addr = 0x12345678;

    unsigned short net_port = htons(host_port);
    unsigned long net_addr = htonl(host_addr);

    // hex流可以让一个数以十六进制输出!!!
    cout << "Host ordered port: 0x" << hex << host_port << endl;
    cout << "Network ordered port: 0x" << hex << net_port << endl;
    cout << "Host ordered address: 0x" << hex << host_addr << endl;
    cout << "Network ordered address: 0x" << hex << net_addr << endl;

    return 0;
}
