#pragma once

#include <memory>
#include <functional>
#include <list>

#include "esp_log.h"

#include "RtosUtils/SemaphoreGuard.hpp"

#include "RtosUtils/RoutineTask.hpp"

#include "Mqtt/MqttMessage.hpp"

#include "SmartMessage/MessageBuilder.hpp"




class RoutineMessageSenderTask : public RoutineTask
{
    class RoutineMessageBuilderData{
    public:
        RoutineMessageBuilderData(std::shared_ptr<MessageBuilder> messageBuilder_, int delay_ = 0)
            : messageBuilder(messageBuilder_), delay(delay_)
            {}

        void resetCurrentRoutineTick(){currentRoutineTick = 1;}
        void incrementCurrentRoutineTick(){currentRoutineTick++;}

        auto getRoutineMessageBuilder(){return messageBuilder;}
        auto getDelay(){return delay;}
        auto getCurrentRoutineTick(){return currentRoutineTick;}

        void pirint(){ESP_LOGV("RoutineMessageBuilderData", "delay: %d, currentRoutineTick: %d", delay, currentRoutineTick); }

    private:
        //std::function<std::shared_ptr<MqttMessage>()> routineMessageBuilder;
        std::shared_ptr<MessageBuilder> messageBuilder;
        int delay;
        int currentRoutineTick = 1;
    };

public:
    RoutineMessageSenderTask();

    void setMessageAppender(std::function<void(std::shared_ptr<MqttMessage>)> _messageAppender);

    void registerRoutineMessage(std::shared_ptr<MessageBuilder> messageBuilder, int delay = 0);

protected:
    virtual void initTask() override;

    virtual void task() override;

private:

    std::function<void(std::shared_ptr<MqttMessage>)> _messageAppender;

    std::list<RoutineMessageBuilderData> _routineMessages;
    SemaphoreHandle_t _xMessagesToSendMutex;

    static constexpr const int TaskDelay = 200;
};

