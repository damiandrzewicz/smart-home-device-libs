#pragma once

#include <string>

#include "SmartDevice/DeviceType.hpp"
#include "SmartDevice/DeviceState.hpp"
#include "SmartDevice/DeviceMemory.hpp"
#include "SystemUtils/AppDescriptor.hpp"
#include "SystemUtils/ChipInfo.hpp"
#include "SmartMessage/MessageDomain.hpp"

namespace SmartDevice
{
    struct DeviceInfo
    {
        DeviceType deviceType = DeviceType::Unknown;
        AppDescriptor appDescriptor;
        ChipInfo chipInfo;
        DeviceState deviceState;
        DeviceMemory deviceMemory;

        std::string mac;
        std::string clientId;
    };
};
