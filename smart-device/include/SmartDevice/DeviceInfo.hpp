#pragma once

#include <string>

#include "SmartDevice/DeviceType.hpp"
#include "SmartDevice/DeviceMemory.hpp"
#include "SystemUtils/AppDescriptor.hpp"
#include "SystemUtils/ChipInfo.hpp"
#include "SmartMessage/MessageDomain.hpp"

namespace SmartDevice
{
    struct DeviceInfo
    {
        DeviceType deviceType;
        AppDescriptor appDescriptor;
        ChipInfo chipInfo;
        DeviceMemory deviceMemory;

        std::string mac;
        std::string clientId;
    };
};
