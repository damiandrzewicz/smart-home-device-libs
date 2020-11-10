#pragma once

#include "RtosUtils/RoutineTask.hpp"
#include "SmartDevice/MemoryDaemon.hpp"
#include "Network/WifiStation.hpp"
#include "Ota/OtaTask.hpp"
#include "Mqtt/MqttTask.hpp"
#include "SmartMessage/IncomingMessageDispatcher.hpp"
#include "SmartMessage/OutcomingMessageDispatcher.hpp"

#include "SmartMessage/MessagesContainer.hpp"


namespace SmartDevice
{
    class Kernel : public RoutineTask
    {
    public:
        Kernel();

        OtaTask &getOtaTask();
        IncomingMessageDispatcher &getIncomingMessageDispatcherTask();

    protected:
        virtual void initTask() override;

        virtual void task() override;

    private:
        void printSystemInfo();

        void initMemoryDaemon();

        void initNetwork();

        void performOta();

        void initMqtt();

        void initMessageDispatchers();

    private:
        MessagesContainer _messgesContainer;

        MemoryDaemon _memoryDaemon;
        WifiStation _station;
        OtaTask _otaTask;
        MqttTask _mqttTask;

        IncomingMessageDispatcher _incomingMessageDispatcherTask;
        OutcomingMessageDispatcher _outcomingMessageDispatcherTask;
    };
};

