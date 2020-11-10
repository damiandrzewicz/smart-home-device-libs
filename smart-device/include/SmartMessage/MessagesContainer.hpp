#pragma once

#include <memory>

#include "RtosUtils/VectorTaskSafe.hpp"
#include "Mqtt/MessageIn.hpp"
#include "Mqtt/MessageOut.hpp"

class MessagesContainer
{
public:

    typedef VectorTaskSafe<std::shared_ptr<MessageIn>> IncomingMessages;
    typedef VectorTaskSafe<std::shared_ptr<MessageOut>> OutcomingMessages;

    IncomingMessages &getIncomingMessages();
    OutcomingMessages &getOutcomingMessages();

private:
    IncomingMessages _incomingMessages;
    OutcomingMessages _outcomingMessages;
};