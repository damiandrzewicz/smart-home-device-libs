#pragma once

#include "SystemUtils/SystemLogMessageRouter.hpp"
#include "RtosUtils/SingleShootTask.hpp"
#include "SmartDevice/MemoryDaemon.hpp"
#include "Network/WifiStation.hpp"
#include "Ota/OtaTask.hpp"
#include "Mqtt/MqttTask.hpp"
#include "SmartDevice/MessageManager.hpp"
#include "SmartDevice/DeviceInfo.hpp"


namespace SmartDevice
{
    class DeviceCore : public SingleShootTask
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

        MessageManager &getMessageManager();

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

        void initMessageManager();

        void startMessageManager();

    private:
        SystemLogMessageRouter _systemLogMessageRouter;

        MemoryDaemon _memoryDaemon;
        WifiStation _station;
        OtaTask _otaTask;
        MqttTask _mqttTask;

        MessageManager _messageManager;
        DeviceInfo _deviceInfo;
    };
};

