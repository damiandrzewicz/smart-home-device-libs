#pragma once
#include "esp_log.h"

#include <map>
#include <memory>


#include "RtosUtils/SemaphoreGuard.hpp"


static SemaphoreHandle_t xMutex;

template<class Key, class Value>
class MapTaskSafe 
{
public:
    MapTaskSafe()
    {
        xMutex = xSemaphoreCreateMutex();
        if(xMutex == NULL)
        {
            ESP_LOGE(TAG, "Cannot create mutex, ho heap left...");
        }
    }

    Value find(Key key)
    {
        SemaphoreGuard lock(xMutex);
        auto message = _queue.find(key);
        bool found = message != _queue.end();
        return (found) ? message->second : nullptr;
    }

    Value extract(Key key)
    {
        SemaphoreGuard lock(xMutex);
        auto nh = _queue.extract(key);
        return (!nh.empty()) ? nh.mapped() : nullptr;
    }

    Value insert(std::pair<Key, Value> msg)
    {
        SemaphoreGuard lock(xMutex);
        _queue.insert(msg);
        return msg.second;
    }

    Value at(Key key)
    {
        SemaphoreGuard lock(xMutex);
        return _queue.at(key);
    }

    int size() const
    {
        SemaphoreGuard lock(xMutex);
        return _queue.size();
    }

private:
    const char *TAG = "MessageQueue";
    std::map<Key, Value> _queue;
};