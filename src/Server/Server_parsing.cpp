#include "Server.hpp"
#include "ParseUtil.hpp"
#include <Message.hpp>

// a supprimer
// fonction de debug
void debugPrint(const std::string& s) {
    for (unsigned char c : s) {
        switch (c) {
            case '\r': std::cout << "\\r"; break;
            case '\n': std::cout << "\\n"; break;
            default: std::cout << c; break;
        }
    }
    std::cout << std::endl;
}

/*
    - Split buffer into Message instances, separated by CLRF (\r\n)
    - Tokenize each submessage and store in Message instance
    - Identify which command is being called and call it
*/
void Server::ParseInput(int fd, char *buffer)
{
    (void)fd;
    // for (int i = 0; buffer[i]; i++)
    // {
    //     if (buffer[i] && buffer[i + 1] && buffer[i] == '\r' && buffer[i + 1] == '\n')
    //     std::cout << "sheesh\n";
    // }

    // std::cout << "commandes : " << buffer << "|Fin|" << std::endl;

    
    const std::string& delimiter = "\r\n";
    std::vector<std::string> listCommands;
    std::string bufferStr(buffer);
    listCommands = Util::split(bufferStr, delimiter);
    // list :
    for(std::vector<std::string>::iterator it = listCommands.begin(); it != listCommands.end(); it++){
        std::cout << "|debut test|" << *it << "|fin test|" << std::endl;
	}

    Message message(fd, listCommands);


    debugPrint(buffer);
}