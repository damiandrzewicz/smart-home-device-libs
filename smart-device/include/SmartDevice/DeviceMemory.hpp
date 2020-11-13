#pragma once

#include <string>
#include "RtosUtils/SemaphoreGuard.hpp"

#include "esp_log.h"



namespace SmartDevice
{
    class DeviceMemory
    {
    public:
        DeviceMemory(){
            xMutexFreeHeap = xSemaphoreCreateMutex();
            if(xMutexFreeHeap == NULL)
            {
                ESP_LOGE(TAG, "Cannot create mutex, ho heap left...");
            }

            xMutexMinFreeHeap = xSemaphoreCreateMutex();
            if(xMutexMinFreeHeap == NULL)
            {
                ESP_LOGE(TAG, "Cannot create mutex, ho heap left...");
            }
        }

        void setCurrentFreeHeapPercent(int percent){
            SemaphoreGuard lock(xMutexFreeHeap);
            _currentHeapFreePercent = percent;
        }

        void setMinHeapFreePercent(int percent){
            SemaphoreGuard lock(xMutexMinFreeHeap);
            _minHeapFreePercent = percent;
        }

        auto getCurrentFreeHeapPercent(){
            SemaphoreGuard lock(xMutexFreeHeap);
            return _currentHeapFreePercent;
        }

        auto getMinHeapFreePercent(){
            SemaphoreGuard lock(xMutexMinFreeHeap);
            return _minHeapFreePercent;
        }

    private:
        constexpr static const char *TAG = "DeviceMemory";
        
        int _currentHeapFreePercent = 0;
        int _minHeapFreePercent = 0;
        SemaphoreHandle_t xMutexFreeHeap;
        SemaphoreHandle_t xMutexMinFreeHeap;
    };
};