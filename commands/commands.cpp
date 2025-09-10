#include "commands.h"
#include <iostream>
#include "../message_builder.h"
#include "../Channel.h"
#include "../User.h"
#include "../UserManager.h"
#include "../ServerCore.h"
#include <string> 
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdlib>

std::string get_arg(std::string str)
{
    std::string str2;
    size_t i = str.find(',');
    if (i == std::string::npos)
    {
        return(str);
    }
    str2 = str.substr(0, i);
    return(str2);
}

void pass_handler(User &user, Message const &message, ServerCore &core)
{
    if (user.is_state(User::CONNECTED))
    {
        user.send_message(bld_err_alreadyregistred(), false);
    }
    if (core.get_password() != message.get_params()[0])
    {
        user.send_message("Error: incorrect password\r\n", true);
        user.set_state(User::WAITING_FOR_QUIT);
    }
    else
    {
        user.set_state(User::WAITING_FOR_CONN_1);
    }
}

int check_nick(const std::string str)
{
    if ((str[0] < 'A' || str[0] > 'Z') && (str[0] < 'a' || str[0] > 'z'))
    {
        return (0);
    }
    for (int i = 0; str[i]; i++)
    {
        if (((str[0] < 'A' || str[0] > 'Z') && (str[0] < 'a' || str[0] > 'z')) && (str[i] < 0 || str[i] > 9))
        {
            if (str[i] != '-' && str[i] != '[' && str[i] != ']' && str[i] != '\\' && str[i] != '`' && str[i] != '^' && str[i] != '{' && str[i] != '}')
            {
                return (0);
            }
        }
    }
    return (1);
}

std::string same_nick(std::string str)
{
    std::string str2;
    if (str[0] == '_')
    {
        for (size_t j = 1; j < str.size(); j++)
        {
            str2 += str[j];
        }
        return(str2);
    }
    return(str);
}


std::string itoa(int nombre) {
    std::string chaine;
    int temp = nombre;
    
    if (nombre < 0) {
        chaine += '-';
        temp = -nombre;
    }
    int temp2 = temp;
    int digits = 0;
    do {
        digits++;
    } while (temp2 /= 10);
    chaine.resize(digits);
    for (int i = digits - 1; i >= 0; --i) {
        chaine[i] = '0' + temp % 10;
        temp /= 10;
    }
    
    return chaine;
}

std::string same_new_nick(std::string str, ServerCore &core)
{
    std::string str2 = same_nick(str);
    UserManager *_user_man = &core.get_userManager();
    while (1)
    {
        srand(time(NULL));

        int random_number = rand();
        std::string num = itoa(random_number);
        if (_user_man->get_user(str2 + num) == NULL)
        {
            return(str2 + num);
        }
    }
    return str2;
}

void nick_handler(User &user, Message const &message, ServerCore &core)
{
    std::string msg = message.get_params()[0];
    UserManager *_user_man = &core.get_userManager();
    if (message.get_params().size() < 1 || message.get_params()[0] == "")
    {
        user.send_message(bld_err_nonicknamegiven(), false);
        return;
    }
    if (_user_man->get_user(msg) != NULL)
    {
        if (user.is_state(User::CONNECTED))
        {
            user.send_message(bld_err_nicknameinuse(user), false);
            return;
        }
        else
        {
            msg = '_' + message.get_params()[0];
        }
    }
    if (_user_man->get_user(same_nick(msg)))
    {
        std::string msg1 = msg;
        msg = same_new_nick(msg1, core);
    }
    if (check_nick(msg) <= 0)
    {
        user.send_message(bld_err_erroneusnickname(msg), false);
        user.set_state(User::WAITING_FOR_QUIT);
        return;
    }
    if (user.is_state(User::WAITING_FOR_CONN_1) || user.is_state(User::WAITING_FOR_CONN_2))
    {
        user.set_nickname(msg);
        if (user.is_state(User::WAITING_FOR_CONN_1))
            user.set_state(User::WAITING_FOR_CONN_2);
        else if (user.is_state(User::WAITING_FOR_CONN_2))
            user.set_state(User::CONNECTED);
    }
    else if (user.is_state(User::CONNECTED))
    {
        user.send_message(bld_nick_msg(user, msg), false);
        user.set_nickname(msg);
    }
}

void user_handler(User &user, Message const &message, ServerCore &core)
{
    (void)user;
    (void)core;
    if (user.is_state(User::CONNECTED))
    {
        user.send_message(bld_err_alreadyregistred(), false);
        return;
    }
    user.set_user(message.get_params()[0], message.get_params()[1], message.get_params()[2], message.get_params()[3]);
    if (user.is_state(User::WAITING_FOR_CONN_1))
        user.set_state(User::WAITING_FOR_CONN_2);
    else if (user.is_state(User::WAITING_FOR_CONN_2))
    {
        user.set_state(User::CONNECTED);
    }
    user.send_message(bld_rpl_welcome(user) + "\r\n", true);
}
void join_handler(User &user, Message const &message, ServerCore &core)
{
    ChannelManager &_chanel_manager = core.get_channelManager();
    std::string chans = message.get_params()[0];
    std::string keys = "";
    while(chans.size() > 0)
    {
        std::string cha_name = get_arg(chans);
        Channel *chan = _chanel_manager.get_channel(cha_name);
        if (message.get_params().size() > 1 && chan != NULL)
            keys = message.get_params()[1];
        if(chans.find(',') != std::string::npos)
        {
            chans = chans.substr(chans.find(',') + 1, chans.size() - chans.find(','));
        }
        else
            chans = "";
        if (chan == NULL)
        {
            if (cha_name[0] != '#' && cha_name[0] != '&') {
                user.send_message(bld_err_nosuchchannel(cha_name));
                return ;
            }
            else
                _chanel_manager.create(user, cha_name);
        }
        else{
        if (chan->get_flag(MODE_l))
        {
            if (chan->get_userlimit() == chan->members_count())
            {
                user.send_message(bld_err_chanfull(*chan, user));
                return;
            }

        }
        else if (chan->get_flag(MODE_k))
        {
            std::string key = get_arg(keys);
            if (keys.find(',') != std::string::npos)
            {
                keys = keys.substr(keys.find(',') + 1, keys.size() - keys.find(','));
            }
            else{
                keys = "";
            }
            if (key == chan->get_key())
            {
                if (chan->is_on_invite())
                {
                    if (chan->is_user_invited(user))
                        chan->join(user);
                    else
                        user.send_message(bld_err_inviteonlychan(*chan, user));
                }
                else
                    chan->join(user);
            }
            else
            {
                user.send_message(bld_err_badchannelkey(chan->get_name(), user));
            }
        }
        else if (chan->get_flag(MODE_i))
        {
                if (chan->is_on_invite())
                {
                    if (chan->is_user_invited(user))
                        chan->join(user);
                    else
                        user.send_message(bld_err_inviteonlychan(*chan, user));
                }
                else
                    chan->join(user);
        }
        else
            chan->join(user);
        }
    }
}

int check_mode(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] != 'i' && str[i] != 'k' && str[i] != 'o' && str[i] != 'l' && str[i] != 't' && str[i] != '+' && str[i] != '-')
        {
            return (0);
        }
    }
    return (1);
}


int check_if_mode(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == 'i' || str[i] == 'k' || str[i] == 'o' || str[i] == 'l' || str[i] == 't')
        {
            return (1);
        }
    }
    return (0);
}

int check_params(Message const &message)
{
    size_t nbpar = 0;
    int k = 0;
    int o = 0;
    int l = 0;
    int signe;
    std::string str = message.get_params()[1];
    if (str[0] == '-')
        signe = -1;
    else
        signe = 1;
    for (size_t i = 0; i < str.size() ; i++)
    {
        if (str[i] == '+')
        {
            signe = 1;
            k = 0;
            o = 0;
            l = 0;
        }
        if (str[i] == '-')
        {
            signe = -1;
            k = 0;
            o = 0;
            l = 0;
        }
        if (str[i] == 'o' && o == 0)
        {
            nbpar += 1;
            o = 1;
        }
        if (str[i] == 'k' && k == 0)
        {
            nbpar += 1;
            k = 1;
        }
        if (str[i] == 'l' && l == 0 && signe == 1)
        {
            nbpar += 1;
            l = 1;
        }
    }
    if (nbpar < message.get_params().size() - 2)
        return (0);
    return (1);
}


std::string finalmodestr(std::string str)
{
    int o = 0;
    int k = 0;
    int l = 0;
    int signe = 0;
    std::string str2;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '+')
        {
            if (signe == 0 || signe == -1)
                str2 += '+';
            signe = 1;
            k = 0;
            o = 0;
            l = 0;
        }
        else if (str[i] == '-')
        {
            if (signe == 0 || signe == 1)
                str2 += '-';
            signe = -1;
            k = 0;
            o = 0;
            l = 0;
        }
        else if (str[i] == 'o' && o == 0)
        {
            str2 += str[i];
            o = 1;
        }
        else if (str[i] == 'k' && k == 0)
        {
            str2 += str[i];
            k = 1;
        }
        else if (str[i] == 'l' && l == 0)
        {
            str2 += str[i];
            l = 1;
        }
        else{
            str2 += str[i];
        }
    }
    return str2;
}

unsigned long int howmuchneedparam(Message const &message)
{
    int number = 0;
    int signe = 1;
    for (size_t i = 0; i < message.get_params()[1].size(); i++)
    {
        if (message.get_params()[1][i] == '+')
            signe = 1;
        if (message.get_params()[1][i] == '-')
            signe = -1;
        if (message.get_params()[1][i] == 'o' || message.get_params()[1][i] == 'k')
            number++;
        if (message.get_params()[1][i] == 'l' && signe == 1)
            number++;

    }
    return (number);
}

void mode_handler(User &user, Message const &message, ServerCore &core) {
    
    (void)user;    

    if ((message.get_params()[0][0] == '#' || message.get_params()[0][0] == '&') && user.is_state(User::CONNECTED) && message.get_params().size() >= 2) {
        if  (howmuchneedparam(message) > message.get_params().size() - 2) {
            user.send_message(bld_err_needmoreparams(message.get_command(), user));
            return;
        }
        if (check_if_mode(message.get_params()[1]) == 0)
            return;
        if (check_mode(message.get_params()[1]) == 0)
        {
            user.send_message(bld_err_unknowmode(message.get_params()[1][0], user));
            return;
        }
        std::string str = message.get_params()[1];
        int i = 1;
        int k = 2;
        std::string addmode;
        std::string addopt = " ";
        UserManager    &user_man = core.get_userManager();
        ChannelManager &chan_man = core.get_channelManager();
        Channel *chan = chan_man.get_channel(message.get_params()[0]);
        if (chan == NULL) {
            user.send_message(bld_err_nosuchchannel(message.get_params()[0]));
            return;
        }
        if (chan->is_user_OP(user) == 0)
        {
            user.send_message(bld_err_chanoprivsneeded(*chan));
            return ;
        }
        for (size_t j = 0; j < str.size(); j++)
        {
            if (str[j] == '-')
            {
                i =-1;
                if (str[j + 1] != '+' && str[j + 1] != '-')
                    addmode += '-';
            }
            if (str[j] == '+')
            {
                i = 1;
                if (str[j + 1] != '+' && str[j + 1] != '-')
                    addmode += '+';
            }
            if (str[j] == 'k')
            {
                if (i == 1)
                {
                    chan->set_flags(MODE_k);
                    chan->set_key(message.get_params()[k]);
                    addmode += 'k';
                    addopt += message.get_params()[k] + ' ';
                    k++;
                }
                if (i == -1 && chan->get_flag(MODE_k) == 1)
                {
                    chan->remove_flag(MODE_k);
                    chan->set_key("");
                    addmode += 'k';
                    addopt += "* ";
                    k++;
                }
                else if (i == -1 && chan->get_flag(MODE_k) == 0)
                    k++;
            }
            if (str[j] == 'l')
            {
                if (i == 1)
                {
                    chan->set_flags(MODE_l);
                    if (ft_str_is_numeric(const_cast<char *>(message.get_params()[k].c_str())))
                    {
                        continue;
                    }
                    chan->set_userlimit(atoi(message.get_params()[k].c_str()));
                    addmode += 'l';
                    addopt += message.get_params()[k] + ' ';
                    k++;
                }
                if (i == -1 && chan->get_flag(MODE_l) == 1)
                {
                    chan->remove_flag(MODE_l);
                    chan->set_userlimit(-1);
                    addmode += 'l';
                }
            }
            if (str[j] == 'o')
            {
                if (i == 1)
                {
                    if (chan->is_user_present(message.get_params()[k]) == 1)
                    {
                        chan->add_OP(*user_man.get_user(message.get_params()[k]));
                        addmode += 'o';
                        addopt += message.get_params()[k] + ' ';
                        k++;
                    }
                    else
                    {
                        user.send_message(bld_err_nosuchnick(user.get_nickname()));
                    }
                }
                if (i == -1)
                {
                    if (chan->is_user_present(message.get_params()[k]) == 1)
                    {
                        chan->remove_OP(*user_man.get_user(message.get_params()[k]));
                        addmode += 'o';
                        addopt += message.get_params()[k] + ' ';
                        k++;
                    }
                    else
                    {
                        user.send_message(bld_err_nosuchnick(user.get_nickname()));
                    }
                    
                }
            }
            if (str[j] == 't')
            {
                if (i == 1)
                {
                    chan->set_flags(MODE_t);
                    addmode += 't';
                }
                if (i == -1)
                {
                    chan->remove_flag(MODE_t);
                    addmode += 't';
                }
            }
            if (str[j] == 'i')
            {
                if (i == 1)
                {
                    chan->set_flags(MODE_t);
                    addmode += 'i';
                }
                if (i == -1)
                {
                    chan->remove_flag(MODE_i);
                    addmode += 'i';
                }
            }
        }
        std::string final = finalmodestr(addmode) + addopt;
        user.send_message(bld_rpl_modechg(user, *chan, final));
        return;
    }
    if (message.get_params().size() < 2 && message.get_params()[0] == user.get_nickname()) {
        user.send_message(bld_rpl_umodeis(user));
        return;
    }
    if (!user.is_me(message.get_params()[0]) && (message.get_params()[0][0] != '#' && message.get_params()[0][0] != '&')) {
        user.send_message(bld_err_usersdontmatch());
        return;
    }
    if (message.get_params().size() >= 2 && (message.get_params()[1]  == "+i" || message.get_params()[1] == "i"))
    {
        user.set_flag(MODE_i);
        user.send_message(bld_rpl_currentmodestateusr(user, "+i"));
    }
    else if (message.get_params().size() >= 2 && (message.get_params()[1]  == "-i"))
    {
        user.remove_flag(MODE_i);
        user.send_message(bld_rpl_currentmodestateusr(user, "-i"));
    }
    else if (message.get_params()[0][0] != '#' && message.get_params()[0][0] != '&')
        user.send_message(bld_err_umodeunknowflag());
}
void part_handler(User &user, Message const &message, ServerCore &core)
{
    ChannelManager &_chan_man = core.get_channelManager();
    std::string chanlist = message.get_params()[0];

    while (chanlist.size() > 0) {
        std::string channame = get_arg(chanlist);
        if (chanlist.find(',') != std::string::npos)
        {
            chanlist = chanlist.substr(chanlist.find(',') + 1, chanlist.size() - chanlist.find(','));
        }
        else{
            chanlist = "";
        }
        Channel *chan = _chan_man.get_channel(channame);
        if (chan){
            if (message.get_params().size() > 1 && message.get_params()[1].size())
            {
                _chan_man.leave(user, *chan, message.get_params()[1]);
            }
            else
                _chan_man.leave(user, *chan, "No reason specified");
        }
        else
            user.send_message(bld_err_nosuchchannel(channame));
    }
}

void pong_handler(User &user, Message const &message, ServerCore &core)
{
    (void)message;
    (void)core;
    user.set_last_pong();
}
void ping_handler(User &user, Message const &message, ServerCore &core)
{
    (void)core;
    user.send_message("PONG :" + message.get_params()[0] + "\r\n", false);
    user.set_last_ping();
    user.set_last_pong();
}


void kick_handler(User &user, Message const &message, ServerCore &core)
{
    ChannelManager &chan_man = core.get_channelManager();
    std::string chans = message.get_params()[0];
    while (chans.size() > 0)
    {
        Channel *chan = chan_man.get_channel(get_arg(chans));
        if(chans.find(',') != std::string::npos)
        {
            chans = chans.substr(chans.find(',') + 1, chans.size() - chans.find(','));
        }
        else
            chans = "";
        if (chan == NULL)
        {
            user.send_message(bld_err_nosuchchannel(chans), false);
        }
        else if (chan->is_user_present(user.get_nickname()) == 0)
        {
            user.send_message(bld_err_notonchannel(chan->get_name()), false);
        }
        else if (chan->is_user_OP(user))
        {

            std::string users = message.get_params()[1];
            while (users.size() > 0)
            {
                UserManager &user_man = core.get_userManager();
                User *user_kick = user_man.get_user(get_arg(users));
                std::string user_name = get_arg(users);
                if(users.find(',') != std::string::npos)
                    users = users.substr(users.find(',') + 1, users.size() - users.find(','));
                else
                    users = "";
                if (user_kick == NULL)
                {
                    user.send_message(bld_err_nosuchnick(user_name), false);
                    continue;
                }
                if (message.get_params().size() == 3)
                    chan->kick(user, *user_kick, message.get_params()[2]);
                else
                    chan->kick(user, *user_kick, "");
            }
        }
        else
        {
            user.send_message(bld_err_chanoprivsneeded(*chan), false);
        }
    }
}

void topic_handler(User &user, Message const &message, ServerCore &core) {
    ChannelManager *_chanel_man = &core.get_channelManager();
    Channel *channel = _chanel_man->get_channel(message.get_params()[0]);
    if (!channel)
        user.send_message(bld_err_nosuchchannel(message.get_params()[0]));
    else if(!channel->is_user_present(user.get_nickname()))
        bld_err_notonchannel(channel->get_name());
    else if (channel->is_user_OP(user))
    {
        if (message.get_params().size() == 2) {
            channel->set_topic_changer(user);
            channel->set_topic(message.get_params()[1]);
            channel->broadcast(bld_rpl_topic_msg(user ,*channel, true), NULL);
        }
        else{
            user.send_message(bld_rpl_topic(*channel, channel->get_topic_changer()));
            user.send_message(bld_rpl_topic_changer(*channel));
        }
    }
    else
    {
        if(message.get_params().size() == 2)
            user.send_message(bld_err_chanoprivsneeded(*channel));
        else
          user.send_message((bld_rpl_topic(*channel, user.get_nickname())));
    }
}

void privmsg_handler(User &user, Message const &message, ServerCore &core)
{
    ChannelManager &_chanel_man = core.get_channelManager();
    std::string message_;
    if (message.get_params().size() < 1)
    {
        user.send_message("412 :No text to send");
        return;
    }
    for (int i = 1; i < (int)message.get_params().size(); i++)
    {
        message_ += message.get_params()[i];
    }
    if (message.get_params()[0][0] == '#' || message.get_params()[0][0] == '&')
    {
        Channel *channel = _chanel_man.get_channel(message.get_params()[0]);
        if (!channel)
        {
            user.send_message(bld_err_nosuchchannel(message.get_params()[0]));
            return ;
        }
        if (!channel->is_user_present(user.get_nickname()))
        {
            user.send_message(bld_err_notonchannel(*channel));
            return ;
        }
        channel->broadcast(bld_privmsg_msg(user, *channel, message_), &user);
    }
    else
    {
        UserManager *user_man = &core.get_userManager();
        User *recipient_user = user_man->get_user(message.get_params()[0]);
        if (recipient_user == NULL)
            user.send_message(bld_err_nosuchnick(message.get_params()[0]), false);
        else
            recipient_user->send_message(bld_privmsg_msg(user, *recipient_user, message_), false);
    }
}

void quit_handler(User &user, Message const &message, ServerCore &core){
    ChannelManager &_chanel_man = core.get_channelManager();

    if (message.get_params().size())
        _chanel_man.leave(user, message.get_params()[0]);
    else
        _chanel_man.leave(user, "No reason specified");
    
    user.set_state(User::WAITING_FOR_QUIT);
}

void invite_handler(User &user, Message const &message, ServerCore &core){
    ChannelManager &_chanel_man = core.get_channelManager();
    Channel   *chan = _chanel_man.get_channel(message.get_params()[1]);
    UserManager &userman =core.get_userManager();
    
    if (chan == NULL) {
        user.send_message(bld_err_nosuchchannel(message.get_params()[1]), false);
        return;
    }

    if (!chan->is_user_OP(user)) {
        user.send_message(bld_err_chanoprivsneeded(*chan));
        return;
    }
    User *invd = userman.get_user(message.get_params()[0]);
    if (invd == NULL) {
        user.send_message(bld_err_nosuchnick(message.get_params()[0]), false);
        return;
    }
    chan->invite(user, invd);
}

void cap_handler(User &user, Message const &message, ServerCore &core)  {
    (void)user;
    (void)message;
    (void)core;
    return;
}

void whois_handler(User &user, Message const &message, ServerCore &core){
    if (message.get_params().size() < 1) {
        user.send_message(bld_err_nonicknamegiven(), false);
        return;
    }
    User *target = core.get_userManager().get_user(message.get_params()[0]);
    if (target == NULL) {
        user.send_message(bld_err_nosuchnick(message.get_params()[0]));
        return;
    }
    user.send_message(bld_rpl_whoisuser(*target));

    if (target->get_flag(MODE_o))
        user.send_message(bld_rpl_whoisoperator(*target));
    user.send_message(bld_rpl_whoisidle(*target));
    user.send_message(bld_rpl_endofwhois(*target));

}
