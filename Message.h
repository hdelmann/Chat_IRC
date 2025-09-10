#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include <vector>

class Message
{
    private:
        std::string                 command;
        std::vector<std::string>    params;
    
    public:
        Message(void);
        Message(std::string const &raw_message);

        void update(const std::string &raw_message);

        std::string const &get_command(void) const;
        std::vector<std::string> const &get_params(void)  const;

        ~Message(void);
};

#endif
