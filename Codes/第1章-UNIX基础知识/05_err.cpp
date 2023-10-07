#include <cstring>
#include <iostream>
using namespace std;

int main() {
    char* error_msg = strerror(EACCES);  // Permission denied
    cerr << error_msg << endl;

    errno = ENOENT;  // No such file or directory
    perror("msg");

    return 0;
}
