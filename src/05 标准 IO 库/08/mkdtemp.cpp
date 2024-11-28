#include <iostream>
using namespace std;

int main() {
    char name[1024] = {"./mydirXXXXXX"};

    char *file_path = mkdtemp(name);
    if (!file_path) {
        perror("mkdtemp");
        return -1;
    }

    cout << name << endl;
    cout << file_path << endl;

    return 0;
}
