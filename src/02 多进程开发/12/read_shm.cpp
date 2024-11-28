#include <cstring>
#include <iostream>
using namespace std;
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    // 获得共享内存的标识，我们是用key标识的
    int shmid = shmget(100, 4096, IPC_CREAT);
    if (-1 == shmid) {
        perror("shmget");
        return -1;
    }
    printf("shmid : %d\n", shmid);

    // 绑定连接
    void* ptr = shmat(shmid, nullptr, 0);
    if ((void*)-1 == ptr) {
        perror("shmat");
        return -1;
    }

    // 读数据
    printf("data : %s", (char*)ptr);

    printf("按任意键继续\n");
    getchar();

    // 关闭关联
    int ret = shmdt(ptr);
    if (-1 == ret) {
        perror("shmdt");
        return -1;
    }

    // 标记删除
    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}
