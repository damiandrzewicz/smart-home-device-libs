#pragma once


#include <memory>
#include <list>

#include "Mqtt/MqttMessage.hpp"
#include "SmartMessage/MessageHandler.hpp"
#include "SmartMessage/MessageTopicProcessor.hpp"

class MessageManager
{
public:
    void process(std::shared_ptr<MqttMessage> msg);

private:
    std::list<std::shared_ptr<MessageHandler>> _messageHandlers;

    MessageTopicProcessor _messageTopicProcessor;
};