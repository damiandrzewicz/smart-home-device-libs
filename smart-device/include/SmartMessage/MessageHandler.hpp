#pragma once

#include <string>
#include <functional>

#include "SystemUtils/Utils.hpp"

#include "SmartMessage/MessageJson.hpp"

class MessageHandler : public MessageJson
{
public:
    MessageHandler()
    {
        getTopic().setDomain(MessageDomain::MY_HOME);
        getTopic().setSubdomain(System::Utils::MAC::GetClientId());
    }

    virtual ~MessageHandler(){}

    void handle(const std::string &data){
        parseRootJsonString(data);

        _handle();

        clearRootJsonObject();
    }

protected:
    virtual void _handle() = 0;
};