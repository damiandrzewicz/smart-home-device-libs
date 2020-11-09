#pragma once

#include "Message.hpp"

class MessageIn : public Message 
{
public:
    MessageIn() : Message(){}
    MessageIn(int id) : Message(id){}
};