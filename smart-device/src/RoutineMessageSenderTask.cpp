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

void RoutineMessageSenderTask::registerRoutineMessage(std::shared_ptr<MessageBuilder> messageBuilder, int delay)
{
    SemaphoreGuard lock(_xMessagesToSendMutex);
    _routineMessages.push_back({ messageBuilder, delay });
}

void RoutineMessageSenderTask::initTask()
{
    ESP_LOGD(TAG, "Registered messages: [%d]", _routineMessages.size());
}

void RoutineMessageSenderTask::task()
{
    SemaphoreGuard lock(_xMessagesToSendMutex);

    for(auto &rmbd : _routineMessages)
    {
        if(rmbd.getDelay() <= TaskDelay * rmbd.getCurrentRoutineTick())
        {
            auto builder = rmbd.getRoutineMessageBuilder();
            if(_messageAppender)
            {
                _messageAppender(builder->build());
            }

            rmbd.resetCurrentRoutineTick();
        }
        else
        {
            rmbd.incrementCurrentRoutineTick();
        }
    }
}
