#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

// 全局变量
int tickets = 1000;
// 全局互斥量
pthread_mutex_t mutex;

void* CALLBACK_sell(void* arg) {
    // 临界区
    while (1) {
        // 加锁
        pthread_mutex_lock(&mutex);
        // pthread_mutex_lock(&mutex);

        if (tickets > 0) {
            // usleep(5000);
            printf("pthread tid %ld is selling ticket %d\n", pthread_self(), tickets);
            --tickets;
        } else {
            // 卖完了
            pthread_mutex_unlock(&mutex);
            break;
        }
        // 解锁
        pthread_mutex_unlock(&mutex);
        // pthread_mutex_unlock(&mutex);
    }

    return nullptr;
}

int main() {
    // 初始化互斥量，必须是全局的
    pthread_mutex_init(&mutex, nullptr);

    // 创建子线程
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, nullptr, CALLBACK_sell, nullptr);
    pthread_create(&tid2, nullptr, CALLBACK_sell, nullptr);
    pthread_create(&tid3, nullptr, CALLBACK_sell, nullptr);

    // 阻塞等待回收线程
    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);
    pthread_join(tid3, nullptr);

    // 释放互斥量资源
    pthread_mutex_destroy(&mutex);

    return 0;
}
