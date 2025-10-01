#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>

class Message
{
    private:
        int sender_; //socket fd
        std::string receiver_; //nickname
        std::vector<std::string> content;
        std::string command;

    public:
        Message(int fd);
};

Message::Message(int fd) : sender_(fd) {}

#endif