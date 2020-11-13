#pragma once

#include <string>
#include <functional>

class MessageHandler
{
public:

    std::string getCommandIdentity() const;

    virtual void handle(const std::string &data) = 0;

private:
    std::string _command;
    std::string _subcommand;
};