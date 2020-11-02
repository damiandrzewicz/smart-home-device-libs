#include "SmartDevice/Kernel.hpp"

#include "esp_log.h"

#include "SystemUtils/Utils.hpp"

static const char *TAG = "Kernel";

namespace SmartDevice
{
    Kernel::Kernel()
        : RoutineTask("SmartDevice:Kernel", 5, 500, 1024 * 8)
    {

    }

    void Kernel::printSystemInfo()
    {
        ESP_LOGI(TAG, "Starting...");
        ESP_LOGI(TAG, "System info:\n\tESP-IDF version: [%s]\n\tMAC: [%s]", 
            System::Utils::EspIdf::GetSdkVersion().c_str(),
            System::Utils::MAC::GetClientMac().c_str());
    }

    void Kernel::initMemoryDaemon()
    {
        ESP_LOGI(TAG, "Starting  memory daemon...");
        _memoryDaemon.start();
    }

    void Kernel::initNetwork()
    {
        ESP_LOGI(TAG, "Starting Wifi...");
        _station.start();
        _station.isConnected(true); //wait for connection
    }

    void Kernel::performOta()
    {
        ESP_LOGI(TAG, "Performing OTA...");
    }

    void Kernel::initTask()
    {
        ESP_LOGI(TAG, "Kernel initialising...");

        printSystemInfo();

        initMemoryDaemon();

        initNetwork();

        performOta();

        ESP_LOGI(TAG, "Kernel ready!");
    }

    void Kernel::task()
    {
        // while(1)
        // {
        //     vTaskDelay(pdMS_TO_TICKS(500));
        // }
    }
}