#pragma once

#include <exception>

#include "SmartMessage/MessageTopicProcessor.hpp"
#include "SmartMessage/MessageTopic.hpp"

class MessageTopicProcessorException : public std::exception
{
public:
    MessageTopicProcessorException(const char *msg_) : msg(msg_){}
    const char *what(){ return msg; }

private:
    const char *msg;
};

class MessageTopicProcessor
{
public:
    static std::string build(MessageTopic messateTopic);
    static MessageTopic parse(std::string messageTopic);
};