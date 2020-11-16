#include "SmartDevice/RoutineMessageSenderTask.hpp"


static const char *TAG = "RoutineMessageSenderTask";

RoutineMessageSenderTask::RoutineMessageSenderTask()
    : RoutineTask(TAG, 3, TaskDelay, 1024 *4)
{
    _xMessagesToSendMutex = xSemaphoreCreateMutex();
    if(_xMessagesToSendMutex == NULL)
    {
        ESP_LOGE(TAG, "Cannot create mutex, ho heap left...");
    }
}

void RoutineMessageSenderTask::setMessageAppender(std::function<void(std::shared_ptr<MqttMessage>)> messageAppender)
{
    _messageAppender = messageAppender;
}

void RoutineMessageSenderTask::registerRoutineMessage(std::function<std::shared_ptr<MqttMessage>()> routineMessageBuilder, int delay)
{
    SemaphoreGuard lock(_xMessagesToSendMutex);
    _routineMessages.push_back({ routineMessageBuilder, delay });
}

void RoutineMessageSenderTask::task()
{
    SemaphoreGuard lock(_xMessagesToSendMutex);

    ESP_LOGD(TAG, "Buffer size before: [%d]", _routineMessages.size());

    for(auto &rmbd : _routineMessages)
    {
        if(rmbd.getDelay() <= TaskDelay * rmbd.getCurrentRoutineTick())
        {
            auto build = rmbd.getRoutineMessageBuilder();
            if(_messageAppender)
            {
                _messageAppender(build());
            }

            rmbd.resetCurrentRoutineTick();
        }
        else
        {
            rmbd.incrementCurrentRoutineTick();
        }
    }

    ESP_LOGD(TAG, "Buffer size after: [%d]", _routineMessages.size());
}
