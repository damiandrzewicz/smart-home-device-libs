#pragma once

#include <memory>
#include <functional>

#include "RtosUtils/RoutineTask.hpp"

#include "Mqtt/MqttMessage.hpp"


class NotifyDeviceAvailableTask : public RoutineTask
{
public:
    NotifyDeviceAvailableTask();

    void setAppender(std::function<void(std::shared_ptr<MqttMessage>)> appender);

protected:
    virtual void task() override;

private:

    std::function<void(std::shared_ptr<MqttMessage>)> _appender;

};

