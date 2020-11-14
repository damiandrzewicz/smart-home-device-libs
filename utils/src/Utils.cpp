#include "SystemUtils/Utils.hpp"  
#include "esp_system.h"
#include "esp_idf_version.h"
#include "esp_ota_ops.h"

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

            ChipInfo GetChipInfo(){
                esp_chip_info_t chip_info;
                esp_chip_info(&chip_info);

                ChipInfo chipInfo;

                if(chip_info.model == CHIP_ESP32) chipInfo.chipModel = "CHIP_ESP32";
                //else if(chip_info.model == CHIP_ESP32S2) chipInfo.chipModel = "CHIP_ESP32S2";
                //else if(chip_info.model == CHIP_ESP32S3) chipInfo.chipModel = "CHIP_ESP32S3";

                chipInfo.coresCPU = std::to_string(chip_info.cores);
                chipInfo.revisionNumber = std::to_string(chip_info.revision);

                return chipInfo;
                
            }

            AppDescriptor GetAppDescriptor(){
                const esp_app_desc_t *appDesc = esp_ota_get_app_description();
                AppDescriptor descr;
                descr.appVersion = appDesc->version;
                descr.compileDate = appDesc->date;
                descr.compoleTime = appDesc->time;
                descr.idfVersion = appDesc->idf_ver;
                descr.projectName = appDesc->project_name;
                return descr;
            }
        }

        namespace Memory
        {
            int GetFreeHeapSize(){ return esp_get_free_heap_size(); }

            int GetMinimimFreeHeapSize(){ return esp_get_minimum_free_heap_size(); }

            //int GetLargestFreeHeapSize(){ return heap_caps_get_largest_free_block(); }
        }
    }

}