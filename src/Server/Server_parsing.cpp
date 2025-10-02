#include "Server.hpp"
#include "ParseUtil.hpp"
#include <Message.hpp>
#include <Commands.hpp>

// a supprimer
// fonction de debug
void debugPrint(const std::string& s) {
    for (size_t i = 0; s[i]; i++)
    {
        switch (s[i]) {
            case '\r': std::cout << "\\r"; break;
            case '\n': std::cout << "\\n"; break;
            default: std::cout << s[i]; break;
        }
    }
    std::cout << std::endl;
}

std::vector<std::string> parsCommand (std::string str)
{
    std::vector<std::string> listArg;
    size_t pos = 0;
    std::string token;
    std::string delimiter = " ";
    while ((pos = str.find(delimiter)) != std::string::npos) {
        if (str[0] == ':') {
            str.erase(0, 1);
            break;
        }
        token = str.substr(0, pos);
        if (token.length() != 0)
            listArg.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    if (str.length() != 0)
        listArg.push_back(str);

    return listArg;
}


/*
    - Split buffer into Message instances, separated by CLRF (\r\n)
    - Tokenize each submessage and store in Message instance
    - Identify which command is being called and call it
*/
void Server::ParseInput(int fd, char *buffer)
{
    (void)fd;

    
    const std::string& delimiter = "\r\n";
    std::vector<std::string> listCommands;
    std::string bufferStr(buffer);
    listCommands = Util::split(bufferStr, delimiter);

    // Message message(fd, listCommands);
    try
    {
        std::vector< std::vector<std::string> >listCommandsToken;
        for(std::vector<std::string>::iterator it = listCommands.begin(); it != listCommands.end(); it++){
            listCommandsToken.push_back(parsCommand(*it));
            std::string cmd = listCommandsToken.back()[0];
            std::cout << listCommandsToken.back()[0] << std::endl;
            std::cout << (Commands::commands.find(cmd) != Commands::commands.end()) << std::endl;

            
            // for(std::vector<std::string>::iterator it2 = listCommandsToken.back().begin(); it2 != listCommandsToken.back().end(); it2++)
            //     std::cout << listCommandsToken.back()[0] << " : " << *it2 << std::endl;

            if (Commands::commands.find(cmd) != Commands::commands.end()) {
                Commands::commands[cmd](*this, fd, listCommandsToken.back()); 
            } else {
                // throw 
            }
        }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        throw e;
    }
    

    debugPrint(buffer);
}