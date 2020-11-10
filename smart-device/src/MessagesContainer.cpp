#include "SmartMessage/MessagesContainer.hpp"

MessagesContainer::IncomingMessages &MessagesContainer::getIncomingMessages()
{
    return _incomingMessages;
}

MessagesContainer::OutcomingMessages &MessagesContainer::getOutcomingMessages()
{
    return _outcomingMessages;
}