#pragma once

#include <string>

namespace SmartDevice
{
    struct FirmwareVersion
    {
        FirmwareVersion(){}

        void setSDKVersion(auto value){
            _sdkVersion = value;
        }

        void setOSVersion(auto value){
            _osVersion = value;
        }

        auto getSDKVersion(){
            return _sdkVersion;
        }

        auto getOSVersion(){
            return _osVersion;
        }

        std::string _sdkVersion;
        std::string _osVersion;
    };
};

