#pragma once

#include <memory>
#include <vector>

#include "RtosUtils/RoutineTask.hpp"

#include "SmartMessage/MessagesContainer.hpp"


class IncomingMessageDispatcher : public RoutineTask
{
public:
    IncomingMessageDispatcher(MessagesContainer::IncomingMessages &incomingMessages);

protected:
    virtual void initTask() override;
    virtual void task() override;

private:

    void processMessage(auto message);

private:
    MessagesContainer::IncomingMessages &_incomingMessages;
};