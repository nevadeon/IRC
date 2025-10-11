#include "Server.hpp"
#include "ParseUtil.hpp"

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

static std::vector<std::string> parsCommand (std::string str)
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

bool Server::ValidCommand(int fd, const std::string cmd)
{
    std::vector<std::string> errorParams;

    if (cmd.empty()) {
        std::cerr << "<" << fd << ">Internal serveur error" << std::endl;
        return ( false );
    }

    std::map<int, Client>::iterator itClient = this->clients_.find(fd);
    if (itClient == this->clients_.end()) {
        std::cerr << "<" << fd << ">Internal serveur error" << std::endl;
        return ( false );
    }
    
    Client& client = itClient->second;

    // mettre en majuscule les attributs
    std::string uc = cmd;
    for (size_t i = 0; i < uc.size(); ++i)
        uc[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(uc[i])));


    if (sv_commands_.commands.find(uc) == sv_commands_.commands.end()) {
        // commande inconnue : 421
        // 421    ERR_UNKNOWNCOMMAND
        // "<command> :Unknown command"
        // ex : :irc.example.com 421 Alice FLY :Unknown command
        // -> FLY n'existe pas
        errorParams.push_back("MSG_UNKNOWNCOMMAND");
        errorParams.push_back(cmd);
        errorParams.push_back(MSG_UNKNOWNCOMMAND);
        this->Reply(fd, this->info_.servername, std::string("421"), errorParams);
        return ( false );
    }

    if (!client.GetPasswordState()) {
        if (uc == "PASS" || uc == "PING" || uc == "CAP")
            return ( true );
        else {
            // pas connecte : 451
            // 451    ERR_NOTREGISTERED
            // ":You have not registered"
            // ex : :irc.example.com 451 JOIN :You have not registered
            // -> pas encore connecte, tu ne peux pas utiliser JOIN
            errorParams.push_back("MSG_NOTREGISTERED");
            errorParams.push_back(MSG_NOTREGISTERED);
            this->Reply(fd, this->info_.servername, std::string("451"), errorParams);
            return ( false );
        }
        
    }

    if ((!client.GetIfNicknameValidated() || !client.GetUserInfoGiven()) && (uc != "NICK" && uc != "USER")) {
        // pas enregistre : 451 
        // 451 ERR_NOTREGISTERED 
        // ":You have not registered"
        errorParams.push_back("MSG_NOTREGISTERED");
        errorParams.push_back(MSG_NOTREGISTERED);
        this->Reply(fd, this->info_.servername, std::string("451"), errorParams);
        return ( false );
    }

    return ( true );
}


/*
    - Split buffer into Message instances, separated by CLRF (\r\n)
    - Tokenize each submessage and store in Message instance
    - Identify which command is being called and call it
*/
void Server::ParseInput(int fd, const char *buffer)
{
    (void)fd;

    const std::string& delimiter = "\r\n";
    std::vector<std::string> listCommands;
    std::string bufferStr(buffer);
    listCommands = Util::split(bufferStr, delimiter);
    // std::cout << (sv_commands_.commands.find(cmd) != sv_commands_.commands.end()) << std::endl;

    try
    {
        std::vector< std::vector<std::string> >listCommandsToken;
        for(std::vector<std::string>::iterator it = listCommands.begin(); it != listCommands.end(); it++){
            listCommandsToken.push_back(parsCommand(*it));
            std::string cmd = listCommandsToken.back()[0];
            std::cout << listCommandsToken.back()[0] << std::endl;
            std::cout << (sv_commands_.commands.find(cmd) != sv_commands_.commands.end()) << std::endl;

            for(std::vector<std::string>::iterator it2 = listCommandsToken.back().begin(); it2 != listCommandsToken.back().end(); it2++)
                std::cout << listCommandsToken.back()[0] << " : " << *it2 << std::endl;


            if (ValidCommand(fd, cmd))
            {
                if (sv_commands_.commands[cmd](*this, fd, listCommandsToken.back())) {
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
    

    debugPrint(buffer);
}