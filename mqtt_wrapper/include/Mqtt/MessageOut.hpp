#pragma once

#include "Message.hpp"

class MessageOut : public Message
{
public:
    uint8_t qos = 0;
    bool retain = false;
};