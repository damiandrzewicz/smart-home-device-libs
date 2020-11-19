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
    MessageBuilder() 
    {
        getTopic().setDomain(MessageDomain::MY_HOME);
        getTopic().setSubdomain(System::Utils::MAC::GetClientId());
    }

    virtual ~MessageBuilder()
    {
        
    }

    std::shared_ptr<MqttMessage> build()
    {
        buildRootJsonObject();
        buildDataJsonObject();

        cJSON_AddItemToObject(getRootJsonObject(), "data", getDataJsonObject());
        auto msg = std::make_shared<MqttMessage>();
        msg->topic = buildTopic();
        msg->qos = getQos();

        _build(msg);
        
        clearRootJsonObject();
        return msg;
    }

protected:
    virtual void _build(std::shared_ptr<MqttMessage> msg) = 0;

    std::string buildTopic(){
        return MessageTopicProcessor::build(getTopic());
    }

};
