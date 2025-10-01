#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include <map>
#include <iostream>
#include "Client.hpp"
#include "colors.hpp"

class Server
{
private:
    uint16_t port_;
    int socket_fd_;
    int epoll_fd_;
    std::map<int, Client> clients_;
    std::string servername;
    std::string version;

    volatile static std::sig_atomic_t running;

    void InitListeningSocket();
    void InitEpollInstance();
    static void HandleSignals();
    static void SignalHandler(int signum);

    void AcceptNewConnections();
    void ReceiveNewData(int fd);
    void DisconnectClient(int fd);

    void parseInput(int fd, char *buffer);

public:
    Server(uint16_t port = 0) : port_(port), socket_fd_(-1), epoll_fd_(-1), servername("binbinland"), version("beta") {}

    void Init();
    void Run();
    void CloseFds();
};

#endif
