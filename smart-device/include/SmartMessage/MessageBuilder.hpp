#pragma once

#include <memory>
#include <exception>

#include "esp_log.h"

#include "SmartMessage/MessageTopic.hpp"
#include "SmartMessage/MessageDomain.hpp"

#include "SmartMessage/MessageJson.hpp"
#include "SmartMessage/MessageTopicProcessor.hpp"

#include "SystemUtils/Utils.hpp"

#include "Mqtt/MqttMessage.hpp"

class MessageBuilderException : public std::exception
{
public:
    MessageBuilderException(const char *msg_) : msg(msg_){}
    const char *what(){return msg;}

private:
    const char *msg;
};

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
        auto msg = std::make_shared<MqttMessage>();

        try{
            buildRootJsonObject();
            buildDataJsonObject();

            cJSON_AddItemToObject(getRootJsonObject(), "data", getDataJsonObject());

            
            msg->topic = buildTopic();
            msg->qos = getQos();

            _build(msg);

        }catch(MessageJsonException &ex){
            ESP_LOGE(TAG, "MessageJsonException: [%s]", ex.what());
            clearRootJsonObject();
            throw MessageBuilderException(ex.what());
        }catch(MessageTopicProcessorException &ex){
            ESP_LOGE(TAG, "MessageTopicProcessorException: [%s]", ex.what());
            clearRootJsonObject();
            throw MessageBuilderException(ex.what());
        }
        
        clearRootJsonObject();
        return msg;
    }

protected:
    virtual void _build(std::shared_ptr<MqttMessage> msg) = 0;

    std::string buildTopic(){
        return MessageTopicProcessor::build(getTopic());
    }

private:
    static constexpr const char *TAG = "MessageBuilder";

};
