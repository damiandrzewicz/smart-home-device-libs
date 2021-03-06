#include "SmartDevice/DeviceCore.hpp"

#include <functional>

#include "esp_log.h"

#include "SystemUtils/Utils.hpp"

#include "RtosUtils/MapTaskSafe.hpp"

#include "SmartMessage/MessageTopicProcessor.hpp"
#include "SmartMessage/MessageDomain.hpp"

#include "BaseSmartMessage/NotifyDeviceAvailableBuilder.hpp"
#include "BaseSmartMessage/RegisterDeviceBuilder.hpp"

#include "BaseSmartMessage/TestHandler.hpp"

static const char *TAG = "SmartDeviceCore";

namespace SmartDevice
{
    DeviceCore &DeviceCore::getInstance()
    {
        static DeviceCore kernel;
        return kernel;
    }

    DeviceCore::DeviceCore()
        :   SingleShootTask("SmartDevice:Kernel", 5, 1024 * 8)
    {
        initLogMessageRouter();

        _messageManager.setMessageAppender(std::bind(&MqttTask::appendMessage, &_mqttTask, std::placeholders::_1));
        _messageManager.setSubscribtionAppender(std::bind(&MqttTask::appendSubscribtion, &_mqttTask, std::placeholders::_1));
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

    MessageManager &DeviceCore::getMessageManager()
    {
        return _messageManager;
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
        ESP_LOGI(TAG, "System info:\n\tESP-IDF version: [%s]\n\tAPP Version: [%s]\n\tMAC: [%s]\n\tClientId: [%s]\n\tChip info:(Model: [%s], cores: [%s], rev no: [%s]) ", 
            getDeviceInfo().appDescriptor.idfVersion.c_str(),
            getDeviceInfo().appDescriptor.appVersion.c_str(),
            getDeviceInfo().mac.c_str(),
            getDeviceInfo().clientId.c_str(),
            getDeviceInfo().chipInfo.chipModel.c_str(),
            getDeviceInfo().chipInfo.coresCPU.c_str(),
            getDeviceInfo().chipInfo.revisionNumber.c_str());

        ESP_LOGI(TAG, "Device type: [%s]", 
            getDeviceInfo().deviceType.toString());
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

    void DeviceCore::initMessageManager()
    {
 

        _messageManager.registerRoutineMessage( std::make_shared<BaseSmartMessage::NotifyDeviceAvailableBuilder>(), 5000 );
        _messageManager.registerMessageHandler(std::make_shared<TestHandler>());
    }

    void DeviceCore::startMessageManager()
    {
        _messageManager.start();
    }

    void DeviceCore::initTask()
    {
        ESP_LOGI(TAG, "DeviceCore Initialising...");

        initDeviceInfo();

        printSystemInfo();

        initMemoryDaemon();

        initNetwork();

        performOta();

        initMessageManager();

        initMqtt();

        startMessageManager();

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