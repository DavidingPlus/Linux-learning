#include <ctime>
#include <iostream>
#include <string>
using namespace std;
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>

#define MAX_FILEINFO_SIZE 1024

int main(int argc, char const* argv[]) {
    if (argc < 2) {
        printf("usage: %s  <filename>\n", argv[0]);
        return -1;
    }

    const char* filename = argv[1];

    struct stat stat_buf;

    int ret = stat(filename, &stat_buf);
    if (-1 == ret) {
        perror("stat");
        return -1;
    }

    // 开始解读stat_buf数组的文件信息内容
    string file_info;

    // 获取文件类型
    switch (stat_buf.st_mode & S_IFMT) {
    case S_IFSOCK:
        file_info.append("s");
        break;
    case S_IFLNK:
        file_info.append("1");
        break;
    case S_IFREG:
        file_info.append("-");
        break;
    case S_IFBLK:
        file_info.append("b");
        break;
    case S_IFDIR:
        file_info.append("d");
        break;
    case S_IFCHR:
        file_info.append("c");
        break;
    case S_IFIFO:
        file_info.append("p");
        break;
    default:
        file_info.append("?");
        break;
    }

    // 获取三个权限
    file_info.append(stat_buf.st_mode & S_IRUSR ? "r" : "-");
    file_info.append(stat_buf.st_mode & S_IWUSR ? "w" : "-");
    file_info.append(stat_buf.st_mode & S_IXUSR ? "x" : "-");

    file_info.append(stat_buf.st_mode & S_IRGRP ? "r" : "-");
    file_info.append(stat_buf.st_mode & S_IWGRP ? "w" : "-");
    file_info.append(stat_buf.st_mode & S_IXGRP ? "x" : "-");

    file_info.append(stat_buf.st_mode & S_IROTH ? "r" : "-");
    file_info.append(stat_buf.st_mode & S_IWOTH ? "w" : "-");
    file_info.append(stat_buf.st_mode & S_IXOTH ? "x" : "-");

    // 获取文件硬连接数
    nlink_t nlink_num = stat_buf.st_nlink;

    // 获取文件所有者
    // getpwuid()通过用户ID反向获得用户名称
    // 引头文件 <grp.h>
    string User = getpwuid(stat_buf.st_uid)->pw_name;

    // 获取文件所在组
    // getgrgid()通过用户组ID反向获得组名称
    // 引头文件 <pwd.h>
    string Group = getgrgid(stat_buf.st_gid)->gr_name;

    // 获得文件的大小
    off_t file_size = stat_buf.st_size;

    // 获取修改时间
    // ctime()函数可以将时间差值转化为本地时间
    string mtime = string(ctime(&stat_buf.st_mtime));
    // 这个时间格式化之后回车换行了，将其去掉
    mtime.pop_back();

    // 格式化信息
    char buf[MAX_FILEINFO_SIZE] = {0};
    sprintf(buf, " %ld %s %s %ld %s %s", nlink_num, User.c_str(),
            Group.c_str(), file_size, mtime.c_str(), filename);

    file_info += buf;

    // 输出信息
    cout << file_info << endl;

    return 0;
}
