#ifndef COMMANDS_HPP
#define COMMANDS_HPP

class Commands
{
    private:
        Commands();
    public:

}

// CAP LS - list server capabilities (can be ignored?)
// NICK - set/change nickname
// PASS - authenticate with password
// USER - set user info
// PING - ping server
// PONG - server response to client
// PRIVMSG - private message (# pour channel, sans # pour user)
// KICK - Eject a client from the channel
// INVITE - Invite a client to a channel
// JOIN - join channel
// PART - quit channel
// QUIT - leave server
// TOPIC - Change or view the channel topic
// MODE - Change the channel’s mode:
// · i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// . l:  Limit max number of users in channel

// TODO: REFACTOR INTO MAP (e.g commands["KICK"] = &kick)
// fonctions prenant des parametres :
// ajouter une commande "help ?"
// int kike(const char *argv);
// int invite(const char *argv);
// int topic(const char *argv);
// int mode(const char *argv);
// int join(const char *argv);
// int msg(const char *argv);

//
// static int (*commandes[])(const char *argv) = {
//     &kike,
//     &invite,
//     &topic,
//     &mode,
//     &join, // join chanel
//     &msg, // send message
// };

// int kike(const char *argv)
// {
//     (void)argv;
//     return ( 0 );
// }

// int invite(const char *argv)
// {
//     (void)argv;
//     return ( 0 );
// }

// int topic(const char *argv)
// {
//     (void)argv;
//     return ( 0 );

// }
// int mode(const char *argv)
// {
//     (void)argv;
//     return ( 0 );
// }

// int join(const char *argv)
// {
//     (void)argv;
//     return ( 0 );
// }

// int msg(const char *argv)
// {
//     (void)argv;
//     return ( 0 );
// }

#endif