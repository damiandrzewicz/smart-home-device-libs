#pragma once

#include <string>

#include "esp_log.h"

class MqttMessage
{
public:
    MqttMessage(){}
    MqttMessage(int id){ this->id = id; }

    int id = -1;
    std::string data;
    std::string topic;
    uint8_t qos;
    bool retain = false;
    bool ready = false;
    int timesNotReady = 0;

    void print(){
        ESP_LOGD("MqttMessage", "id=[%d], data=[%s], topic=[%s], qos=[%d], retain=[%d], ready=[%d], timesNotReady=[%d]",
            id, data.c_str(), topic.c_str(), qos, retain, ready, timesNotReady);
    }
};