#pragma once

#include <string>
#include <functional>

#include "SmartMessage/MessageJson.hpp"

class MessageHandler : public MessageJson
{
public:
    MessageHandler(){}
    virtual ~MessageHandler(){}

    void handle(const std::string &data){
        parseRootJsonString(data);

        _handle();

        clearRootJsonObject();
    }

protected:
    virtual void _handle() = 0;
};