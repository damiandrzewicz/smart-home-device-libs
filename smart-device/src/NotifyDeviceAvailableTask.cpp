#include "SmartDevice/NotifyDeviceAvailableTask.hpp"

#include "BaseSmartMessage/NotifyDeviceAvailable.hpp"


NotifyDeviceAvailableTask::NotifyDeviceAvailableTask()
    : RoutineTask("NotifyDeviceAvailableTask", 3, 10000, 1024 *4)
{
    
}

void NotifyDeviceAvailableTask::setAppender(std::function<void(std::shared_ptr<MqttMessage>)> appender)
{
    _appender = appender;
}

void NotifyDeviceAvailableTask::task()
{
    printf("NotifyDeviceAvailableTask::task() exec...");
    BaseSmartMessage::NotifyDeviceAvailable nda;
    auto message = nda.build();

    if(_appender)
        _appender(message);
}
