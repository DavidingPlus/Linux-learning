#include <cstring>
#include <iostream>
using namespace std;
#include <sys/ipc.h>
#include <sys/shm.h>
#define _size 1024

int main() {
    // 创建共享内存
    int shmid = shmget(100, 4096, 0664 | IPC_CREAT);
    if (-1 == shmid) {
        perror("shmget");
        return -1;
    }
    printf("shmid : %d\n", shmid);

    // 和当前进程进行关联
    void *ptr = shmat(shmid, nullptr, 0);
    if ((void *)-1 == ptr) {
        perror("shmat");
        return -1;
    }

    char str[_size] = {0};

    printf("请输入写入的字符串: ");
    fgets(str, sizeof(str), stdin);

    // 写数据
    memcpy(ptr, str, strlen(str) + 1);  // 为了保险，拷上字符串结束符

    printf("按任意键继续\n");
    getchar();

    // 解除关联
    int ret = shmdt(ptr);
    if (-1 == ret) {
        perror("shmdt");
        return -1;
    }

    // 删除共享内存
    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}
