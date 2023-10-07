#include <ctime>
#include <iostream>
#include <string>
using namespace std;
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXSIZE 1024

// 模拟实现 ls -l 这个指令
// -rwxrwxrwx 1 lzx0626 lzx0626 107 Jul 15 09:40 ls-l.cpp

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        cout << "usage: " << argv[0] << " <filename>" << endl;
        return -1;
    }

    struct stat statbuf;

    // 通过stat()函数获取文件的信息
    int ret = stat(argv[1], &statbuf);
    if (ret == -1) {
        perror("stat");
        return -1;
    }

    // 获取文件类型和文件权限 st_mode变量
    string perms;  // 保存文件类型和权限的字符串
    mode_t _mode = statbuf.st_mode;
    // 获得文件类型和掩码 -S_IFMT 相与
    switch (_mode & S_IFMT) {
    case S_IFSOCK:
        perms.append("s");
        break;
    case S_IFLNK:
        perms.append("1");
        break;
    case S_IFREG:
        perms.append("-");
        break;
    case S_IFBLK:
        perms.append("b");
        break;
    case S_IFDIR:
        perms.append("d");
        break;
    case S_IFCHR:
        perms.append("c");
        break;
    case S_IFIFO:
        perms.append("p");
        break;
    default:
        perms.append("?");
        break;
    }

    // 判断文件访问权限 Users Group Others
    // Users
    perms.append((_mode & S_IRUSR) ? "r" : "-");
    perms.append((_mode & S_IWUSR) ? "w" : "-");
    perms.append((_mode & S_IXUSR) ? "x" : "-");
    // Group
    perms.append((_mode & S_IRGRP) ? "r" : "-");
    perms.append((_mode & S_IWGRP) ? "w" : "-");
    perms.append((_mode & S_IXGRP) ? "x" : "-");
    // Others
    perms.append((_mode & S_IROTH) ? "r" : "-");
    perms.append((_mode & S_IWOTH) ? "w" : "-");
    perms.append((_mode & S_IXOTH) ? "x" : "-");

    // 获取硬连接数
    nlink_t link_num = statbuf.st_nlink;

    // 文件所有者
    // 这个函数可以通过用户uid获得用户名称
    string _User = getpwuid(statbuf.st_uid)->pw_name;

    // 文件所在组
    // 这个函数通过组gid获得名称
    string _Group = getgrgid(statbuf.st_gid)->gr_name;

    // 文件大小
    off_t _size = statbuf.st_size;

    // 获取修改时间
    // ctime()函数可以将时间差值转化为本地时间
    string _mtime = string(ctime(&statbuf.st_mtime));
    // 这个时间格式化之后回车换行了，将其去掉
    _mtime.pop_back();

    // 输出
    char ret_buf[MAXSIZE];
    // 这个函数可以往字符串中填充
    sprintf(ret_buf, "%s %ld %s %s %ld %s %s", perms.c_str(), link_num, _User.c_str(), _Group.c_str(),
            _size, _mtime.c_str(), argv[1]);

    cout << ret_buf << endl;

    return 0;
}
