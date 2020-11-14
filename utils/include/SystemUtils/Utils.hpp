#pragma once

#include <vector>
#include <string>

#include "SystemUtils/AppDescriptor.hpp"
#include "SystemUtils/ChipInfo.hpp"

namespace System
{
    namespace Utils
    {
        namespace MAC
        {
            std::vector<uint8_t> GetMacRaw();

            std::string GetClientMac();

            std::string GetClientId();
        }

        namespace EspIdf
        {
            std::string GetSdkVersion();

            ChipInfo GetChipInfo();

            AppDescriptor GetAppDescriptor();
        }

        namespace Memory
        {
            int GetFreeHeapSize(); 

            int GetMinimimFreeHeapSize();
        }
    }

};