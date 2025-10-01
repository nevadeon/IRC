#include "Server.hpp"

/*
    - Split buffer into Message instances, separated by CLRF (\r\n)
    - Tokenize each submessage and store in Message instance
    - Identify which command is being called and call it
*/
void Server::ParseInput(int fd, char *buffer)
{
    (void)fd;
    for (int i = 0; buffer[i]; i++)
    {
        if (buffer[i] && buffer[i + 1] && buffer[i] == '\r' && buffer[i + 1] == '\n')
            std::cout << "sheesh\n";
    }
}