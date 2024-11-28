#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

/*
    #include <pthread.h>

    int pthread_mutex_init(pthread_mutex_t *restrict mutex,
        const pthread_mutexattr_t *restrict attr);
作用：初始化互斥量
参数：
    mutex：pthread_mutex_t类型，需要初始化的互斥量变量
    attr：pthread_mutexattr_t类型，互斥量相关的属性，可以通过参数传入进行设置，一般传递nullptr使用默认的
    - restrict：C语言的修饰符，被修饰的指针指向的区域不能由另外的指针进行操作，保证了唯一性
        例如：      pthread_mutex_t *restrict mutex;
            这时候写pthread_mutex_t *mutex1=mutex;
            就是错的，因为我们限定了这块内存只能由mutex指针指向，不能由其他的指针指向

    int pthread_mutex_destroy(pthread_mutex_t *mutex);
作用：释放互斥量的资源

    int pthread_mutex_lock(pthread_mutex_t *mutex);
作用：加锁，这个方法是阻塞的，如果有一个线程加锁了，其他的线程只能阻塞等待

    int pthread_mutex_trylock(pthread_mutex_t *mutex);
作用：尝试加锁，如果加锁失败，不会阻塞，而直接返回

    int pthread_mutex_unlock(pthread_mutex_t *mutex);
作用：解锁
*/

// 全局变量
int tickets = 1000;
// 全局互斥量
pthread_mutex_t mutex;

void* CALLBACK_sell(void* arg) {
    // 临界区
    while (1) {
        // 加锁
        pthread_mutex_lock(&mutex);

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
