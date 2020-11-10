#include "SmartDevice/Kernel.hpp"

#include <functional>

#include "esp_log.h"

#include "SystemUtils/Utils.hpp"

#include "RtosUtils/MapTaskSafe.hpp"

static const char *TAG = "Kernel";

namespace SmartDevice
{
    Kernel::Kernel()
        :   RoutineTask("SmartDevice:Kernel", 5, 500, 1024 * 18),
            _mqttTask(_messgesContainer),
            _incomingMessageDispatcherTask(_messgesContainer.getIncomingMessages()),
            _outcomingMessageDispatcherTask(_messgesContainer.getOutcomingMessages())
    {
        
    }

    OtaTask &Kernel::getOtaTask()
    {
        return _otaTask;
    }

    IncomingMessageDispatcher &Kernel::getIncomingMessageDispatcherTask()
    {
        return _incomingMessageDispatcherTask;
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

        _otaTask.start();
        _otaTask.waitForExecuted();
    }

    void Kernel::initMqtt()
    {
        _mqttTask.setClientId(System::Utils::MAC::GetClientId());
        _mqttTask.start();
        _mqttTask.waitForInitialized();
    }

    void Kernel::initMessageDispatchers()
    {
        _incomingMessageDispatcherTask.start();
        _incomingMessageDispatcherTask.waitForInitialized();

        _outcomingMessageDispatcherTask.setSender(std::bind(&MqttTask::send, _mqttTask, std::placeholders::_1));
        _outcomingMessageDispatcherTask.start();
        _incomingMessageDispatcherTask.waitForInitialized();
    }

    void Kernel::initTask()
    {
        ESP_LOGI(TAG, "Kernel initialising...");

        printSystemInfo();

        initMemoryDaemon();

        initNetwork();

        performOta();

        initMqtt();

        initMessageDispatchers();

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