#pragma once

#include <string>

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
    unsigned int sendDelayMs = 0;
};