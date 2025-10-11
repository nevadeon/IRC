#include "Channel.hpp"
#include <string>

#define NUMBER_OF_SYMBOLS 5

Channel::Channel(std::string& name, Client* founder)
{
    info_.ch_name = "#" + name;
    for (int i = 0; i < NUMBER_OF_SYMBOLS; i++)
        modes_[symbols[i]] = false;
    ch_clients_[founder] = IS_OPERATOR;
}

void Channel::SetMode(char c, bool value) { modes_[c] = value; }

void Channel::SetTopic(std::string& str) { info_.ch_topic = str; }

void Channel::SetKey(std::string& str) { info_.ch_key = str; }

void Channel::SetOperator(std::string& nickname) { (void)nickname; }

void Channel::SetLimit(int limit) { info_.ch_user_limit = limit; }