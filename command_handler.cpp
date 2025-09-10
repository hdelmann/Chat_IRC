#include <iostream>
#include <string>
#include <unistd.h>
#include "command_handler.h"
#include "commands/commands.h"
#include "message_builder.h"

struct command_table
{
    std::string cmd;
    User::user_state_t state_needed;
    int min_arg;
    int max_arg;
    void (*handle)(User &, Message const &, ServerCore &);

};

struct command_table static const command_table[] = {
    {"PASS",    (User::user_state_t)-1, 1, 1,  pass_handler},
    {"NICK",    (User::user_state_t)-1, 1, 1,  nick_handler},
    {"USER",    (User::user_state_t)-1, 4, 4,  user_handler},
    {"JOIN",    User::CONNECTED,        1, -1, join_handler},
    {"PART",    User::CONNECTED,        1, 2,  part_handler},
    {"PRIVMSG", User::CONNECTED,        1,-1,  privmsg_handler},
    {"PING",    (User::user_state_t)-1, 1, 2,  ping_handler},
    {"PONG",    (User::user_state_t)-1, 0, 2,  pong_handler},
    {"TOPIC",   (User::CONNECTED),      1, 2,  topic_handler},
    {"KICK",    User::CONNECTED,        1, -1, kick_handler},
    {"MODE",    (User::user_state_t)-1,        1, -1, mode_handler},
    {"INVITE",  (User::CONNECTED),      2, 2,  invite_handler},
    {"CAP",     (User::user_state_t)-1,      0, -1, cap_handler},
    {"WHOIS",   (User::CONNECTED),      0, 2,  whois_handler},
    {"QUIT",    (User::CONNECTED),      1, 2,  quit_handler}
 };

int check_command(User &user, Message const &message, int i, ServerCore &core)
{
    (void)core;
    std::vector<std::string> const &vector = message.get_params();
    if (!user.is_state(command_table[i].state_needed) && command_table[i].state_needed != (User::user_state_t)-1)
    {
        return (0);
    }
    if ((int)vector.size() < command_table[i].min_arg)
    {
        return (2);
    }
    if ((ssize_t)(vector.size()) > (ssize_t)command_table[i].max_arg && (ssize_t)command_table[i].max_arg >= 0)
    {
        return (0);
    }
    return (1);
}

void handle_command(User &user, Message const &message, ServerCore &core)
{
    for (unsigned long index = 0; index < sizeof(command_table) / sizeof(command_table[0]); index++)
    {
        if (message.get_command() == command_table[index].cmd && check_command(user, message, index, core) == 1)
        {
             if (message.get_command() != "PING" && message.get_command() != "PONG" && message.get_command() != "WHOIS")
                user.set_idle();
            command_table[index].handle(user, message, core);
            return;
        }
        else if (message.get_command() == command_table[index].cmd && check_command(user, message, index, core) == 2)
        {
            user.send_message(bld_err_needmoreparams(message.get_command(), user), false);
            return;
        }
    }
    user.send_message(bld_err_unknowncmd(message.get_command()));
}
