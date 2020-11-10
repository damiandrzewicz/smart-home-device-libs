#include "SmartMessage/IncomingMessageDispatcher.hpp"


#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static const char *TAG = "IncomingMessageDispatcher";

IncomingMessageDispatcher::IncomingMessageDispatcher(MessagesContainer::IncomingMessages &incomingMessages)
    :   RoutineTask("SmartDevice:IncomingMessageDispatcher", 4, 500, 1024 * 4),
        _incomingMessages(incomingMessages)
{
    
}

void IncomingMessageDispatcher::initTask()
{
    ESP_LOGD(TAG, "Starting IncomingMessageDispatcher loop...");
}

void IncomingMessageDispatcher::task()
{
    ESP_LOGD(TAG, "before iter: _incomingMessages.size=%d", _incomingMessages.size());
    for(auto it = _incomingMessages.begin(); it != _incomingMessages.end();)
    {
        if((*it)->ready)
        {
            ESP_LOGD(TAG, "Processing msg (id: %d), msg size: %d", (*it)->id, (*it)->data.length());
            processMessage(*it);

            it = _incomingMessages.erase(it);
        }
        else{ ++it; }
    }
    ESP_LOGD(TAG, "after iter: _incomingMessages.size=%d", _incomingMessages.size());

}   

void IncomingMessageDispatcher::processMessage(auto message)
{

}