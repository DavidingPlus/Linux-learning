#include <iostream>
using namespace std;
#include <bitset>

int main() {
    int num = 100;

    cout << bitset<10>(num) << endl;  // 0001100100，模板参数10是输出多少位
    cout << oct << num << endl;       // 144，oct流
    cout << hex << num << endl;       // 64，hex流

    return 0;
}
