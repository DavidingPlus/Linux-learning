#include <iostream>
#include <string>
using namespace std;
#include <dirent.h>
#include <sys/types.h>

/*
    //打开目录
    #include <dirent.h>
    #include <sys/types.h>
    DIR *opendir(const char *name);
参数：
    name：需要打开的目录的名称
返回值：
    DIR * 类型，理解为目录流
    错误 返回nullptr

    //读取目录中的数据
    #include <dirent.h>
    struct dirent *readdir(DIR *dirp);
参数：
    dirp是通过opendir返回的结果
返回值：
    struct dirent 代表读取到的文件的信息
    读取到了文件末尾或者失败了，返回Null，区别是读到文件末尾不会修改errno，失败会修改

    //关闭目录
    #include <dirent.h>
    int closedir(DIR *dirp);
*/

// 递归函数，用于获取目录下所有普通文件的个数
void getFileNum(string path, int& num) {
    // 打开目录
    DIR* _dir = opendir(path.c_str());
    if (_dir == nullptr) {
        perror("opendir");
        exit(0);
    }

    // 读取目录数据
    // 循环读取，因为readdir是一个一个读取的，读到末尾返回Null
    struct dirent* _ptr;

    while ((_ptr = readdir(_dir)) != nullptr) {
        // 获取名称
        // Shell终端中有当前目录 ./ 和 上级目录 ../，这两个不能拿来递归，需要忽略
        string _dname = _ptr->d_name;
        if (_dname == "." || _dname == "..")
            continue;

        // 判断是普通文件还是目录
        if (_ptr->d_type == DT_DIR)
            // 目录需要拼接一下
            getFileNum(path + "/" + _dname, num);
        else if (_ptr->d_type == DT_REG)
            ++num;
    }

    // 关闭目录
    closedir(_dir);
}

int main(int argc, char* const argv[]) {
    // 读取某个目录下所有普通文件的个数
    if (argc < 2) {
        cout << "usage: " << argv[0] << " <path>" << endl;
        return -1;
    }

    int num = 0;
    getFileNum(argv[1], num);
    cout << "普通文件的个数为: " << num << endl;

    return 0;
}
