#include "Server.hpp"
#include <time.h>

static const char* insults[] = {
    "Ton arbre genealogique est un cercle",
    "Mangeur de crayons",
    "Mange tes morts meme si t'es vegan",
    "Talon E ta fenetre",
    "R Cho'Gath ton paf",
    "Va jouer sur l'autoroute",
    "Tu es comme forest Gump quand il court pas, tu es juste attardé",
    "Si j'avais su que t'etais aussi nul a chier j'aurais repris la clope. Ca me donne moins le cancer que de te regarder jouer"
};

static const int sizeof_insults = sizeof(insults) / sizeof(char *);

static std::string generateBotMsg()
{
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    std::string msg(insults[rand() % sizeof_insults]);
    return msg;
}

void Server::botReply(Channel& channel)
{
    Client bot = this->GetClients()[BOT_FD];
    std::string info = bot.GetNick() + "!" + bot.GetUserInfo().username + "@" + DUMMY_HOSTNAME;

    std::vector<std::string> params;
    params.push_back(channel.GetName());
    params.push_back(generateBotMsg());

    std::map<int, operator_status>::iterator it = channel.GetClients().begin();
    for (; it != channel.GetClients().end(); it++)
    {
        Reply(it->first, info, "PRIVMSG", params);
    }
}
