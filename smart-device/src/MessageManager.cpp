#include "SmartMessage/MessageManager.hpp"

#include <algorithm>

#include "esp_log.h"

static const char *TAG = "MessageManager";

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
    (*it)->handle(msg->data);
}