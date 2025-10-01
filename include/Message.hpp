#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

class Message
{
    private:
        int sender;
        int receiver;
        std::string content;
        std::string command;

    public:
        Message(int fd, const char *buffer);
};

#endif