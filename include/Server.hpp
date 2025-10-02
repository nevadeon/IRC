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
        std::string password_;
        int socket_fd_;
        int epoll_fd_;
        std::map<int, Client> unauthenticated_clients_;
        std::map<int, Client> connected_clients_;
        std::map<std::string, Channel> channels_;
        std::string servername_;
        std::string version_;

        volatile static std::sig_atomic_t running;

        void InitListeningSocket();
        void InitEpollInstance();
        static void HandleSignals();
        static void SignalHandler(int signum);

        void AcceptNewConnections();
        void ReceiveNewData(int fd);

        
        
    public:
        Server(uint16_t port = 0, const char *pass = "") : port_(port), password_(pass), socket_fd_(-1), epoll_fd_(-1), servername_("binbinland"), version_("beta") {}
        
        void Init();
        void Run();
        void CloseFds();
        void ParseInput(int fd, char *buffer);
        void Disconnect(int fd);
        
        std::string& GetPassword();
        void AuthenticateClient(int fd);
        std::map<int, Client>& GetUnauthenticatedClients() { return unauthenticated_clients_; }
        std::map<int, Client>& GetAuthenticatedClients() { return connected_clients_; }

};

#endif
