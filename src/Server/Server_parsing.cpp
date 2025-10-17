#include "Server.hpp"
#include "ParseUtil.hpp"

// a supprimer
// fonction de debug
void debugPrint(const std::string& s)
{
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

/*
    Checks if server command
*/
bool Server::CommandExists(int fd, const std::string& cmd)
{
    std::vector<std::string> errorParams;

    if (cmd.empty()) {
        std::cerr << "<" << fd << ">Internal serveur error" << std::endl;
        return ( false );
    }

    if (sv_commands_.commands.find(cmd) == sv_commands_.commands.end()) {
        //ERR_UNKNOWNCOMMAND
        errorParams.push_back(clients_[fd].GetNick());
        errorParams.push_back(cmd);
        errorParams.push_back(MSG_UNKNOWNCOMMAND);
        Reply(fd, info_.servername, ERR_UNKNOWNCOMMAND, errorParams);
        return ( false );
    }
    return ( true );
}

/*
    -> If not registered, only allow certain commands
    -> If registered, prevent resending authentification commands
*/
bool Server::RegistrationCheck(int fd, const std::string& cmd)
{
    Client client = clients_[fd];
    std::vector<std::string> errorParams;

    if (!client.GetPasswordState()
        && !(cmd == "PASS" || cmd == "PING" || cmd == "CAP"))
    {
            //ERR_NOTREGISTERED
            errorParams.push_back(client.GetNick());
            errorParams.push_back(MSG_NOTREGISTERED);
            Reply(fd, info_.servername, ERR_NOTREGISTERED, errorParams);
            return ( false );
    }

    if ((client.GetPasswordState() && cmd == "PASS")
        || (client.GetUserInfoGiven() && cmd == "USER"))
    {
        //ERR_ALREADYREGISTERED
        errorParams.push_back(MSG_ALREADYREGISTRED);
        Reply(fd, info_.servername, ERR_ALREADYREGISTRED, errorParams);
        return ( false );
    }
    return true;
}


/*
    - Split buffer into Message instances, separated by CLRF (\r\n)
    - Tokenize each submessage and store in Message instance
    - Identify which command is being called and call it
*/
void Server::ParseInput(int fd, const char *buffer)
{
    debugPrint(buffer);

    const std::string& delimiter = "\r\n";
    std::vector<std::string> listCommands;
    std::string bufferStr(buffer);
    listCommands = Util::split(bufferStr, delimiter);
    // commande en majuscule :
    std::string cmdUpperCase;

    try
    {
        std::vector< std::vector<std::string> >listCommandsToken;
        for(std::vector<std::string>::iterator it = listCommands.begin(); it != listCommands.end(); it++){
            listCommandsToken.push_back(Util::parseCommand(*it));
            std::string cmd = listCommandsToken.back()[0];
            cmdUpperCase = cmd;
            for (size_t i = 0; i < cmdUpperCase.size(); ++i)
                cmdUpperCase[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(cmdUpperCase[i])));

            if (CommandExists(fd, cmdUpperCase) && RegistrationCheck(fd, cmdUpperCase))
            {
                if (sv_commands_.commands[cmdUpperCase](*this, fd, listCommandsToken.back())) {
                    std::cerr << "<" << fd << ">Internal serveur error" << std::endl;
                }
            } else {
                std::cerr << "Invalid command" << std::endl;
            }

        }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw e;
    }

}
