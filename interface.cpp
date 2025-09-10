#include <cstdio>
#include <iostream>

#include "User.h"
#include "UserManager.h"
#include "ServerCore.h"
#include "message_builder.h"


#define SUS "Welcome to Jugo's IRC Server"

#define BADPASS "Bad password"

#define NOPASS "NO PASSWORD WTF VRO"

const std::string bld_rpl_welcome(const User &user) {
	return "001 " + user.get_nickname() + " :" + SUS + "\r\n";
}

const std::string bld_rpl_umodeis(const User &user) {
	return "221 " + user.get_nickname() + " " + user.get_mode() + "\r\n";
}

const std::string bld_rpl_currentmodestateusr(const User &user, std::string mode){
    return ":" + user.get_nickname() + " MODE " + user.get_nickname() + " :" + mode + "\r\n";
}

const std::string bld_rpl_topic(const Channel &chan, const std::string &nickname) {
    return "332 " + nickname  + " " + chan.get_name() + " :" + chan.get_topic() + "\r\n";
}

const std::string bld_rpl_notopic(const Channel &chan) {
    return "331 " + chan.get_name() + " :No topic set\r\n";
}

const std::string bld_rpl_whoisuser(const User &user) {
    return "311 " + user.get_nickname() + " " + user.get_nickname() + " " + user.get_username() + " " + user.get_hostname() + " * :" + user.get_realname() + "\r\n";
}

const std::string bld_rpl_whoisoperator(const User &user) {
    return "313 " + user.get_nickname() + " :is an IRC operator\r\n"; 
}

const std::string bld_rpl_endofwhois(const User &user) {
    return "318 " + user.get_nickname() + " "  + user.get_nickname() + " :End of /WHOIS list\r\n";
}


const std::string bld_rpl_topic_changer(const Channel &chan){
    char a[255];
    sprintf(a,"%ld",chan.get_t_changed_at());
    return "333 " + chan.get_topic_changer() + " " + chan.get_name() + " " + chan.get_topic_changer_prefix() + " :" + a + "\r\n";
} 

const std::string bld_rpl_userstatus(const User &user, Channel &chan){
    std::string str = "";
    if (chan.get_flag(MODE_i))
        str += "i";
    if (chan.get_flag(MODE_t))
        str += "t";
    if (chan.get_flag(MODE_k))
        str += "k";
    if (chan.get_flag(MODE_l))
        str += "l";
    
    return "324 " + user.get_nickname() + " " + chan.get_name() + " +" + str +"\r\n";
}

const std::string bld_rpl_namreply(const Channel &chan, const User &user) {
	return "353 " + user.get_nickname() + " @ " + chan.get_name() + " :" + chan.get_username_list("@") + "\r\n";
}

const std::string bld_rpl_invite(const User &user, const Channel &chan)
{
    return "341 " + chan.get_name() + " " + user.get_nickname() + " :" + chan.get_name() + "\r\n";
}
 
const std::string bld_rpl_modechg(const User &user, const Channel &chan, const std::string mchanges) {
    return ":" + user.get_prefix() + " MODE " + chan.get_name() + " " + mchanges + "\r\n";
}

const std::string bld_rpl_invite_msg(const User &user, const Channel &chan)
{
    return  "\033[1;37m" + user.get_nickname() + "\033[0;37m"+ " invites you to " "\033[1;37m" + chan.get_name() + "\r\n";
}


const std::string bld_rpl_endofnames(const Channel &chan, User &user) {
    if (chan.is_user_OP(user.get_nickname()))
    {
	    return "366 " + user.get_nickname() + " " + chan.get_name() + " :End of /NAMES list." + "\r\n";
    }
    
	return "366 " + user.get_nickname() + " " + chan.get_name() + " :End of /NAMES list." + "\r\n";
}

const std::string bld_rpl_whoisidle(const User &user) {
    char tmp[10] = {0};
    sprintf(tmp, "%ld", user.get_idle());
    return "317 " + user.get_nickname() + " " + user.get_username() + " " + tmp + " :seconds idle\r\n";
}

#include <iostream>
const std::string bld_join_msg(const User &user, const Channel &chan) {
    return  ":" + user.get_prefix() + " JOIN :" + chan.get_name()  + "\r\n";
}

const std::string bld_rpl_topic_msg(const User &user, const Channel &chan, bool i) {
    if (i == false)
    {
        return ":" +user.get_prefix() + " TOPIC " + chan.get_name() + "\r\n";
    }
    return ":" +user.get_prefix() + " TOPIC " + chan.get_name() + " :" + chan.get_topic() + "\r\n";
}
const std::string bld_privmsg_msg(const User &user, const Channel &chan, const std::string &message) {
    if (chan.is_user_OP(user.get_nickname()) == 1)
        return ":@" + user.get_prefix() + " PRIVMSG " + chan.get_name() + " :" + message + "\r\n";
    else
        return ":" + user.get_prefix() + " PRIVMSG " + chan.get_name() + " :" + message + "\r\n";
}

const std::string bld_privmsg_msg(const User &user, const User &target, const std::string &message) {
    return ":" + user.get_prefix() + " PRIVMSG " + target.get_nickname() + " :" + message + "\r\n";
}

const std::string bld_part_msg(const User &user, const Channel &chan, const std::string &message) {
    return ":" + user.get_prefix() + " PART " + chan.get_name() + " " + message + "\r\n";
}

const std::string bld_nick_msg(const User &user, const std::string &nick) {
    return ":" + user.get_nickname() + " NICK " + nick + "\r\n";
}

const std::string bld_ping_msg(const User &user) {
    return "PING :" + user.get_servername() + "\r\n";
}

const std::string bld_pong_msg(const User &user) {
	return ":" + user.get_servername() + " PONG\r\n";
}

const std::string bld_kick_msg(const User &oper, const User &target, const Channel &chan, const std::string reason) {
    if (reason != "")
    	return ":" + oper.get_nickname() + " KICK " + chan.get_name() + " " + target.get_nickname() + " :" + reason + "\r\n";
    return ":" + oper.get_nickname() + " KICK " + chan.get_name() + " " + target.get_nickname() + "\r\n";
}

const std::string bld_err_nicknameinuse(const User &user) {
    return "433 " + user.get_nickname() + " :" + user.get_nickname() + "\r\n";
}

const std::string bld_err_nosuchnick(const std::string &name) {
    return "401 " + name + " :No such nick/channel\r\n";
}

const std::string bld_err_chanoprivsneeded(const Channel &chan) {
    return "482 " + chan.get_name() + " :You're not channel operator\r\n";
}

const std::string bld_err_nonicknamegiven(void) {
    return "431 * :No nickname given\r\n";
}

const std::string bld_err_erroneusnickname(const std::string &nick) {
    return "432 " + nick + " :Erroneus nickname\r\n";
}

const std::string bld_err_badchannelkey(const std::string &chan, const User &user)
{
    return ("475" + user.get_nickname() + chan + " :Cannot join channel (+k) - bad key" "\r\n");
}

const std::string bld_err_nosuchchannel(const std::string &chan) {
    return "403 " + chan + " :No such channel\r\n";
}

const std::string bld_err_notonchannel(const Channel &chan) {
    return "442 " + chan.get_name() + " :You're not on that channel\r\n";
}

const std::string bld_err_notonchannel(const std::string &chan) {
    return "442 " + chan + " :You're not on that channel\r\n";
}

const std::string bld_err_usernotinchannel(const std::string &user, const Channel &chan) {
    return "441 " + user + " " + chan.get_name() + " :They aren't on that channel\r\n";
}

const std::string bld_err_unknowmode(const char mode, const User &user) {
    return "472 " + user.get_nickname() + " " + mode + " :is an unknown mode char to me\r\n";
}

const std::string bld_err_usersdontmatch(void) {
    return "502 * :Cant change mode for other users\r\n";
}

const std::string bld_err_umodeunknowflag(void) {
    return "501 * :Unknown MODE flag\r\n";
}

const std::string bld_err_notexttosend(void) {
    return "412 :No text to send\r\n";
}

const std::string bld_err_cannotsendtochan(const Channel &chan) {
    return "404 " + chan.get_name() + " :No such channel\r\n";
}

const std::string bld_err_needmoreparams(const std::string &cmd, const User &user) {
    return "461 " + user.get_nickname() + " " + cmd + " :Not enough parameters\r\n";
}

const std::string bld_err_needpass(void) {
    return "ERROR :" NOPASS "\r\n";
}

const std::string bld_bad_pass(void) {
    return "ERROR :" BADPASS "\r\n";
}

const std::string bld_err_alreadyregistred(void)
{
    return "462 :Unauthorized command (already registered)\r\n";
}

const std::string bld_err_chanfull(const Channel &chan, const User &user){
    return "471 " + user.get_nickname() + " " + chan.get_name() + " :Cannot join channel (+l) - channel is full, try again later\r\n";
}

const std::string bld_err_inviteonlychan(const Channel &chan, const User &user){
    return "473 " + user.get_nickname() + " " + chan.get_name() + " :Cannot join channel (+i) - you must be invited\r\n";
}

const std::string bld_err_unknowncmd(const std::string cmd)
{
    return "421 " + cmd + " :Unknown command\r\n";
}
