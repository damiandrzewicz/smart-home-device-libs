#pragma once

#include "SmartMessage/MessageTopicProcessor.hpp"
#include "SmartMessage/MessageTopic.hpp"

class MessageTopicProcessor
{
public:
    static std::string build(MessageTopic messateTopic);
    static MessageTopic parse(std::string messageTopic);
};