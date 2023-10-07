#include <iostream>
using namespace std;

int main() {
    // 重定向stdin和stdout
    freopen("03_src.txt", "r", stdin);
    freopen("03_dest.txt", "w", stdout);

    int a, b;
    cin >> a >> b;

    cout << a + b << endl;

    // 关闭重定向的标准输入输出流，注意放在末尾，不然关了之后标准输入输出用不了
    fclose(stdout);
    fclose(stdin);

    cout << "hello" << endl;

    return 0;
}
