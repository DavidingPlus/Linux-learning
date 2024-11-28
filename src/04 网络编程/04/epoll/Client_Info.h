#ifndef _CLIENT_INFO_
#define _CLIENT_INFO_

#include <arpa/inet.h>

#include <cstring>

#define MAX_IPV4_STRING 16

class Client_Info {
public:
    Client_Info() {
        __init__();
    };

    Client_Info& operator=(const Client_Info& _cli_info) {
        strcpy(this->client_ip, _cli_info.client_ip);
        this->client_port = _cli_info.client_port;

        return *this;
    }

    Client_Info(const char* _ip, const in_port_t& _port) {
        strcpy(this->client_ip, _ip);
        this->client_port = _port;
    }

    Client_Info(const Client_Info& _cli_info) {
        *this = _cli_info;
    }

    void __init__() {
        bzero(this->client_ip, sizeof(this->client_ip));
        this->client_port = 0;
    }

public:
    char client_ip[MAX_IPV4_STRING];
    in_port_t client_port;
};

#endif
