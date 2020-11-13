#pragma once

#include "SmartMessage/MessageTopicProcessor.hpp"
#include "SmartMessage/MessageTopic.hpp"

class MessageTopicProcessor
{
public:
    virtual std::string build(MessageTopic messateTopic);
    virtual MessageTopic parse(std::string messageTopic);
};