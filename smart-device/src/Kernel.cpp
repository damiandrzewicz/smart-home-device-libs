#include "SmartDevice/Kernel.hpp"

#include "esp_log.h"

#include "SystemUtils/Utils.hpp"

static const char *TAG = "Kernel";

namespace SmartDevice
{
    Kernel::Kernel()
        : RoutineTask("SmartDevice:Kernel", 3, 500, 1024 * 8)
    {
        ESP_LOGI(TAG, "Starting...");
        ESP_LOGI(TAG, "System info:\n\tESP-IDF version: [%s]\n\tMAC: [%s]", 
            System::Utils::EspIdf::GetSdkVersion().c_str(),
            System::Utils::MAC::GetClientMac().c_str());


        _memoryDaemon.start();
    }

    void Kernel::task()
    {


        // while(1)
        // {
        //     vTaskDelay(pdMS_TO_TICKS(500));
        // }
    }
}