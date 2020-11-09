#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class BinarySemaphoreGuard
{
public:
    BinarySemaphoreGuard(SemaphoreHandle_t handle);
    ~BinarySemaphoreGuard();

private:
    SemaphoreHandle_t _handle;
};