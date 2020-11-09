#pragma once

#include <memory>

#include "RtosUtils/RoutineTask.hpp"
#include "RtosUtils/VectorTaskSafe.hpp"
#include "Mqtt/MessageIn.hpp"


class IncomingMessageDispatcher : public RoutineTask
{
public:
    IncomingMessageDispatcher();

    //void registerMap(std::shared_ptr<MapTaskSafe<int, std::shared_ptr<MessageIn>>> map);
    void registerIncomingMessages(VectorTaskSafe<std::shared_ptr<MessageIn>> *incomingMessages);

protected:
    virtual void initTask() override;
    virtual void task() override;

private:
    //std::shared_ptr<MapTaskSafe<int, std::shared_ptr<MessageIn>>> _messageMap;
    VectorTaskSafe<std::shared_ptr<MessageIn>> *_incomingMessages;
};