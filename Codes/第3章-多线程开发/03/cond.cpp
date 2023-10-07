#include <ctime>
#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>

/*
    生产者消费者模型(粗略的版本)
*/

// 创建互斥量来解决数据同步的问题
pthread_mutex_t mutex;
// 定义条件变量
pthread_cond_t cond;

// 定义一个链表
struct Node {
    int val;
    struct Node* next;
}* head = nullptr;  // 定义头结点

void* PRO_CALLBACK(void* args) {
    // 不断生成新节点，插入到链表当中(头插)
    while (1) {
        pthread_mutex_lock(&mutex);

        struct Node* newNode = new struct Node;
        newNode->next = head;
        head = newNode;

        newNode->val = rand() % 1000;

        printf("add node , val : %d , tid : %ld\n", newNode->val, pthread_self());

        pthread_mutex_unlock(&mutex);

        pthread_cond_signal(&cond);

        usleep(1000);
    }

    return nullptr;
}

void* CUS_CALLBACK(void* args) {
    // 不断从头部释放头结点
    while (1) {
        pthread_mutex_lock(&mutex);

        struct Node* tmp = head;

        // 这里如果没有数据head就为nullptr就会报错这一行，非法访问内存
        // 需要进行判断
        if (head != nullptr) {
            head = head->next;
            printf("delete node , val : %d , tid : %ld\n", tmp->val, pthread_self());

            delete tmp;
            tmp = nullptr;

            pthread_mutex_unlock(&mutex);

            usleep(1000);
        } else {
            // 没有数据，需要阻塞等待
            // 当这个函数调用阻塞的时候，会解锁，当不阻塞的时候继续向下执行，会重新加锁
            pthread_cond_wait(&cond, &mutex);
            // 当有数据唤醒之后，我们的代码逻辑是重新进入循环加锁，因此必须提前释放锁
            pthread_mutex_unlock(&mutex);
        }
    }

    return nullptr;
}

int main() {
    // 初始化互斥锁
    pthread_mutex_init(&mutex, nullptr);
    // 初始化条件变量
    pthread_cond_init(&cond, nullptr);

    // 创建5个生产者线程，和5个消费者线程
    pthread_t ptids[5], ctids[5];

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

    // 释放条件变量
    pthread_cond_destroy(&cond);
    // 释放互斥锁
    pthread_mutex_destroy(&mutex);

    // 主线程退出(这里其实没什么用了)
    pthread_exit(nullptr);

    return 0;
}
