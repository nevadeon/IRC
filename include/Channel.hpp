#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>

/*
    - Boolean values if operator(s) has/have decided to set the option
    - Limit set to -1 if unlimited amount of users can join, else limit is in place
*/
struct s_ch_info
{
    std::pair<bool, std::string> ch_name;
    std::pair<bool, std::string> ch_topic;
    bool ch_invite_only;
    int ch_user_limit;
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
        std::vector<Client *> operators_;
        std::vector<Client *> connected_clients_;
        struct s_ch_info info_;

    public:
        Channel();
};

#endif