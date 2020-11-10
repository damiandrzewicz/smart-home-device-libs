#pragma once

#include <memory>
#include <functional>

#include "RtosUtils/RoutineTask.hpp"

#include "SmartMessage/MessagesContainer.hpp"

class OutcomingMessageDispatcher : public RoutineTask
{
public:
    typedef std::function<void(std::shared_ptr<MessageOut> message)> MqttSender;

    OutcomingMessageDispatcher(MessagesContainer::OutcomingMessages &outcomingMessages);

    void setSender(MqttSender sender);

protected:
    virtual void initTask() override;
    virtual void task() override;

private:
    void process(auto message);

private:
    MessagesContainer::OutcomingMessages &_outcomingMessages;

    MqttSender _sender;
};