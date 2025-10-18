#include "Channel.hpp"
#include "Server.hpp"

#define NUMBER_OF_SYMBOLS 5

Channel::Channel(std::string& name, int fd_founder)
{
    info_.ch_name = name;
    for (int i = 0; i < NUMBER_OF_SYMBOLS; i++)
        modes_[symbols[i]] = false;
    ch_clients_[fd_founder] = IS_OPERATOR;
}

Channel::Channel() {}

Channel::~Channel() {}


void Channel::SetMode(char c, bool value) { modes_[c] = value; }

void Channel::SetTopic(std::string& str) { info_.ch_topic = str; }

void Channel::SetKey(std::string& str) { info_.ch_key = str; }

void Channel::SetOperator(std::string& nickname) { (void)nickname; }

void Channel::SetLimit(int limit) { info_.ch_user_limit = limit; }

std::string Channel::GetName() { return (this->info_.ch_name); }

std::string Channel::GetKey() { return (this->info_.ch_key); }

std::string Channel::GetTopic() {
    std::string topic = this->info_.ch_topic;
    if (!topic.empty())
        return (this->info_.ch_topic);
    return (":");
}


size_t Channel::GetUserLimit() { return (this->info_.ch_user_limit); }

std::map<int, operator_status> &Channel::GetClients() { return (this->ch_clients_); }

bool Channel::GetModeState(char mode) { return (this->modes_.find(mode)->second); }

bool Channel::IsInvitedClient(int fd) {
    for(std::set<int>::iterator it = inviteClient_.begin(); it != inviteClient_.end(); it++) {
        if (*it == fd) {
            return (true);
        }
    }
    return (false);
}

void Channel::Invite(int fd) {
    inviteClient_.insert(fd);
}


void Channel::AddClient(int fd) {
    ch_clients_[fd] = IS_NOT_OPERATOR;
}

int Channel::FindClient(std::string &nickname, Server &server) {
    for(std::map<int, operator_status>::iterator it = ch_clients_.begin(); it != ch_clients_.end(); it++) {
        if (server.GetClients()[it->first].GetNick() == nickname) {
            return (it->first);
        }
    }
    return (-1);
}

int Channel::IsOperator(int fd) {
    for(std::map<int, operator_status>::iterator it = ch_clients_.begin(); it != ch_clients_.end(); it++) {
        if (it->first == fd) {
            return (it->second);
        }
    }
    return (-1);
}


void Channel::EraseInvitation(int fd) {
    inviteClient_.erase(fd);
}
