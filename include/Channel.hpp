#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>

class Channel
{
    private:
        std::vector<Client *> operators_;
        std::vector<Client *> connected_clients_;
        std::string channel_name_;
    public:
        Channel();
};

#endif