#ifndef USER_H_
#define USER_H_

#include <string>

#include <ctime>

#include "Message.h"

class User
{
    public:
        typedef enum {
            WAITING_FOR_PASS,
            WAITING_FOR_CONN_1,
            WAITING_FOR_CONN_2,
            CONNECTED,

            WAITING_FOR_QUIT
        } user_state_t;

    private:
        int _fd;
        std::string _nickname;
        std::string _username;
		std::string _hostname;
		std::string _servername;
		std::string _realname;
		std::string _prefix;
        std::string _buffer;
        user_state_t  _state;
        int _flags;
		time_t _last_ping;
		time_t _last_pong;
		time_t _idle;

    
    public:
        User(int fd);

        User operator=(const User &rhs) const;
        bool operator==(const User &user) const;
        int is_me(int fd)                      const;
        int is_me(std::string const &nickname) const;
        int is_state(user_state_t state)       const;

		int		check_flag(int flag) const;
		void	set_flag(int flag);
        void    set_state(user_state_t state);
        void    set_nickname(std::string const &nickname);
        void    set_user(
            std::string const &username,
            std::string const &hostname,
            std::string const &servername,
            std::string const &realname
        );
        void               set_last_pong(void);
        void               set_last_ping(void);
        int                get_flag(int flag);
        int                remove_flag(int flag);
        const user_state_t &get_state(void);
        const std::string  &get_nickname(void)   const;
        const std::string  &get_username(void)   const;
        const std::string  &get_hostname(void)   const;
        const std::string  &get_servername(void) const;
        const std::string  &get_realname(void)   const;
        const std::string  get_prefix(void)     const;
        const std::string  get_mode(void)        const;
        time_t             get_last_ping(void)   const;
        time_t             get_delta(void)       const;

        int     send_message(std::string const &message, bool throw_exception=true);
        void    recvu(void);
        int     is_message_buffered(void);
        void    get_message(Message &msg);

        void    send_ping(void);

		void	set_idle(void);
		time_t	get_idle(void) const;

		void	close_connection(void);

        class DisconnectException : public std::exception { public: virtual const char* what() const throw() { return ("DisconnectException"); } };

        ~User(void);
};


#endif
