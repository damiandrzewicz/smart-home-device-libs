#include "RtosUtils/BinarySemaphoreGuard.hpp"

#include "esp_log.h"

static const char *TAG = "BinarySemaphoreGuard";

BinarySemaphoreGuard::BinarySemaphoreGuard(SemaphoreHandle_t handle)
{
    _handle = handle;
    if(xSemaphoreTake( _handle, portMAX_DELAY ) != pdTRUE)
    {
        ESP_LOGW(TAG, "Cannot take binary semaphore!");
    }
}

BinarySemaphoreGuard::~BinarySemaphoreGuard()
{
    if(xSemaphoreGive( _handle ) != pdTRUE)
    {
        ESP_LOGW(TAG, "Cannot give binary semaphore!");
    }
}