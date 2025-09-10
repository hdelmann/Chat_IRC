#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "../ServerCore.h"
#include "../Message.h"
#include "../User.h"

void cap_handler(User &user, Message const &message, ServerCore &core);
void pass_handler(User &user, Message const &message, ServerCore &core);
void nick_handler(User &user, Message const &message, ServerCore &core);
void user_handler(User &user, Message const &message, ServerCore &core);
void mode_handler(User &user, Message const &message, ServerCore &core);
void join_handler(User &user, Message const &message, ServerCore &core);
void part_handler(User &user, Message const &message, ServerCore &core);
void quit_handler(User &user, Message const &message, ServerCore &core);
void pong_handler(User &user, Message const &message, ServerCore &core);
void ping_handler(User &user, Message const &message, ServerCore &core);
void names_handler(User &user, Message const &message, ServerCore &core);
void kick_handler(User &user, Message const &message, ServerCore &core);
void topic_handler(User &user, Message const &message, ServerCore &core);
void whois_handler(User &user, Message const &message, ServerCore &core);
void privmsg_handler(User &user, Message const &message, ServerCore &core);
void invite_handler(User &user, Message const &message, ServerCore &core);
int	 ft_str_is_numeric(char *str);
#endif
