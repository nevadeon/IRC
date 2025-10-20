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
        params.push_back("PRIVMSG");
        server.Reply(fd, server.info_.servername, ERR_NORECIPIENT, params);
        return (0);
    }
    if (argsSize < 3) {
        // 412     ERR_NOTEXTTOSEND
        // ":No text to send"
        params.push_back(server.clients_[fd].GetNick());
        params.push_back(MSG_NOTEXTTOSEND);
        server.Reply(fd, server.info_.servername, ERR_NOTEXTTOSEND, params);
        return (0);
    }

    if (args[1][0] == '#')
    {
        std::string ch_name = args[1];
        if (!server.channels_.count(ch_name))
        {
            params.push_back(server.clients_[fd].GetNick());
            params.push_back(ch_name.substr(1));
            params.push_back(MSG_NOSUCHNICK);
            server.Reply(fd, server.info_.servername, ERR_NOSUCHNICK, params);
            return (0);
        }

        Channel ch = server.channels_[ch_name];
        if (!ch.GetClients().count(fd))
        {
            params.push_back(server.clients_[fd].GetNick());
            params.push_back(MSG_CANNOTSENDTOCHAN);
            server.Reply(fd, server.info_.servername, ERR_CANNOTSENDTOCHAN, params);
            return (0);
        }

        server.SendToChannel(fd, ch, args[2]);

        std::string msg(args[2]);
        if (msg.substr(0, 5) == ":!bot" || msg.substr(0, 4) == "!bot") {
            server.botReply(ch);
        }
    }
    else
    {
        if ((clientFd = server.FindClient(args[1])) != -1) {
            Client sender = server.clients_[fd];
            std::string info = sender.GetNick() + "!" + sender.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
            params.push_back(server.clients_[clientFd].GetNick());
            params.push_back(args[2]);
            server.Reply(clientFd, info, "PRIVMSG", params);
        } else {
            // 401     ERR_NOSUCHNICK
            // "<nickname> :No such nick/channel"
            params.push_back(server.clients_[fd].GetNick());
            params.push_back(args[1]);
            params.push_back(MSG_NOSUCHNICK);
            server.Reply(fd, server.info_.servername, ERR_NOSUCHNICK, params);
        }
    }
    return (0);
}
