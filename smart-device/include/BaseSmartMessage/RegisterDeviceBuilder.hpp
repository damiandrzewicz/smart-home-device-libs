#pragma once

#include "SmartMessage/MessageBuilder.hpp"

#include "SmartDevice/DeviceCore.hpp"

namespace BaseSmartMessage
{
    class RegisterDeviceBuilder : public MessageBuilder
    {
    public:
        RegisterDeviceBuilder()
        {
            setQos(1);
            getTopic().setCommand("RegisterDevice");
        }

        virtual ~RegisterDeviceBuilder()
        {
            
        }

        virtual void _build(std::shared_ptr<MqttMessage> msg) override{
            const auto &appDescriptor = SmartDevice::DeviceCore::getInstance().getDeviceInfo().appDescriptor;
            const auto &chipInfo = SmartDevice::DeviceCore::getInstance().getDeviceInfo().chipInfo;
            const auto &deviceType = SmartDevice::DeviceCore::getInstance().getDeviceInfo().deviceType;

            auto jAppDescriptor = cJSON_CreateObject();
            cJSON_AddStringToObject(jAppDescriptor, "appVersion", appDescriptor.appVersion.c_str());
            cJSON_AddItemToObject(getDataJsonObject(), "appDescriptor", jAppDescriptor);

            auto jChipInfo = cJSON_CreateObject();
            cJSON_AddStringToObject(jChipInfo, "chipModel", chipInfo.chipModel.c_str());
            cJSON_AddStringToObject(jChipInfo, "coresCPU", chipInfo.coresCPU.c_str());
            cJSON_AddStringToObject(jChipInfo, "revisionNumber", chipInfo.revisionNumber.c_str());
            cJSON_AddItemToObject(getDataJsonObject(), "chipInfo", jChipInfo);

            cJSON_AddStringToObject(getDataJsonObject(), "deviceType", deviceType.toString());

            msg->data = getStringUnformatted();
        }
    };
};

