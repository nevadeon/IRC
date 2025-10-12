#include "Channel.hpp"
#include "Server.hpp"

#define NUMBER_OF_SYMBOLS 5

Channel::Channel(std::string& name, Client* founder)
{
    info_.ch_name = name;
    for (int i = 0; i < NUMBER_OF_SYMBOLS; i++)
        modes_[symbols[i]] = false;
    ch_clients_[founder] = IS_OPERATOR;
}

void Channel::SetMode(char c, bool value) { modes_[c] = value; }

void Channel::SetTopic(std::string& str) { info_.ch_topic = str; }

void Channel::SetKey(std::string& str) { info_.ch_key = str; }

void Channel::SetOperator(std::string& nickname) { (void)nickname; }

void Channel::SetLimit(int limit) { info_.ch_user_limit = limit; }

std::string Channel::GetName() { return (this->info_.ch_name); }

std::string Channel::GetKey() { return (this->info_.ch_key); }

std::string Channel::GetTopic() {
    std::string topic = this->info_.ch_topic;
    if (topic.empty())
        return (this->info_.ch_topic);
    return (":");
}


size_t Channel::GetUserLimit() { return (this->info_.ch_user_limit); }

std::map<Client *, operator_status> Channel::GetClients() { return (this->ch_clients_); }

bool Channel::GetModeState(char mode) { return (this->modes_.find(mode)->second); }

bool Channel::IsInvitedClient(Client *client) {
    for(std::vector<Client *>::iterator it = inviteClient_.begin(); it != inviteClient_.end(); it++) {
        if (*it == client) {
            return (true);
        }
    }
    return (false);
}

void Channel::AddClient(Client *client) {
    ch_clients_[client] = IS_NOT_OPERATOR;
}


