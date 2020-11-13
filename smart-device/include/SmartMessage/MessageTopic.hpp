#pragma once

#include <string>

class MessageTopic
{
public:
    void setDomain(auto domain){this->domain = domain;}
    void setSmartDeviceId(auto smartDeviceId){ this->smartDeviceId = smartDeviceId; }
    void setCommand(auto command){ this->command = command; }
    void setSubcommand(auto subcommand){ this->subcommand = subcommand; }

    auto getDomain() const {return domain;}
    auto getSmartDeviceId() const {return smartDeviceId;}
    auto getCommand() const {return command;}
    auto getSubcommand() const {return subcommand;}

    auto getDomaindIdentity() const {return domain + "/" + smartDeviceId;}
    auto getCommandIdentity() const 
    {
        std::string temp = command; 
        if(!subcommand.empty()) temp += "/" + subcommand;
        return temp;
    }


private:
    std::string domain;
    std::string smartDeviceId;
    std::string command;
    std::string subcommand;
};