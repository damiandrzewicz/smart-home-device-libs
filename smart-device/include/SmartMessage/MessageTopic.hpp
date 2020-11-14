#pragma once

#include <string>

class MessageTopic
{
public:
    MessageTopic(){}
    MessageTopic(auto domain_, auto subdomain_, auto command_, auto subcommand_) 
        : domain(domain_), subdomain(subdomain_), command(command_), subcommand(subcommand_)
    {}


    void setDomain(auto domain){this->domain = domain;}
    void setSubdomain(auto subdomain){ this->subdomain = subdomain; }
    void setCommand(auto command){ this->command = command; }
    void setSubcommand(auto subcommand){ this->subcommand = subcommand; }

    auto getDomain() const {return domain;}
    auto getSubdomain() const {return subdomain;}
    auto getCommand() const {return command;}
    auto getSubcommand() const {return subcommand;}

    auto getDomaindIdentity() const {return domain + "/" + subdomain;}
    auto getCommandIdentity() const 
    {
        std::string temp = command; 
        if(!subcommand.empty()) temp += "/" + subcommand;
        return temp;
    }

private:
    std::string domain;
    std::string subdomain;
    std::string command;
    std::string subcommand;
};