#include <iostream>
using namespace std;

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// 创建一个新的文件
// int open(const char *pathname, int flags, mode_t mode);
// pathname：创建的文件路径

// flags：对文件的操作权限和其他的设置
// 必选项：O_RDONLY, O_WRONLY, or O_RDWR 这三个之间是互斥的
// 可选项：O_CREAT 文件不存在创建新文件；O_APPEND 可以进行文件的追加
// flags参数是一个int类型的数据，占4个字节，32位，每一位就是一个标志位，1表示有，0表示没有，所以用按位或

// mode：八进制的数，表示用户对创建出的新的文件的操作权限，比如：0777
// 3个7分别表示对不同的用户(所有者，组成员，其他用户的权限)的权限，每一个都是3位 ，第一位表示读R，第二位表示写W，第三位 表示可执行X，7对应就是111全有!!!
// 最终的权限是：mode & ~umask
// umask可以通过shell命令 umask 查看
// umask的作用是为了抹去某些权限，让我们创建的权限更加合理一些
// 例子：0777 & ~0022

int main() {
    // 创建一个新的文件
    int fd = open("1.txt", O_RDWR | O_CREAT, 0777);  // 标签之间要用按位或

    if (fd == -1)
        perror("create");

    close(fd);

    return 0;
}
