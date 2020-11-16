#pragma once

#include <string>

class Message
{
public:
    Message(int qos, const std::string command, const std::string subcommand = "");
    virtual ~Message();

    std::string getCommand() const;
    std::string getSubcommand() const;
    std::string getCommandIdentity() const;
    int getQos() const;

private:
    std::string _command;
    std::string _subcommand;
    int _qos;
};