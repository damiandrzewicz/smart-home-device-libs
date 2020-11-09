#include "SmartDevice/IncomingMessageDispatcher.hpp"

#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static const char *TAG = "IncomingMessageDispatcher";

IncomingMessageDispatcher::IncomingMessageDispatcher()
    : RoutineTask("SmartDevice:IncomingMessageDispatcher", 4, 500, 1024 * 4)
{
    
}

// void IncomingMessageDispatcher::registerMap(std::shared_ptr<MapTaskSafe<int, std::shared_ptr<MessageIn>>> map)
// {
//     _messageMap = map;
// }

void IncomingMessageDispatcher::registerIncomingMessages(VectorTaskSafe<std::shared_ptr<MessageIn>> *incomingMessages)
{
    _incomingMessages = incomingMessages;
}

void IncomingMessageDispatcher::initTask()
{
    //if(!_messageMap)
    //{
    //     ESP_LOGE(TAG, "Cannot start IncomingMessageDispatcher, map null");
    //     vTaskDelete(NULL);
    // }

    ESP_LOGD(TAG, "Starting IncomingMessageDispatcher loop...");
}

void IncomingMessageDispatcher::task()
{
    ESP_LOGD(TAG, "before iter: _incomingMessages.size=%d", _incomingMessages->size());
    for(auto it = _incomingMessages->begin(); it != _incomingMessages->end();)
    {
        if((*it)->ready)
        {
            ESP_LOGD(TAG, "Processing msg (id: %d), msg size: %d", (*it)->id, (*it)->data.length());
            it = _incomingMessages->erase(it);
        }
        else{ ++it; }
    }
    ESP_LOGD(TAG, "after iter: _incomingMessages.size=%d", _incomingMessages->size());

}   