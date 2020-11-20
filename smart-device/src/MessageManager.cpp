#include "SmartDevice/MessageManager.hpp"


static const char *TAG = "MessageManager";

MessageManager::MessageManager()
    : RoutineTask(TAG, 3, TaskDelay, 1024 *4)
{
    _xMessagesToSendMutex = xSemaphoreCreateMutex();
    if(_xMessagesToSendMutex == NULL)
    {
        ESP_LOGE(TAG, "Cannot create mutex, ho heap left...");
    }
}

void MessageManager::setMessageAppender(std::function<void(std::shared_ptr<MqttMessage>)> messageAppender)
{
    _messageAppender = messageAppender;
}

void MessageManager::registerRoutineMessage(std::shared_ptr<MessageBuilder> messageBuilder, int delay)
{
    SemaphoreGuard lock(_xMessagesToSendMutex);
    _routineMessages.push_back({ messageBuilder, delay });
}

void MessageManager::initTask()
{
    ESP_LOGD(TAG, "Registered messages: [%d]", _routineMessages.size());
}

void MessageManager::process(std::shared_ptr<MqttMessage> msg)
{
    //decompose topic
    auto mt = _messageTopicProcessor.parse(msg->topic);

    //find message by command and subcommand
    auto it = std::find_if(_messageHandlers.begin(), _messageHandlers.end(), [&](const std::shared_ptr<MessageHandler> handler){
        return mt == handler->getTopic();
    });

    if(it == _messageHandlers.end())
    {
        ESP_LOGW(TAG, "No handler found for topic: [%s]", msg->topic.c_str());
        return;
    }

    //process message
    ESP_LOGI(TAG, "Handling message: topic=[%s], data=[%s], qos=[%d]", msg->topic.c_str(), msg->data.c_str(), msg->qos);
    (*it)->setTopic(mt);
    (*it)->handle(msg->data);
}

void MessageManager::task()
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
