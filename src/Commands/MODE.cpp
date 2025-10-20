#include "Server.hpp"
#include <set>
#include <sstream>
// static bool IsAChanOp(Channel* ch, int fd)
// {
//     return (ch->GetClients()[fd] == IS_OPERATOR);
// }

/*
    (+ ou -).(i, o, t, k, l)
*/
// static bool IsKnownModeString(std::string& str)
// {
//     return (str.size() == 2
//         && (str[0] == '+' || str[0] == '-')
//         && (allowed_modes.count(str[1])));
// }
std::map<char, bool> sign_value;

// :<nick>!<user>@<host> MODE #channel +i
void Server::IMode(int fd, Channel& channel, char sign) {
    channel.SetMode('i', sign_value[sign]);

    std::vector<std::string> params;
    Client client = this->GetClients()[fd];

    std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
    params.push_back(channel.GetName());
    params.push_back(std::string(1, sign) + "i");
    std::map<int, operator_status> &clientsMap = channel.GetClients();
    for(std::map<int, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
         this->Reply(it->first, info, "MODE", params);
    }
}

void Server::TMode(int fd, Channel& channel, char sign) {
    channel.SetMode('t', sign_value[sign]);

    std::vector<std::string> params;
    Client client = this->GetClients()[fd];

    std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
    params.push_back(channel.GetName());
    params.push_back(std::string(1, sign) + "t");
    std::map<int, operator_status> &clientsMap = channel.GetClients();
    for(std::map<int, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
         this->Reply(it->first, info, "MODE", params);
    }
}

void Server::OMode(int fd, Channel& channel, char sign, std::vector<std::string>& args) {
    std::vector<std::string> params;
    if (args.size() < 4)
    {
        // 461     ERR_NEEDMOREPARAMS
        // "<command> :Not enough parameters"
        params.push_back("USER");
        params.push_back(MSG_NEEDMOREPARAMS);
        this->Reply(fd, this->info_.servername, (ERR_NEEDMOREPARAMS), params);
        return ;
    }

    int targetFd = this->FindClient(args[3]);
    if (targetFd == -1) {
        // 401     ERR_NOSUCHNICK
        // "<nickname> :No such nick/channel"
        params.push_back(args[3]);
        params.push_back(MSG_NOSUCHNICK);
        this->Reply(fd, this->info_.servername, (ERR_NOSUCHNICK), params);
        return ;
    }
    std::map<int, operator_status> &clientsMap = channel.GetClients();
    if ( clientsMap.find(targetFd) == clientsMap.end()) {
        // 441     ERR_USERNOTINCHANNEL
        // "<nick> <channel> :They aren't on that channel"
        params.push_back(this->GetClients()[targetFd].GetNick());
        params.push_back(channel.GetName());
        params.push_back(MSG_USERNOTINCHANNEL);
        this->Reply(fd, this->info_.servername, (ERR_USERNOTINCHANNEL), params);
        return ;
    }
    
    clientsMap[targetFd] = (operator_status)sign_value[sign];

    Client client = this->GetClients()[fd];
    // std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
    std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
    params.push_back(channel.GetName());
    params.push_back(std::string(1, sign) + "o");
    params.push_back(this->GetClients()[targetFd].GetNick());
    for(std::map<int, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
        this->Reply(it->first, info, "MODE", params);
    }
}

void Server::KMode(int fd, Channel& channel, char sign, std::vector<std::string>& args) {
    std::vector<std::string> params;
    if (sign == '+' && args.size() < 4)
    {
        // 461     ERR_NEEDMOREPARAMS
        // "<command> :Not enough parameters"
        params.push_back("USER");
        params.push_back(MSG_NEEDMOREPARAMS);
        this->Reply(fd, this->info_.servername, (ERR_NEEDMOREPARAMS), params);
        return ;
    }

    channel.SetMode('k', sign_value[sign]);
    if (sign == '+')
        channel.SetKey(args[3]);

    Client client = this->GetClients()[fd];
    std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
    params.push_back(channel.GetName());
    params.push_back(std::string(1, sign) + "k");
    if (sign == '+')
        params.push_back(args[3]);
    std::map<int, operator_status> &clientsMap = channel.GetClients();
    for(std::map<int, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
         this->Reply(it->first, info, "MODE", params);
    }
}

void Server::LMode(int fd, Channel& channel, char sign, std::vector<std::string>& args) {
    std::vector<std::string> params;
    if (sign == '+' && args.size() < 4)
    {
        // 461     ERR_NEEDMOREPARAMS
        // "<command> :Not enough parameters"
        params.push_back("USER");
        params.push_back(MSG_NEEDMOREPARAMS);
        this->Reply(fd, this->info_.servername, (ERR_NEEDMOREPARAMS), params);
        return ;
    }

    channel.SetMode('l', sign_value[sign]);
    if (sign == '+') {
        std::istringstream ss(args[3]);
        int ret;
        ss >> ret;
        channel.SetLimit(ret);
    }

    Client client = this->GetClients()[fd];
    std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
    params.push_back(channel.GetName());
    params.push_back(std::string(1, sign) + "l");
    if (sign == '+')
        params.push_back(args[3]);
    std::map<int, operator_status> &clientsMap = channel.GetClients();
    for(std::map<int, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
         this->Reply(it->first, info, "MODE", params);
    }
}

/*
    ONLY ACCEPT ONE MODESTIRNG
    Args: MODE <target> []
*/
int Server::Commands::MODE(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;
    Client client = server.clients_[fd];

    if (args.size() < 2)
    {
        // 461     ERR_NEEDMOREPARAMS
        // "<command> :Not enough parameters"
        params.push_back("USER");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, (ERR_NEEDMOREPARAMS), params);
        return (0);
    }

    std::string chanName = args[1];
    Channel *channel = server.FindChannel(chanName);
    if (!channel) {
        // 403     ERR_NOSUCHCHANNEL
        // "<channel name> :No such channel"
        params.push_back(server.clients_[fd].GetNick());
        params.push_back(chanName);
        params.push_back(MSG_NOSUCHCHANNEL);
        server.Reply(fd, server.info_.servername, (ERR_NOSUCHCHANNEL), params);
        return (0);
    }

    // :blackhole.boys.com 324 Alice #test +kt truc\r\n
    if (args.size() < 3)
    {
        // 324     RPL_CHANNELMODEIS
        // "<channel> <mode> <mode params>"
        const size_t nbMode = 4;
        const char modeChars[nbMode] = { 'i', 't', 'k', 'l' };
        std::string info = "+";
        for (size_t i = 0; i < nbMode; ++i) {
            if (channel->GetModeState(modeChars[i]))
                info += modeChars[i];
        }
        params.push_back(client.GetNick());
        if (info.size() > 1)
            params.push_back(info);
        if (channel->GetModeState('k'))
            params.push_back(channel->GetKey());
        if (channel->GetModeState('l')) {
            std::stringstream ss;
            ss << channel->GetUserLimit();
            params.push_back(ss.str());
        }
        server.Reply(fd, server.info_.servername, "324", params);
        return (0);
    }

    if (channel->FindClient(client.GetNick(), server) == -1) {
        // 442     ERR_NOTONCHANNEL
        // "<channel> :You're not on that channel"
        params.push_back(chanName);
        params.push_back(MSG_NOTONCHANNEL);
        server.Reply(fd, server.info_.servername, (ERR_NOTONCHANNEL), params);
        return (0);
    }

    if (channel->IsOperator(fd) != IS_OPERATOR)
    {
        // 482     ERR_CHANOPRIVSNEEDED 442 M
        // "<channel> :You're not channel operator"
        params.push_back(chanName);
        params.push_back(MSG_CHANOPRIVSNEEDED);
        server.Reply(fd, server.info_.servername, (ERR_CHANOPRIVSNEEDED), params);
        return (0);
    }


    // :<nick> MODE <channel> <modestring> [params]
    
    // CHANGER TOPIC EN FONCTION DU MODE !!!!!!!!!!!!
    char sign = '|';
    sign_value['-'] = false;
    sign_value['+'] = true;
    std::string modes = args[2];
    for (size_t i = 0; i < modes.size(); i++)
    {
        size_t is_valid = 0;
        char mode = modes[i];
        switch (mode)
        {
            case '+':
                sign = '+';
                break;
            case '-':
                sign = '-';
                break;
            case 'i':
                
                if (sign != '+' && sign != '-')
                    is_valid = 2;
                else {
                    std::cout << "mode i" << std::endl;
                    server.IMode(fd, *channel, sign);
                }
                break;
            case 't':
                if (sign != '+' && sign != '-')
                    is_valid = 2;
                else {
                    std::cout << "mode t" << std::endl;
                    server.TMode(fd, *channel, sign);
                }
                break;
            case 'o':
                if (sign != '+' && sign != '-')
                    is_valid = 2;
                else {
                    std::cout << "mode o" << std::endl;
                    server.OMode(fd, *channel, sign, args);
                }
                break;
            case 'k':
                if (sign != '+' && sign != '-')
                    is_valid = 2;
                else {
                    std::cout << "mode k" << std::endl;
                    server.KMode(fd, *channel, sign, args);
                }
                break;
            case 'l':
                if (sign != '+' && sign != '-')
                    is_valid = 2;
                else {
                    std::cout << "mode l" << std::endl;
                    server.LMode(fd, *channel, sign, args);
                }
                break;
            default:
                is_valid = 1;
                break;
        }
        if (is_valid > 0) {
                // 472     ERR_UNKNOWNMODE
                // "<char> :is unknown mode char to me"
                params.push_back(&mode);
                params.push_back(MSG_UNKNOWNMODE);
                server.Reply(fd, server.info_.servername, (ERR_UNKNOWNMODE), params);
                if (is_valid == 2)
                    return (0);
        }
    }
    return (0);
}

