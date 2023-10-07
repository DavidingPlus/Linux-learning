#include <ctime>
#include <iostream>
using namespace std;
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/*
    #include <semaphore.h>

    int sem_init(sem_t *sem, int pshared, unsigned int value);
作用：初始化信号量
参数：
    sem：信号量变量的地址
    pshared：0 用在线程； 非0 用在进程
    value：信号量的值

    int sem_destroy(sem_t *sem);
作用：释放资源

    int sem_wait(sem_t *sem);
作用：对信号量加锁，调用一次，对信号量的值减1，如果值为0，就阻塞

    int sem_trywait(sem_t *sem);
作用：尝试wait

    int sem_timedwait(sem_t *restrict sem, const struct timespec *restrict abs_timeout);
作用：等待一段时间

    int sem_post(sem_t *sem);
作用：解锁一个信号量，调用一次，对信号量的值加1

    int sem_getvalue(sem_t *restrict sem, int *restrict sval);
作用：获取信号量的值

*/

// 创建互斥量来解决数据同步的问题
pthread_mutex_t mutex;
// 创建两个信号量
sem_t psem, csem;

// 定义一个链表
struct Node {
    int val;
    struct Node* next;
}* head = nullptr;  // 定义头结点

void* PRO_CALLBACK(void* args) {
    // 不断生成新节点，插入到链表当中(头插)
    while (1) {
        sem_wait(&psem);

        pthread_mutex_lock(&mutex);

        struct Node* newNode = new struct Node;
        newNode->next = head;
        head = newNode;

        newNode->val = rand() % 1000;

        printf("add node , val : %d , tid : %ld\n", newNode->val, pthread_self());

        pthread_mutex_unlock(&mutex);

        sem_post(&csem);  // 将消费者的信号量加1表示可以进行消费

        usleep(1000);
    }

    return nullptr;
}

void* CUS_CALLBACK(void* args) {
    // 不断从头部释放头结点
    // 这就是操作系统当中学的经典的PV问题了，巩固一下
    while (1) {
        sem_wait(&csem);

        pthread_mutex_lock(&mutex);

        struct Node* tmp = head;

        head = head->next;
        printf("delete node , val : %d , tid : %ld\n", tmp->val, pthread_self());

        delete tmp;
        tmp = nullptr;

        pthread_mutex_unlock(&mutex);

        sem_post(&psem);  // 将生产者的信号量加1表示可以生产

        usleep(1000);
    }

    return nullptr;
}

int main() {
    // 初始化互斥锁
    pthread_mutex_init(&mutex, nullptr);
    // 初始化信号量
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);

    // 创建5个生产者线程，和5个消费者线程
    pthread_t ptids[5],
        ctids[5];

    for (int i = 0; i < 5; ++i) {
        pthread_create(&ptids[i], nullptr, PRO_CALLBACK, nullptr);
        pthread_create(&ctids[i], nullptr, CUS_CALLBACK, nullptr);
    }

    // 回收线程
    for (int i = 0; i < 5; ++i) {
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }

    // 用死循环来保证主线程不会结束，如果用 pthread_exit() 会导致互斥锁释放的位置问题
    while (1)
        ;

    // 释放互斥锁
    pthread_mutex_destroy(&mutex);

    // 主线程退出(这里其实没什么用了)
    pthread_exit(nullptr);

    return 0;
}
