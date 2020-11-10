#include "SmartMessage/OutcomingMessageDispatcher.hpp"

#include "esp_log.h"

static const char *TAG = "OutcomingMessageDispatcher";

OutcomingMessageDispatcher::OutcomingMessageDispatcher(MessagesContainer::OutcomingMessages &outcomingMessages)
    :   RoutineTask("SmartDevice:OutcomingMessageDispatcher", 4, 500, 1024 * 4),
        _outcomingMessages(outcomingMessages)
{

}

void OutcomingMessageDispatcher::setSender(MqttSender sender)
{
    _sender = sender;
}

void OutcomingMessageDispatcher::initTask()
{

}

void OutcomingMessageDispatcher::task()
{
    ESP_LOGD(TAG, "before iter: _outcomingMessages.size=%d", _outcomingMessages.size());
    for(auto it = _outcomingMessages.begin(); it != _outcomingMessages.end();)
    {
        process(*it);
        ESP_LOGD(TAG, "Message (id: %d) sent, msg size: %d", (*it)->id, (*it)->data.length());
        it = _outcomingMessages.erase(it);
    }
    ESP_LOGD(TAG, "after iter: _outcomingMessages.size=%d", _outcomingMessages.size());
}

void OutcomingMessageDispatcher::process(auto message)
{
    _sender(message);
}