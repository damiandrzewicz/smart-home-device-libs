#include "SmartDevice/MemoryDaemon.hpp"

#include "esp_log.h"

#include "SystemUtils/Utils.hpp"

#include <vector>

static const char *TAG = "MemoryDaemon";

namespace SmartDevice
{
    MemoryDaemon::MemoryDaemon()
        : RoutineTask("MemoryDaemon", 1, 1000, 1024 *4)
    {
        
    }

    void MemoryDaemon::task()
    {
        uint32_t freeHeapSize_byte = System::Utils::Memory::GetFreeHeapSize();;
        uint32_t freeHeapSize_kbyte = freeHeapSize_byte / 1024;

        uint32_t minFreeHeapSize_byte = System::Utils::Memory::GetMinimimFreeHeapSize();;
        uint32_t minFreeHeapSize_kbyte = minFreeHeapSize_byte / 1024;

        uint32_t maxFreeHeapSize_byte = _maxHeapSize * 1024;
        uint32_t maxFreeHrapSize_kbyte = _maxHeapSize;

        uint8_t freeHeapSizePercent = (freeHeapSize_kbyte * 100 ) / maxFreeHrapSize_kbyte;

        ESP_LOGI(TAG, "Free heap memory size: [%d B (%d KB)] of [%d B (%d KB)] - (%d%%), minimal heap size occured: [%d B (%d KB)]", 
            freeHeapSize_byte,
            freeHeapSize_kbyte,
            maxFreeHeapSize_byte,
            maxFreeHrapSize_kbyte,
            freeHeapSizePercent,
            minFreeHeapSize_byte,
            minFreeHeapSize_kbyte);

        
    }
}