#include "RtosUtils/SemaphoreGuard.hpp"

#include "esp_log.h"

static const char *TAG = "SemaphoreGuard";

SemaphoreGuard::SemaphoreGuard(SemaphoreHandle_t handle)
{
    _handle = handle;
    if(xSemaphoreTake( _handle, portMAX_DELAY ) != pdTRUE)
    {
        ESP_LOGW(TAG, "Cannot take binary semaphore!");
    }
}

SemaphoreGuard::~SemaphoreGuard()
{
    if(xSemaphoreGive( _handle ) != pdTRUE)
    {
        ESP_LOGW(TAG, "Cannot give binary semaphore!");
    }
}