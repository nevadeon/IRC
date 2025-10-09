#include "Server.hpp"

/*
    Format of a server reply:
    [:prefix] (<-osef) code [params] [:trailing]
    Ex: :irc.example.com 001 mynick :Welcome to the IRC Network mynick!~user@host

    -> Up to command to construct the correct code and params
*/
void Server::Reply(int fd, std::string& prefix, const std::string& code, std::vector<std::string>& params)
// void Server::Reply(int sender_fd, int receiver_fd, const char *code, std::vector<std::string>& params, const char *trailing)
{
    std::string str = ":" + prefix + " " + code; //+ params + ":" +  trailing
    for(std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
    {
        str += " ";
        if (*it == params.back())
            str += ":";
        str += *it;
    }
    str += "\r\n";
    // 0 : Aucun flag, comportement standard.
    // MSG_NOSIGNAL : Empêche l’envoi du signal SIGPIPE si le socket est fermé côté client (évite que ton programme crash).
    // MSG_DONTWAIT : Rend l’appel non bloquant (si le socket n’est pas prêt, la fonction retourne immédiatement).
    // MSG_MORE : Indique qu’il y aura d’autres données à envoyer (optimisation TCP).
    send(fd, str.data(), str.length(), 0);
}