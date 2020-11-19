#pragma once

#include "SmartMessage/MessageBuilder.hpp"

#include "SmartDevice/DeviceCore.hpp"

namespace BaseSmartMessage
{
    class NotifyDeviceAvailableBuilder : public MessageBuilder
    {
    public:
        NotifyDeviceAvailableBuilder()
        {
            setQos(1);
            getTopic().setCommand("NotifyDeviceAvailable");
        }

        virtual void _build(std::shared_ptr<MqttMessage> msg) override
        {
            const char *timestamp = esp_log_system_timestamp();
            cJSON_AddStringToObject(getDataJsonObject(), "uptime", timestamp);

            auto deviceMemory = SmartDevice::DeviceCore::getInstance().getDeviceInfo().deviceMemory;
            auto jDeviceMemory = cJSON_CreateObject();
            cJSON_AddNumberToObject(jDeviceMemory, "currentFreeHeap", deviceMemory.getCurrentFreeHeap());
            cJSON_AddNumberToObject(jDeviceMemory, "minHeapFree", deviceMemory.getMinHeapFree());
            cJSON_AddItemToObject(getDataJsonObject(), "deviceMemory", jDeviceMemory);

            msg->data = getStringUnformatted();
        }

    private:

    };
};
