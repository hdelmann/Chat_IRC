#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <string>
#include <vector>

#include "User.h"
#include "UserManager.h"

class Channel
{
    private:
        std::string   		channel_name;
        std::string         topic;
        std::vector<User*>  members;
        std::vector<User*>  OPs;
        std::vector<User*>  invited;
        std::string         topic_changer;
        std::string         topic_changer_prefix;
        std::string         key;
        std::string         modes;
        int                 flags;
        time_t              t_changed_at;
        size_t                 user_limit;
        
    public:
        Channel(std::string const &channel_name);

        bool operator==(Channel const &channel);

        int is_me(std::string const &channel_name);

        std::string const &get_name(void)  const;
        std::string const &get_topic(void) const;
        std::string const &get_key(void) const;
        std::string const &get_topic_changer(void) const;
        std::string const &get_topic_changer_prefix(void) const;
        std::string const &get_member_names(void) const;
        time_t      get_t_changed_at(void) const;
        const size_t   &get_userlimit(void) const;
        User        get_invited(void);
        std::string const &get_chanmodes(void);

        int join(User &user);
        int quit(User &user, std::string const message);
        int kick(User &kicker, User &kicked, std::string const &reason);
        int invite(User &user, User *uinvited);
        int is_user_invited(User &user);
        int is_on_invite(void);

        void set_userlimit(int i);
        void set_channel();
        void set_key(std::string str);
        void set_topic(std::string const &topic);
        void set_invited(User &user);
        void set_topic_changer(const User &user);
        void set_flags(int flag);
        void set_chanmodes(const std::string &modes);
        int add_OP(User &user);
        int remove_OP(const User &user);

		int remove_user(User &user);

        int is_user_present(User const &user);
        int is_user_present(std::string const &nickname);

        int is_user_OP(User const &user) const;
        int is_user_OP(std::string const &nickname) const;

        int broadcast(std::string const &message, const User *from_user);

		size_t members_count(void) const;
        void   addmodes(std::string modes);
		const std::string get_username_list(std::string const mode) const;
        int get_flag(int flag);
        int remove_flag(int flag);

        ~Channel(void){
            
        };
};

#endif
