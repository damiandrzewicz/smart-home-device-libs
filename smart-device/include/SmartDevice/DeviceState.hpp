#pragma once

#include <string>
#include "RtosUtils/SemaphoreGuard.hpp"

#include "esp_log.h"



namespace SmartDevice
{
    class DeviceState
    {
    public:
        DeviceState(){
            xMutex = xSemaphoreCreateMutex();
            if(xMutex == NULL)
            {
                ESP_LOGE(TAG, "Cannot create mutex, ho heap left...");
            }
        }
        void setLastError(auto error){ 
            SemaphoreGuard lock(xMutex);
            _lastError = error; 
        }

        auto getLastError(){
            SemaphoreGuard lock(xMutex);
            return _lastError;
        }

    private:
        constexpr static const char *TAG = "DeviceState";

        std::string _lastError;
        SemaphoreHandle_t xMutex;
    };
};