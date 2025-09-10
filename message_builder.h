#ifndef MESSAGE_BUILDER_H_
#define MESSAGE_BUILDER_H_

#include <string>

#include "Channel.h"
#include "User.h"

const std::string bld_rpl_topic(const Channel &chan, const std::string &nickname);
const std::string bld_rpl_welcome(const User &user);
const std::string bld_rpl_umodeis(const User &user);
const std::string bld_rpl_notopic(const Channel &chan);
const std::string bld_rpl_whoisuser(const User &user);
const std::string bld_rpl_whoisoperator(const User &user);
const std::string bld_rpl_endofwhois(const User &user);
const std::string bld_rpl_namreply(const Channel &chan, const User &user);
const std::string bld_rpl_topic_changer(const Channel &chan);
const std::string bld_rpl_userstatus(const User &user, Channel &chan);
const std::string bld_rpl_endofnames(const Channel &chan, User &user);
const std::string bld_rpl_whoisidle(const User &user);
const std::string bld_rpl_invite(const User &user, const Channel &chan);
const std::string bld_rpl_invite_msg(const User &user, const Channel &chan);
const std::string bld_rpl_modechg(const User &user, const Channel &chan, const std::string mchanges);
const std::string bld_rpl_topic_msg(const User &user, const Channel &chan, bool i);
const std::string bld_rpl_currentmodestateusr(const User &user, std::string mode);
const std::string bld_join_msg(const User &user, const Channel &chan);
const std::string bld_privmsg_msg(const User &user, const Channel &chan, const std::string &message);
const std::string bld_privmsg_msg(const User &user, const User &target, const std::string &message);
const std::string bld_part_msg(const User &user, const Channel &chan, const std::string &message);
const std::string bld_nick_msg(const User &user, const std::string &nick);
const std::string bld_ping_msg(const User &user);
const std::string bld_pong_msg(const User &user);
const std::string bld_kick_msg(const User &oper, const User &target, const Channel &chan, const std::string reason);

const std::string bld_err_badchannelkey(const std::string &chan, const User &user);
const std::string bld_err_nicknameinuse(const User &user);
const std::string bld_err_nosuchnick(const std::string &name);
const std::string bld_err_chanoprivsneeded(const Channel &chan);
const std::string bld_err_nonicknamegiven(void);
const std::string bld_err_erroneusnickname(const std::string &nick);
const std::string bld_err_nosuchchannel(const std::string &chan);
const std::string bld_err_notonchannel(const Channel &chan);
const std::string bld_err_notonchannel(const std::string &chan);
const std::string bld_err_usernotinchannel(const std::string &user, const Channel &chan);
const std::string bld_err_unknowmode(const char mode, const User &user);
const std::string bld_err_usersdontmatch(void);
const std::string bld_err_umodeunknowflag(void);
const std::string bld_err_notexttosend(void);
const std::string bld_err_cannotsendtochan(const Channel &chan);
const std::string bld_err_needmoreparams(const std::string &cmd, const User &user);
const std::string bld_err_needpass(void);
const std::string bld_err_alreadyregistred(void);
const std::string bld_err_inviteonlychan(const Channel &chan, const User &user);
const std::string bld_bad_pass(void);
const std::string bld_err_chanfull(const Channel &chan, const User &user);
const std::string bld_err_unknowncmd(const std::string cmd);
#endif
