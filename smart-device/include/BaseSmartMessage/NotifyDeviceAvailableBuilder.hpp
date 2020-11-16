#pragma once

#include "SmartMessage/MessageBuilder.hpp"

#include "SmartDevice/Kernel.hpp"

namespace BaseSmartMessage
{
    class NotifyDeviceAvailableBuilder : public MessageBuilder
    {
    public:
        NotifyDeviceAvailableBuilder()
            : MessageBuilder(1, "NotifyDeviceAvailable")
        {
            
        }

        virtual void _build(std::shared_ptr<MqttMessage> msg) override
        {
            auto deviceMemory = SmartDevice::Kernel::getInstance().getDeviceInfo().deviceMemory;
            auto jDeviceMemory = cJSON_CreateObject();
            cJSON_AddNumberToObject(jDeviceMemory, "currentFreeHeap", deviceMemory.getCurrentFreeHeapPercent());
            cJSON_AddNumberToObject(jDeviceMemory, "minHeapFree", deviceMemory.getMinHeapFreePercent());
            cJSON_AddItemToObject(getDataJsonObject(), "deviceMemory", jDeviceMemory);

            msg->data = getStringUnformatted();
        }

    private:

    };
};
