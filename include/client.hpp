#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h> //for socket()
#include <netinet/in.h> //for sockaddr_in()
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h> //for inet_ntoa()
#include <sys/epoll.h>
#include <csignal> //sigaction

class Client
{
private:
    int _fd;
    std::string _ip;
public:
    Client() {}

    int GetFD() const { return _fd; }
    void SetFD(int fd) { _fd = fd; }
    void SetIpAddress(const std::string& ip) { _ip = ip; }
};

#endif
