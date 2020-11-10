#pragma once

#include <memory>

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
    void process(auto message);

private:
    MessagesContainer::IncomingMessages &_incomingMessages;
};