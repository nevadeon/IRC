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
#include <vector>
#include <iostream>
#include "Channel.hpp"
#include "Client.hpp"
#include <ctime>

#define RED "\001\033[31m\002"
#define GREEN "\001\033[1;32m\002"
#define PURPLE "\001\033[1;35m\002"
#define BROWN "\001\033[1;33m\002"
#define WHITE "\001\033[0;37m\002"
#define YELLOW "\001\033[1;33m\002"
#define RESET "\001\033[0m\002"

#define DUMMY_HOSTNAME "42"

// Numeric replies
#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"
#define RPL_ISUPPORT "005"
#define RPL_MOTD "372"
#define ERR_UNKNOWNCOMMAND "421"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
#define ERR_NOTREGISTERED "451"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTRED "462"
#define ERR_PASSWDMISMATCH "464"
#define ERR_NORECIPIENT "411"
#define ERR_NOTEXTTOSEND "412"
#define ERR_NOSUCHNICK "401"
#define ERR_BADCHANMASK "476"
#define ERR_INVITEONLYCHAN "473"
#define ERR_BADCHANNELKEY "475"
#define ERR_CHANNELISFULL "471"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_NOTONCHANNEL "442"
#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_USERONCHANNEL "443"


// Error messages
#define MSG_UNKNOWNCOMMAND "Unknown command"
#define MSG_NOTREGISTERED "You have not registered"
#define MSG_NICKNAMEINUSE "Nickname is already in use"
#define MSG_ERRONEUSNICKNAME "Erroneus nickname"
#define MSG_NEEDMOREPARAMS "Not enough parameters"
#define MSG_ALREADYREGISTRED "You may not reregister"
#define MSG_PASSWDMISMATCH "Password incorrect"
#define MSG_NONICKNAMEGIVEN "No nickname given"
#define MSG_NORECIPIENT(command) "No recipient given ("command")"
#define MSG_NOTEXTTOSEND "No text to send"
#define MSG_NOSUCHNICK "No such nick/channel"
#define MSG_BADCHANMASK "Bad Channel Mask"
#define MSG_INVITEONLYCHAN "Cannot join channel (+i)"
#define MSG_BADCHANNELKEY "Cannot join channel (+k)"
#define MSG_CHANNELISFULL "Cannot join channel (+l)"
#define MSG_NOSUCHCHANNEL "No such channel"
#define MSG_NOTONCHANNEL "You're not on that channel"
#define MSG_CHANOPRIVSNEEDED "You're not channel operator"
#define MSG_USERNOTINCHANNEL "They aren't on that channel"
#define MSG_USERONCHANNEL "is already on channel"

#define SERVERNAME "blackhole.boys.com"
#define REALNAME "Blackhole Boys"
#define VERSION "dev"

struct s_sv_info
{
    std::string servername;
    std::string realname;
    std::string version;
    std::string password;
    std::tm creation_time;

    s_sv_info()
    {
        servername = SERVERNAME;
        realname = REALNAME;
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
        bool CommandExists(int fd, const std::string& cmd);
        bool RegistrationCheck(int fd, const std::string& cmd);

        void Reply(int fd, std::string& prefix, const std::string& code, std::vector<std::string>& params);
        void Notify(std::string& prefix, const std::string& code, std::vector<std::string>& params);
        void Welcome(int fd);

        bool IsNicknameAlreadyUsed(std::string& nickname);
    public:
        Server(uint16_t port, std::string& pass);
        
        void Init();
        void Run();
        void CloseFds();
        
        std::string& GetPassword();
        std::map<int, Client>& GetClients() { return clients_; }
        int FindClient(std::string nickname);
        Channel *FindChanel(std::string name);
        static void WelcomeChanel(Server server, int fd, Channel &channel, Client client);

};

#endif
