#ifndef REPLY_HPP
#define REPLY_HPP

#include <string>
#include <vector>
#include "Client.hpp"

class Reply
{
    private:
        char *code;
        int sender_fd_;
        int receiver_fd_;
        std::vector<std::string> args_; // Last argument should be one after :

    public:
        

Reply::Reply(char *code, int sender_fd, int receiver_fd, std::vector<std::string>& args)
{

}

#endif