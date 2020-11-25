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

void MessageManager::setSubscribtionAppender(std::function<void(std::shared_ptr<MqttMessage>)> subscribtionAppender)
{
    _subscribtionAppender = subscribtionAppender;
}

void MessageManager::registerRoutineMessage(std::shared_ptr<MessageBuilder> messageBuilder, int delay)
{
    SemaphoreGuard lock(_xMessagesToSendMutex);
    _routineMessages.push_back({ messageBuilder, delay });
}

void MessageManager::registerMessageHandler(std::shared_ptr<MessageHandler> handler)
{
    _messageHandlers.push_back(handler);

    if(_subscribtionAppender)
    {
        auto msg = std::make_shared<MqttMessage>();
        msg->qos = handler->getQos();
        msg->topic = _messageTopicProcessor.build(handler->getTopic());

        _subscribtionAppender(msg);
    }
}

void MessageManager::initTask()
{
    ESP_LOGD(TAG, "Registered messages: [%d]", _routineMessages.size());
}

void MessageManager::process(std::shared_ptr<MqttMessage> msg)
{
    //decompose topic
    MessageTopic mt;
    try{
        mt = _messageTopicProcessor.parse(msg->topic);
    }catch(MessageTopicProcessorException &ex){
        ESP_LOGE(TAG, "MessageTopicProcessorException: [%s]", ex.what());
        return;
    }
    

    //find message by command and subcommand
    auto it = std::find_if(_messageHandlers.begin(), _messageHandlers.end(), [&](const std::shared_ptr<MessageHandler> handler){
        ESP_LOGD(TAG, "Comparing handlers: %s:%s = %s:%s", 
        mt.getDomaindIdentity().c_str(), mt.getCommandIdentity().c_str(), 
        handler->getTopic().getDomaindIdentity().c_str(),
        handler->getTopic().getCommandIdentity().c_str());

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
                try{
                    _messageAppender(builder->build());
                }catch(MessageBuilderException &ex){
                    ESP_LOGE(TAG, "MessageBuilderException: [%s]", ex.what());
                }
                
            }

            rmbd.resetCurrentRoutineTick();
        }
        else
        {
            rmbd.incrementCurrentRoutineTick();
        }
    }
}
