#pragma once

#include <memory>

#include "esp_log.h"

#include "SmartMessage/MessageTopic.hpp"
#include "SmartMessage/MessageDomain.hpp"

#include "SmartMessage/MessageJson.hpp"
#include "SmartMessage/MessageTopicProcessor.hpp"

#include "SystemUtils/Utils.hpp"

#include "Mqtt/MqttMessage.hpp"

class MessageBuilder : public MessageJson
{
public:
    MessageBuilder(int qos, const std::string command, const std::string subcommand = "") 
        :   MessageJson(qos, command, subcommand),
            _messageTopic(MessageDomain::MY_HOME, System::Utils::MAC::GetClientId(), getCommand(), getSubcommand())
        {
            cJSON_AddItemToObject(getRootJsonObject(), "data", getDataJsonObject());
            
        }

    std::shared_ptr<MqttMessage> build()
    {
        auto msg = std::make_shared<MqttMessage>();
        msg->topic = buildTopic();
        msg->qos = getQos();

        appendTimestampItem();

        _build(msg);

        //msg->ready = true;

        return msg;
    }

protected:
    virtual void _build(std::shared_ptr<MqttMessage> msg) = 0;

    std::string buildTopic(){
        return MessageTopicProcessor::build(_messageTopic);
    }

    void appendTimestampItem()
    {
        const char *timestamp = esp_log_system_timestamp();
        cJSON_AddStringToObject(getDataJsonObject(), "timestamp", timestamp);
    }

protected:
    MessageTopic _messageTopic;
};
