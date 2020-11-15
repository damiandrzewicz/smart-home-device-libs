#pragma once

#include <memory>
#include <functional>
#include <list>

#include "RtosUtils/SemaphoreGuard.hpp"

#include "RtosUtils/RoutineTask.hpp"

#include "Mqtt/MqttMessage.hpp"

#include "esp_log.h"


class RoutineMessageSenderTask : public RoutineTask
{
    class RoutineMessageBuilderData{
    public:
        RoutineMessageBuilderData(std::function<std::shared_ptr<MqttMessage>()> rmb_, int delay_ = 0)
            : routineMessageBuilder(rmb_), delay(delay_)
            {}

        void resetCurrentRoutineTick(){currentRoutineTick = 1;}
        void incrementCurrentRoutineTick(){currentRoutineTick++;}

        auto getRoutineMessageBuilder(){return routineMessageBuilder;}
        auto getDelay(){return delay;}
        auto getCurrentRoutineTick(){return currentRoutineTick;}

        void pirint(){ESP_LOGV("RoutineMessageBuilderData", "delay: %d, currentRoutineTick: %d", delay, currentRoutineTick); }

    private:
        std::function<std::shared_ptr<MqttMessage>()> routineMessageBuilder;
        int delay;
        int currentRoutineTick = 1;
    };

public:
    RoutineMessageSenderTask();

    void setMessageAppender(std::function<void(std::shared_ptr<MqttMessage>)> _messageAppender);

    void registerRoutineMessage(std::function<std::shared_ptr<MqttMessage>()> routineMessageBuilder, int delay = 0);

protected:
    virtual void task() override;

private:

    std::function<void(std::shared_ptr<MqttMessage>)> _messageAppender;

    std::list<RoutineMessageBuilderData> _routineMessages;
    SemaphoreHandle_t _xMessagesToSendMutex;

    static constexpr const int TaskDelay = 200;
};

