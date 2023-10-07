#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

// 使用多线程去实现卖票的案例
// 有3个窗口，一共100张票

// 全局变量，但是还是有问题
int tickets = 100;

void* CALLBACK_sell(void* arg) {
    while (tickets > 0) {
        usleep(8000);
        printf("pthread tid %ld is selling ticket %d\n", pthread_self(), tickets);
        --tickets;
    }

    return nullptr;
}

int main() {
    // 创建子线程
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, nullptr, CALLBACK_sell, nullptr);
    pthread_create(&tid2, nullptr, CALLBACK_sell, nullptr);
    pthread_create(&tid3, nullptr, CALLBACK_sell, nullptr);

    // 分离线程
    pthread_detach(tid1);
    pthread_detach(tid2);
    pthread_detach(tid3);

    // 主线程退出
    pthread_exit(nullptr);

    return 0;
}
