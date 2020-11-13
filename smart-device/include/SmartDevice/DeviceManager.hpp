#pragma once

#include <string>

#include "SmartDevice/DeviceType.hpp"
#include "SmartDevice/FirmwareVersion.hpp"
#include "SmartDevice/DeviceState.hpp"
#include "SmartDevice/DeviceMemory.hpp"

namespace SmartDevice
{
    class DeviceManager
    {
    public:
        auto &getDeviceType(){return _deviceType;}
        auto &getFirmwareVersion(){return _version;}
        auto &getDeviceState(){return _deviceState;}
        auto &getDeviceMemory(){return _deviceMemory;}
    
    private:
        DeviceType _deviceType;
        FirmwareVersion _version;
        DeviceState _deviceState;
        DeviceMemory _deviceMemory;
    };
};
