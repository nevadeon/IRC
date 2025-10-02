#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <string>
#include <map>
#include "Client.hpp"
#include "Server.hpp"

// Probably will need to change this eventually
class Commands
{
    private:
        Commands();
        
    public:
        static void InitCommands();
        static int cap(Server& serv, Client& sender, std::vector<std::string>& args);
        static int pass(Server& serv, Client& sender, std::vector<std::string>& args);
        static int user(Server& serv, Client& sender, std::vector<std::string>& args);
        static int ping(Server& serv, Client& sender, std::vector<std::string>& args);
        static int privmsg(Server& serv, Client& sender, std::vector<std::string>& args);
        static int nick(Server& serv, Client& sender, std::vector<std::string>& args);
        static int kick(Server& serv, Client& sender, std::vector<std::string>& args);
        static int invite(Server& serv, Client& sender, std::vector<std::string>& args);
        static int join(Server& serv, Client& sender, std::vector<std::string>& args);
        static int part(Server& serv, Client& sender, std::vector<std::string>& args);
        static int quit(Server& serv, Client& sender, std::vector<std::string>& args);
        static int topic(Server& serv, Client& sender, std::vector<std::string>& args);
        static int mode(Server& server, Client& sender, std::vector<std::string>& args);

        static std::map<std::string, int(*)(Server&, Client&, std::vector<std::string>&)> commands;
        
};

// CAP LS - list server capabilities (can be ignored?)
// NICK - set/change nickname
// PASS - authenticate with password
// USER - set user info
// PING - ping server
// PONG - server response to client
// PRIVMSG - private message (# pour channel, sans # pour user)
// KICK - Eject a client from the channel
// INVITE - Invite a client to a channel
// JOIN - join channel
// PART - quit channel
// QUIT - leave server
// TOPIC - Change or view the channel topic
// MODE - Change the channel’s mode:
// · i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// . l:  Limit max number of users in channel

// TODO: REFACTOR INTO MAP (e.g commands["KICK"] = &kick)
// fonctions prenant des parametres :
// ajouter une commande "help ?"
// int kike(const char *argv);
// int invite(const char *argv);
// int topic(const char *argv);
// int mode(const char *argv);
// int join(const char *argv);
// int msg(const char *argv);

#endif