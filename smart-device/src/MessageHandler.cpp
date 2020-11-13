#include "SmartMessage/MessageHandler.hpp"

std::string MessageHandler::getCommandIdentity() const
{
    return _command + "/" + _subcommand;
}