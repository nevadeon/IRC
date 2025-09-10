#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include <vector>
#include "client.hpp"

class Server
{
private:
    int socket_fd_;
    int epoll_fd_;
    uint16_t port_;
    std::vector<Client> clients_;

    static bool running;

    void InitListeningSocket();
    static void HandleSignals();
    static void SignalHandler(int signum);

public:
    Server(uint16_t port = 0) : port_(port) {}

    void Init();
    void Run();
    void AcceptNewClient();
    void ReceiveNewData(int fd);

    void CloseFds();
    void ClearClient(int fd);
};

#endif
