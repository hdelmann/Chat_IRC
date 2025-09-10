#include "Channel.h"
#include "message_builder.h"
#include <iostream>
Channel::Channel(std::string const &channel_name) : channel_name(channel_name), key(""),flags(0), user_limit(-1){
}

bool Channel::operator==(Channel const &channel)
{
    return this->channel_name == channel.channel_name;
}

int Channel::is_me(std::string const &channel_name)
{
    return this->channel_name == channel_name;
}

void Channel::set_key(std::string str)
{
    this->key = str;
}

int Channel::remove_flag(int flag)
{
   return this->flags &= ~flag;
}

void Channel::set_flags(int flag)
{
    this->flags |= flag;
}

int Channel::get_flag(int flag)
{
    return((this->flags & flag) == flag);
}

std::string const &Channel::get_name(void) const
{
    return this->channel_name;
}

std::string const &Channel::get_topic(void) const
{
    return this->topic;
}

std::string const &Channel::get_key(void) const{
    return this->key;
}


int Channel::join(User &user){
    this->members.push_back(&user);
    this->broadcast(bld_join_msg(user, *this), NULL);
    user.send_message(bld_rpl_userstatus(user, *this));
    user.send_message(bld_rpl_namreply(*this, user));
    user.send_message(bld_rpl_endofnames(*this, user));
    return 0;
}

int Channel::is_on_invite(void){
    if (this->get_flag(MODE_i))
        return 1;
    return 0;
}


int Channel::is_user_invited(User &user)
{
    for (std::vector<User*>::iterator it = this->invited.begin(); it < this->invited.end(); it++)
    {
        if (user == (**it))
        {
            return 1;
        }
        
    }
    return 0;
}

int Channel::quit(User &user, std::string const message)
{
    for (std::vector<User*>::iterator it = this->members.begin(); it < this->members.end(); it++)
    {
        if (user.get_nickname() == (*it)->get_nickname())
        {
            this->broadcast(bld_part_msg(user, *this, message), NULL);
            this->members.erase(it);
            break;
        }
    }

    for (std::vector<User*>::iterator it2 = this->members.begin(); it2 < this->members.end(); it2++)
    {
    }
    return (0);
}

int Channel::kick(User &kicker, User &kicked, std::string const &reason)
{
    for (std::vector<User*>::iterator it = this->members.begin(); it < this->members.end(); it++)
    {
        if (kicked.get_nickname() == (*it)->get_nickname())
        {
            this->broadcast(bld_kick_msg(kicker, kicked, *this, reason), NULL);
            this->members.erase(it);
            break;
        }

    }
    return 0;
}

int Channel::invite(User &user, User *uinvited){
    user.send_message(bld_rpl_invite(*uinvited, *this));
    uinvited->send_message(bld_rpl_invite_msg(user, *this));
    this->invited.push_back(uinvited);
    return 1;
}


void Channel::set_channel()
{
}

void Channel::set_chanmodes(const std::string &modes){
    this->modes = modes;
}

void Channel::set_topic(std::string const &topic){
    this->topic = topic;
}

void Channel::set_topic_changer(const User &user)
{    
    this->topic_changer = user.get_nickname();
    this->topic_changer_prefix = user.get_prefix();
    this->t_changed_at = time(NULL);
}

std::string const &Channel::get_topic_changer(void) const
{
    return this->topic_changer;
}

std::string const &Channel::get_topic_changer_prefix(void) const
{
    return this->topic_changer_prefix;
}

std::string const &Channel::get_chanmodes(void){
    return this->modes;
}

time_t  Channel::get_t_changed_at(void) const
{
    return this->t_changed_at;
}

int Channel::add_OP(User &user){
    this->OPs.push_back(&user);
    return 0;
}

int Channel::remove_OP(const User &user)
{
    for (std::vector<User*>::iterator it = this->OPs.begin(); it < this->OPs.end(); it++)
    {
        if (user.get_nickname() == (*it)->get_nickname())
        {
            this->OPs.erase(it);
            break;
        }

    }
    return 0;
}

int Channel::is_user_present(std::string const &nickname)
{
    for (std::vector<User*>::iterator it = this->members.begin(); it < this->members.end(); it++)
    {
        if ((*it)->get_nickname() == nickname)
            return 1;
    }
    return 0;
}

int Channel::is_user_OP(User const &user) const{
    for (unsigned long i = 0; i < this->OPs.size(); i++)
    {
        if(*this->OPs[i] == user)
        {
            return (1);
        }
    }
    return 0;
}

int Channel::is_user_OP(std::string const &nickname) const{
    for (size_t i = 0; i < this->OPs.size(); i++)
    {
        if(this->OPs[i]->is_me(nickname) == 1)
        {
            return (1);
        }
    }
    return 0;
}

int Channel::broadcast(std::string const &message, const User *from_user){
    for (int i = 0; i < (int)this->members.size(); i++)
    {
        if (this->members[i] == from_user)
        {
            continue;
        }
        this->members[i]->send_message(message, false);
    }
    return 0;
}

size_t Channel::members_count(void) const
{
    return this->members.size();
}

const std::string Channel::get_username_list(std::string const mode) const{

    std::string parsd_mem;

    for (std::vector<User*>::const_iterator it = this->members.begin(); it < this->members.end(); it++)
    {
        if(this->is_user_OP((*it)->get_nickname()))
        {
            parsd_mem += mode + (*it)->get_nickname() + " ";
        }
        else
            parsd_mem += (*it)->get_nickname() + " ";
    }
    return parsd_mem;
}

void Channel::set_userlimit(int i){
    this->user_limit = i;
}

const size_t &Channel::get_userlimit(void) const{
    return(this->user_limit);
}

