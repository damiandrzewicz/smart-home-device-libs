#include "SmartDevice/DeviceCore.hpp"

#include <functional>

#include "esp_log.h"

#include "SystemUtils/Utils.hpp"

#include "RtosUtils/MapTaskSafe.hpp"

#include "SmartMessage/MessageTopicProcessor.hpp"
#include "SmartMessage/MessageDomain.hpp"

#include "BaseSmartMessage/NotifyDeviceAvailableBuilder.hpp"
#include "BaseSmartMessage/RegisterDeviceBuilder.hpp"

static const char *TAG = "SmartDeviceCore";

namespace SmartDevice
{
    DeviceCore &DeviceCore::getInstance()
    {
        static DeviceCore kernel;
        return kernel;
    }

    DeviceCore::DeviceCore()
        :   RoutineTask("SmartDevice:Kernel", 5, 500, 1024 * 8)
    {
        initLogMessageRouter();
    }

    DeviceCore::~DeviceCore(){}

    OtaTask &DeviceCore::getOtaTask()
    {
        return _otaTask;
    }

    MqttTask &DeviceCore::getMqttTask()
    {
        return _mqttTask;
    }

    DeviceInfo &DeviceCore::getDeviceInfo()
    {
        return _deviceInfo;
    }

    void DeviceCore::initLogMessageRouter()
    {
        //TODO
        // _systemLogMessageRouter.appendRouteCallback([&](const char *buffer){});
        _systemLogMessageRouter.init();
    }

    void DeviceCore::printSystemInfo()
    {
        ESP_LOGI(TAG, "Starting...");
        ESP_LOGI(TAG, "System info:\n\tESP-IDF version: [%s]\n\tAPP Version: [%s]\n\tMAC: [%s]\n\tClientId: [%s]", 
            getDeviceInfo().appDescriptor.idfVersion.c_str(),
            getDeviceInfo().appDescriptor.appVersion.c_str(),
            getDeviceInfo().mac.c_str(),
            getDeviceInfo().clientId.c_str());
    }

    void DeviceCore::initDeviceInfo()
    {
        if(getDeviceInfo().deviceType == DeviceType::Value::Unknown)
        {
            ESP_LOGE(TAG, "DeviceType: [Unknown]. Stopping kernel init...");
            deleteTask();
        }
        getDeviceInfo().appDescriptor = System::Utils::EspIdf::GetAppDescriptor();
        getDeviceInfo().chipInfo = System::Utils::EspIdf::GetChipInfo();

        getDeviceInfo().mac = System::Utils::MAC::GetClientMac();
        getDeviceInfo().clientId = System::Utils::MAC::GetClientId();
    }

    void DeviceCore::initMemoryDaemon()
    {
        ESP_LOGI(TAG, "Starting  memory daemon...");
        _memoryDaemon.setUpdateMemoryStateFunction([&](int currentHeapSize, int minHeapSize){
            getDeviceInfo().deviceMemory.setCurrentFreeHeap(currentHeapSize);
            getDeviceInfo().deviceMemory.setMinHeapFree(minHeapSize);
        });
        _memoryDaemon.start();
    }

    void DeviceCore::initNetwork()
    {
        ESP_LOGI(TAG, "Starting Wifi...");
        _station.start();
        _station.isConnected(true); //wait for connection
    }

    void DeviceCore::performOta()
    {
        ESP_LOGI(TAG, "Performing OTA...");

        _otaTask.setBinaryName(getDeviceInfo().deviceType.toString());
        _otaTask.start();
        _otaTask.waitForExecuted();
    }

    void DeviceCore::initMqtt()
    {
        _mqttTask.setClientId(System::Utils::MAC::GetClientId());
        _mqttTask.setMessageProcessor(std::bind(&MessageManager::process, &_messageManager, std::placeholders::_1));

        _mqttTask.setOnConnectedCallback([&](){
            _mqttTask.appendMessage(BaseSmartMessage::RegisterDeviceBuilder().build());
        });

        _mqttTask.start();
        _mqttTask.waitForInitialized();
    }

    void DeviceCore::registerMessageHandlers()
    {
        //_messageManager.registerMessage()
    }

    void DeviceCore::registerMessageSenders()
    {
        _routineMessageSenderTask.setMessageAppender(std::bind(&MqttTask::appendMessage, &_mqttTask, std::placeholders::_1));
        _routineMessageSenderTask.registerRoutineMessage( std::make_shared<BaseSmartMessage::NotifyDeviceAvailableBuilder>(), 5000 );
        _routineMessageSenderTask.start();
    }

    void DeviceCore::initTask()
    {
        ESP_LOGI(TAG, "DeviceCore Initialising...");

        initDeviceInfo();

        printSystemInfo();

        initMemoryDaemon();

        initNetwork();

        performOta();

        registerMessageHandlers();

        initMqtt();

        registerMessageSenders();

        ESP_LOGI(TAG, "DeviceCore ready!");
    }

    void DeviceCore::task()
    {
        // while(1)
        // {
        //     vTaskDelay(pdMS_TO_TICKS(500));
        // }
    }
}