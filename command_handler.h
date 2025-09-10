#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

#include "ServerCore.h"
#include "Message.h"
#include "User.h"

void handle_command(User &user, Message const &message, ServerCore &core);

#endif /* COMMAND_HANDLER_H_ */
