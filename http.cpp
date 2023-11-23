#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    // std::string path = "/";
    std::string path = "/image";
    std::string host = "106.55.60.140";
    int port = 8081;

    std::string send_message = "GET " + path + " HTTP/1.1\r\n";
    send_message += "Host: " + host + "\r\n";
    send_message += "Connection: keep-alive\r\n";
    // send_message += "User-Agent: Mozilla/5.0\r\n";
    // send_message += "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n";
    send_message += "\r\n";

    int connect_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == connect_fd) {
        perror("socket");
        return -1;
    }

    struct hostent *p_hostent = gethostbyname(host.c_str());

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    memcpy(&(server_addr.sin_addr), p_hostent->h_addr_list[0], sizeof(server_addr.sin_addr));
    server_addr.sin_port = htons(port);

    int ret = connect(connect_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (-1 == ret) {
        perror("connect");
        return -1;
    }

    std::cout << send_message << "--------------------------------" << std::endl;

    send(connect_fd, send_message.c_str(), send_message.size(), 0);

    char readBuffer[BUFSIZ] = {0};
    std::string std_readMessage;

    // 定义一些数据，在下面需要用到
    size_t data_start = 0;               // 响应体的开头下标
    size_t data_end = 0;                 // 响应体的结束下标
    size_t content_length = 0;           // 响应体的长度
    bool is_Location_in_header = false;  // url是否进行了重定向
    int count = 0;                       // 计数，为了提高效率

    // 我换了一个算法，能读取到全部的数据，就是缓冲区满了需要多次读取的时候
    while (1) {
        bzero(readBuffer, BUFSIZ);
        int len = recv(connect_fd, readBuffer, BUFSIZ - 1, 0);
        if (-1 == len) {
            perror("recv");
            exit(-1);
        }
        if (len > 0) {
            std_readMessage.insert(std_readMessage.end(), readBuffer, readBuffer + len);

            if (0 == count) {
                // 检查收到的数据是否完整包含了响应头，这一步骤只进行一次，目的是为了得到响应的数据
                size_t headers_end = std_readMessage.find("\r\n\r\n");
                if (std::string::npos != headers_end) {
                    // 下一次就不进来了，写在这也可以保证如果第一次没有完整读到响应头，也能从这里进来并且下一次就不进来了
                    ++count;
                    // 注意substr第一个参数是子串的开始位置，第二个参数是字串的长度
                    std::string headers = std_readMessage.substr(0, headers_end);

                    // 查看是否有重定向存在
                    if (!is_Location_in_header) {
                        size_t location_pos = headers.find("Location:");
                        if (std::string::npos != location_pos)
                            is_Location_in_header = true;
                    }

                    size_t content_length_pos = headers.find("Content-Length:");
                    if (std::string::npos != content_length_pos) {
                        size_t content_length_end = headers.find("\r\n", content_length_pos);

                        content_length = atoi(headers.substr(content_length_pos + strlen("Content-Length: "), content_length_end - content_length_pos - strlen("Content-Length: ")).c_str());

                        // 拿到所需要的数据
                        data_start = headers_end + 4;
                        data_end = data_start + content_length;
                    }
                }
            }

            // 得到了所有的数据，循环结束
            if (std_readMessage.size() - data_start >= content_length)
                break;
        } else if (0 == len)
            break;  // 服务端关闭了...
    }

    // std::cout << std_readMessage << std::endl;
    std::cout << "Received data size: " << std_readMessage.size() << " bytes" << std::endl;

    // 将数据部分从std_readMessage中提取出来
    std::string data_str = std_readMessage.substr(data_start, data_end - data_start);

    // 将数据部分写入文件
    FILE *file = fopen("image.jpg", "w+");
    if (nullptr == file) {
        perror("fopen");
        return -1;
    }

    fwrite(data_str.c_str(), 1, data_str.size(), file);

    fclose(file);

    std::cout << "Image saved to image.jpg" << std::endl;

    close(connect_fd);

    return 0;
}
