#pragma once

#include <memory>
#include <functional>
#include <list>

#include "esp_log.h"

#include "RtosUtils/SemaphoreGuard.hpp"

#include "RtosUtils/RoutineTask.hpp"

#include "Mqtt/MqttMessage.hpp"

#include "SmartMessage/MessageBuilder.hpp"
#include "SmartMessage/MessageHandler.hpp"

#include "SmartMessage/MessageTopicProcessor.hpp"


class MessageManager : public RoutineTask
{
    class MessageBuilderData{
    public:
        MessageBuilderData(std::shared_ptr<MessageBuilder> messageBuilder_, int delay_ = 0)
            : messageBuilder(messageBuilder_), delay(delay_)
            {}

        void resetCurrentRoutineTick(){currentRoutineTick = 1;}
        void incrementCurrentRoutineTick(){currentRoutineTick++;}

        auto getRoutineMessageBuilder(){return messageBuilder;}
        auto getDelay(){return delay;}
        auto getCurrentRoutineTick(){return currentRoutineTick;}

        void pirint(){ESP_LOGV("MessageBuilderData", "delay: %d, currentRoutineTick: %d", delay, currentRoutineTick); }

        void process(std::shared_ptr<MqttMessage> msg);

    private:
        //std::function<std::shared_ptr<MqttMessage>()> routineMessageBuilder;
        std::shared_ptr<MessageBuilder> messageBuilder;
        int delay;
        int currentRoutineTick = 1;
    };

public:
    MessageManager();

    void setMessageAppender(std::function<void(std::shared_ptr<MqttMessage>)> messageAppender);

    void setSubscribtionAppender(std::function<void(std::shared_ptr<MqttMessage>)> messageSubscriber);

    void registerRoutineMessage(std::shared_ptr<MessageBuilder> messageBuilder, int delay = 0);

    void registerMessageHandler(std::shared_ptr<MessageHandler> handler);

    void process(std::shared_ptr<MqttMessage> msg);

protected:
    virtual void initTask() override;

    virtual void task() override;

private:

    std::function<void(std::shared_ptr<MqttMessage>)> _messageAppender;
    std::function<void(std::shared_ptr<MqttMessage>)> _subscribtionAppender;

    std::list<MessageBuilderData> _routineMessages;
    SemaphoreHandle_t _xMessagesToSendMutex;

    static constexpr const int TaskDelay = 200;

    std::list<std::shared_ptr<MessageHandler>> _messageHandlers;
    MessageTopicProcessor _messageTopicProcessor;
};

