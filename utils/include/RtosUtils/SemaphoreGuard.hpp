#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class SemaphoreGuard
{
public:
    SemaphoreGuard(SemaphoreHandle_t handle);
    ~SemaphoreGuard();

private:
    SemaphoreHandle_t _handle;
};