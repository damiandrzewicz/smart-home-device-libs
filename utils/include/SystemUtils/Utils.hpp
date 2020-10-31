#pragma once

#include <vector>
#include <string>

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
        }

        namespace Memory
        {
            int GetFreeHeapSize(); 

            int GetMinimimFreeHeapsize();
        }
    }

};