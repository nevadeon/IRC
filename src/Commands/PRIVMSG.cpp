#include "Server.hpp"

// 401 ERR_NOSUCHNICK : destinataire inconnu.
// 411 ERR_NORECIPIENT : pas de destinataire.
// 412 ERR_NOTEXTTOSEND : pas de texte à envoyer.
// 404 ERR_CANNOTSENDTOCHAN : impossible d’envoyer sur ce canal (par exemple, mode +n).
// :<prefix> PRIVMSG <receveur> :<texte>
// exemple:
// PRIVMSG Bob :Salut Bob !
// PRIVMSG #canal :Bonjour à tous !

int Server::Commands::PRIVMSG(Server& server, int fd, std::vector<std::string>& args)
{
    int argsSize = args.size();
    std::vector<std::string> params;
    int clientFd;

    if (argsSize < 2) {
        // 411     ERR_NORECIPIENT
        // ":No recipient given (<command>)"
        params.push_back(server.clients_[fd].GetNick());
        params.push_back(MSG_NORECIPIENT("PRIVMSG"));
        server.Reply(fd, server.info_.servername, std::string(ERR_NORECIPIENT), params);
        return (0);
    }
    if (argsSize < 3) {
        // 412     ERR_NOTEXTTOSEND
        // ":No text to send"
        params.push_back(server.clients_[fd].GetNick());
        params.push_back(MSG_NOTEXTTOSEND);
        server.Reply(fd, server.info_.servername, std::string(ERR_NOTEXTTOSEND), params);
        return (0);
    }

    if (args[1][0] == '#' || args[1][0] == '&') {
        // gestion du chanel
        // a gerer :
        // 404 ERR_CANNOTSENDTOCHAN : impossible d’envoyer sur ce canal (par exemple, mode +n)
        // + 401 ERR_NOSUCHNICK

    } else {
        if ((clientFd = server.FindClient(args[1])) != -1) {
            params.push_back(args[2]);
            server.Reply(clientFd, server.clients_[fd].GetNick(), server.clients_[clientFd].GetNick(), params);
        } else {
            // 401     ERR_NOSUCHNICK
            // "<nickname> :No such nick/channel"
            params.push_back(server.clients_[fd].GetNick());
            params.push_back(MSG_NOSUCHNICK);
            server.Reply(fd, server.info_.servername, std::string(ERR_NOSUCHNICK), params);
        }
    }
    return (0);
}