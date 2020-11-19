#pragma once

#include <string>

#include "SmartMessage/MessageTopic.hpp"

class Message
{
public:
    Message(){}
    virtual ~Message()
    {
        printf("~Message");
    }

    MessageTopic &getTopic(){return _messageTopic;}

    void setQos(int qos){ _qos = qos; }
    int getQos() const { return _qos; };

private:
    MessageTopic _messageTopic;
    int _qos = 0;
};