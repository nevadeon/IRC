#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include <map>
#include <iostream>
#include "Channel.hpp"
#include "Client.hpp"
#include "colors.hpp"

/*
    2 types of connections:
     - the ones who are simply connected to the server
     - the ones who are authenticated
    A client is either in unauthenticated or connected, but not both;
*/
class Server
{
    private:
        uint16_t port_;
        int socket_fd_;
        int epoll_fd_;
        std::map<int, Client> unauthenticated_clients;
        std::map<std::string, Client> connected_clients_;
        std::map<std::string, Channel> channels_;
        std::string servername;
        std::string version;

        volatile static std::sig_atomic_t running;

        void InitListeningSocket();
        void InitEpollInstance();
        static void HandleSignals();
        static void SignalHandler(int signum);

        void AcceptNewConnections();
        void ReceiveNewData(int fd);

        void Disconnect(int fd);
        void DisconnectUnauthenticated(int fd);
        void DisconnectAuthenticated(Client& c);

        Client* FindClientByFD(int fd);

        void parseInput(int fd, char *buffer);

    public:
        Server(uint16_t port = 0) : port_(port), socket_fd_(-1), epoll_fd_(-1), servername("binbinland"), version("beta") {}

        void Init();
        void Run();
        void CloseFds();
};

#endif
