#pragma once

#include <string>
#include <functional>

#include "SmartMessage/Message.hpp"

class MessageHandler : public Message
{
public:
    MessageHandler(int qos, const std::string command, const std::string subcommand = "") 
        : Message(qos, command, subcommand){}

    virtual void handle(const std::string &data) = 0;
};