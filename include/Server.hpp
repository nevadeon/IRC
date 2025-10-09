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

// Error messages
#define ERR_UNKNOWNCOMMAND "Unknown command"
#define ERR_NOTREGISTERED "You have not registered"
#define ERR_NICKNAMEINUSE "Nickname is already in use"
#define ERR_ERRONEUSNICKNAME "Erroneus nickname"
#define ERR_NEEDMOREPARAMS "Not enough parameters"
#define ERR_ALREADYREGISTRED "You may not reregister"
#define ERR_PASSWDMISMATCH "Password incorrect"
#define ERR_NONICKNAMEGIVEN "No nickname given"


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
                static int CAP(Server& serv, int fd, std::vector<std::string>& args);
                static int PASS(Server& serv, int fd, std::vector<std::string>& args);
                static int USER(Server& serv, int fd, std::vector<std::string>& args);
                static int PING(Server& serv, int fd, std::vector<std::string>& args);
                static int PRIVMSG(Server& serv, int fd, std::vector<std::string>& args);
                static int NICK(Server& serv, int fd, std::vector<std::string>& args);
                static int KICK(Server& serv, int fd, std::vector<std::string>& args);
                static int INVITE(Server& serv, int fd, std::vector<std::string>& args);
                static int JOIN(Server& serv, int fd, std::vector<std::string>& args);
                static int PART(Server& serv, int fd, std::vector<std::string>& args);
                static int QUIT(Server& serv, int fd, std::vector<std::string>& args);
                static int TOPIC(Server& serv, int fd, std::vector<std::string>& args);
                static int MODE(Server& server, int fd, std::vector<std::string>& args);
                
            public:
                std::map<std::string, int(*)(Server&, int, std::vector<std::string>&)> commands;     
        };

        Commands sv_commands_;
        std::map<int, Client> clients_;
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
        void ParseInput(int fd, const char *buffer);
        // void Reply(int sender_fd, int receiver_fd, const char *code, std::vector<std::string>& params, const char *trailing);
        void Reply(int fd, std::string& prefix, const std::string& code, std::vector<std::string>& params);

        bool IsNicknameAlreadyUsed(std::string& nickname);
        
    public:
        Server(uint16_t port = 0, const char *pass = "") : port_(port), socket_fd_(-1), epoll_fd_(-1) { info_.password = std::string(pass); }
        
        void Init();
        void Run();
        void CloseFds();
        bool ValidCommand(int fd, const std::string cmd);
        
        std::string& GetPassword();
        std::map<int, Client>& GetClients() { return clients_; }

};

#endif
