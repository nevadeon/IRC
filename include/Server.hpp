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

#define RED "\001\033[31m\002"
#define GREEN "\001\033[1;32m\002"
#define PURPLE "\001\033[1;35m\002"
#define BROWN "\001\033[1;33m\002"
#define WHITE "\001\033[0;37m\002"
#define YELLOW "\001\033[1;33m\002"
#define RESET "\001\033[0m\002"

#define SERVERNAME "Blackhole Boys"
#define VERSION "beta"

struct s_sv_info
{
    std::string name;
    std::string version;
    std::string password;

    s_sv_info()
    {
        name = SERVERNAME;
        version = VERSION;
        password = "undefined";
    }
};

/*
    2 types of connections:
     - the ones who are simply connected to the server
     - the ones who are authenticated
    A client is either in unauthenticated or connected, but not both;
*/
class Server
{
    private:
        class Commands
        {
            private:
                friend class Server;
                Commands() { InitCommands(); };
                void InitCommands();
                static int cap(Server& serv, int fd, std::vector<std::string>& args);
                static int pass(Server& serv, int fd, std::vector<std::string>& args);
                static int user(Server& serv, int fd, std::vector<std::string>& args);
                static int ping(Server& serv, int fd, std::vector<std::string>& args);
                static int privmsg(Server& serv, int fd, std::vector<std::string>& args);
                static int nick(Server& serv, int fd, std::vector<std::string>& args);
                static int kick(Server& serv, int fd, std::vector<std::string>& args);
                static int invite(Server& serv, int fd, std::vector<std::string>& args);
                static int join(Server& serv, int fd, std::vector<std::string>& args);
                static int part(Server& serv, int fd, std::vector<std::string>& args);
                static int quit(Server& serv, int fd, std::vector<std::string>& args);
                static int topic(Server& serv, int fd, std::vector<std::string>& args);
                static int mode(Server& server, int fd, std::vector<std::string>& args);
                
            public:
                std::map<std::string, int(*)(Server&, int, std::vector<std::string>&)> commands;     
        };

        Commands sv_commands_;
        std::map<int, Client> unauthenticated_clients_;
        std::map<int, Client> connected_clients_;
        std::map<std::string, Channel> channels_;
        uint16_t port_;
        int socket_fd_;
        int epoll_fd_;
        struct s_sv_info info_;

        volatile static std::sig_atomic_t running;

        void InitListeningSocket();
        void InitEpollInstance();
        static void HandleSignals();
        static void SignalHandler(int signum);

        void AcceptNewConnections();
        void ReceiveNewData(int fd);
        void AuthenticateClient(int fd);
        void Disconnect(int fd);
        void ParseInput(int fd, char *buffer);
        void Reply(int sender_fd, int receiver_fd, const char *code, const char *params, const char *trailing);
        
    public:
        Server(uint16_t port = 0, const char *pass = "") : port_(port), socket_fd_(-1), epoll_fd_(-1) { info_.password = std::string(pass); }
        
        void Init();
        void Run();
        void CloseFds();
        
        std::string& GetPassword();
        std::map<int, Client>& GetUnauthenticatedClients() { return unauthenticated_clients_; }
        std::map<int, Client>& GetAuthenticatedClients() { return connected_clients_; }

};

#endif
