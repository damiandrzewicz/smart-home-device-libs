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
    class DeviceCore : public RoutineTask
    {
    public:

        static DeviceCore &getInstance();

        // disable copy/move -- this is a Singleton
        DeviceCore(const DeviceCore&) = delete;
        DeviceCore(DeviceCore&&) = delete;
        DeviceCore& operator=(const DeviceCore&) = delete;
        DeviceCore& operator=(DeviceCore&&) = delete;

        OtaTask &getOtaTask();

        MqttTask &getMqttTask();

        DeviceInfo &getDeviceInfo();

    private:
        DeviceCore();
        ~DeviceCore();

    protected:
        virtual void initTask() override;

        virtual void task() override;

    private:
        void initLogMessageRouter();

        void printSystemInfo();

        void initDeviceInfo();

        void initMemoryDaemon();

        void initNetwork();

        void performOta();

        void initMqtt();

        void registerMessageHandlers();

        void registerMessageSenders();

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

