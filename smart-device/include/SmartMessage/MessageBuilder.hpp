#pragma once

#include <memory>

#include "SmartMessage/Message.hpp"

#include "Mqtt/MqttMessage.hpp"

class MessageBuilder : public Message
{
public:
    MessageBuilder(int qos, const std::string command, const std::string subcommand) 
        : Message(qos, command, subcommand) {}

    virtual std::shared_ptr<MqttMessage> build() = 0;
};
