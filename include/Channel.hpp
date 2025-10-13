#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include "Client.hpp"
// #include "Server.hpp"
//  déclaration anticipée pour eviter les pb lors de la compilation
class Server;
#include <string>
#include <vector>


const char symbols[] = {'i', 't', 'k', 'o', 'l'};

typedef enum e_operator_status
{
    IS_NOT_OPERATOR = 0,
    IS_OPERATOR = 1
} operator_status ;

/*
    - Boolean values if operator(s) has/have decided to set the option
    - Limit set to -1 if unlimited amount of users can join, else limit is in place
*/
struct s_ch_info
{
    std::string ch_name;
    std::string ch_topic;
    std::string ch_key;
    size_t ch_user_limit;
};

/*
    .Channel name:
    - First character is # or &
    - No commas
    - No ASCII 7
    - No spaces
    - Length of up to 200 characters

    .Channel creation:
    - Doesnt exist: User JOINs channel, automatically becomes channel operator.
    - Exists: Depends on channel mode

    .Operators:
    - Have @ prefix 
*/
class Channel
{
    private:
        struct s_ch_info info_;
        std::map<char, bool> modes_;
        std::map<int, operator_status> ch_clients_;
        std::vector<int> inviteClient_;

    public:
        Channel(std::string& name, int fd_founder);
        Channel();
        ~Channel();
        
        void SetMode(char c, bool mode);
        void SetTopic(std::string& str);
        void SetKey(std::string& str);
        void SetOperator(std::string& nickname);
        void SetLimit(int limit);
        std::string GetName();
        std::string GetKey();
        std::string GetTopic();
        size_t GetUserLimit();
        std::map<int, operator_status> &GetClients();
        bool GetModeState(char mode);
        bool IsInvitedClient(int fd);
        void AddClient(int fd);
        int FindClient(std::string nickname, Server server);
        int IsOperator(int fd);
};

#endif