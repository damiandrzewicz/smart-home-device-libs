#include "SmartMessage/Message.hpp"

Message::Message(int qos, const std::string command, const std::string subcommand)
{
    _qos = qos;
    _command = command;
    _subcommand = subcommand;
}

std::string Message::getCommand() const
{
    return _command;
}

std::string Message::getSubcommand() const
{
    return _subcommand;
}

std::string Message::getCommandIdentity() const
{
    return _command + "/" + _subcommand;
}

int Message::getQos() const
{
    return _qos;
}
