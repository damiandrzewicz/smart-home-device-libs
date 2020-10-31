#include "SystemUtils/Utils.hpp"  
#include "esp_system.h"

namespace System
{
    namespace Utils
    {
        namespace MAC
        {
            std::vector<uint8_t> GetMacRaw()
            {
                uint8_t mac[6];
                esp_efuse_mac_get_default(mac);
                return std::vector(std::begin(mac), std::end(mac));
            }

            std::string GetClientMac()
            {
                const auto &mac = GetMacRaw();
                char buffer[30];
                sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                return buffer;
            }

            std::string GetClientId()
            {
                const auto &mac = GetMacRaw();
                char buffer[30];
                sprintf(buffer, "esp32_%02x%02x%02x", mac[3], mac[4], mac[5]);
                return buffer;
            }
        }

        namespace EspIdf
        {
            std::string GetSdkVersion(){ return esp_get_idf_version(); }
        }

        namespace Memory
        {
            int GetFreeHeapSize(){ return esp_get_free_heap_size(); }

            int GetMinimimFreeHeapsize(){ return esp_get_minimum_free_heap_size(); }
        }
    }

}