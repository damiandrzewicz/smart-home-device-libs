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

        void setCurrentFreeHeap(int percent){
            SemaphoreGuard lock(xMutexFreeHeap);
            _currentHeapFree = percent;
        }

        void setMinHeapFree(int percent){
            SemaphoreGuard lock(xMutexMinFreeHeap);
            _minHeapFree = percent;
        }

        auto getCurrentFreeHeap(){
            SemaphoreGuard lock(xMutexFreeHeap);
            return _currentHeapFree;
        }

        auto getMinHeapFree(){
            SemaphoreGuard lock(xMutexMinFreeHeap);
            return _minHeapFree;
        }

    private:
        constexpr static const char *TAG = "DeviceMemory";
        
        int _currentHeapFree = 0;
        int _minHeapFree = 0;
        SemaphoreHandle_t xMutexFreeHeap;
        SemaphoreHandle_t xMutexMinFreeHeap;
    };
};