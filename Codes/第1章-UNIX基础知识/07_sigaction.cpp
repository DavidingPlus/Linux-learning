#include <cstring>
#include <iostream>
using namespace std;
#include <signal.h>

#define MAX_BUFFER_SIZE 1024

void my_hander(int signum);

int main() {
    // 设置一些参数
    struct sigaction act;
    act.sa_handler = my_hander;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, nullptr);

    char buf[MAX_BUFFER_SIZE] = {0};
    while (1) {
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf) - 1, stdin);

        write(STDOUT_FILENO, buf, strlen(buf));
    }

    return 0;
}

void my_hander(int signum) {
    printf("signal SIGINT got.\n");
}
