#pragma once

#include "SystemUtils/SystemLogMessageRouter.hpp"
#include "RtosUtils/RoutineTask.hpp"
#include "SmartDevice/MemoryDaemon.hpp"
#include "Network/WifiStation.hpp"
#include "Ota/OtaTask.hpp"
#include "Mqtt/MqttTask.hpp"
#include "SmartMessage/MessageManager.hpp"
#include "SmartDevice/DeviceInfo.hpp"
#include "SmartDevice/RoutineMessageSenderTask.hpp"


namespace SmartDevice
{
    class Kernel : public RoutineTask
    {
    public:

        static Kernel &getInstance();

        // disable copy/move -- this is a Singleton
        Kernel(const Kernel&) = delete;
        Kernel(Kernel&&) = delete;
        Kernel& operator=(const Kernel&) = delete;
        Kernel& operator=(Kernel&&) = delete;

        OtaTask &getOtaTask();

        MqttTask &getMqttTask();

        DeviceInfo &getDeviceInfo();

    private:
        Kernel();
        ~Kernel();

    protected:
        virtual void initTask() override;

        virtual void task() override;

    private:
        void printSystemInfo();

        void initDeviceInfo();

        void initMemoryDaemon();

        void initNetwork();

        void performOta();

        void initMqtt();

        void initRootMessages();

    private:
        SystemLogMessageRouter _systemLogMessageRouter;

        MemoryDaemon _memoryDaemon;
        WifiStation _station;
        OtaTask _otaTask;
        MqttTask _mqttTask;
        RoutineMessageSenderTask _routineMessageSenderTask;

        MessageManager _messageManager;
        DeviceInfo _deviceInfo;
    };
};

