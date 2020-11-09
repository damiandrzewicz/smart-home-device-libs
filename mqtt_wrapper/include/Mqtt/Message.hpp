#pragma once

#include <string>

class Message
{
public:
    Message(){}
    Message(int id){ this->id = id; }

    int id = -1;
    std::string data;
    std::string topic;
    uint8_t qos;
    bool ready = false;
};