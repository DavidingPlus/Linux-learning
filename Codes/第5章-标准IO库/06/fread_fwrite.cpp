#include <iostream>
using namespace std;
#include <string>

struct Person {
    string name;
    string sex;
    double height;
    double weight;
};

int main() {
    // 将二进制数组的一些元素写到标准输出
    char data[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

    fwrite(&data[2], sizeof(char), 3, stdout);

    puts("");  // 输出一个空行

    // 写一个结构到标准输出
    Person p{"Paul", "boy", 190, 88.5};

    fwrite(&p, sizeof(p), 1, stdout);

    puts("");  // 输出一个空行

    return 0;
}
