#include "SystemUtils/SystemLogMessageRouter.hpp"

#include "esp_log.h"

static const char *TAG = "SystemLogMessageRouter";

std::list<SystemLogMessageRouter::RouteCallback> SystemLogMessageRouter::_systemListMessageRouteCallbacks;
char SystemLogMessageRouter::buffer[1024];
SemaphoreHandle_t SystemLogMessageRouter::_xMutex;

void SystemLogMessageRouter::init()
{
    _xMutex = xSemaphoreCreateMutex();
    if(_xMutex == NULL)
    {
        ESP_LOGE(TAG, "Cannot create mutex, ho heap left...");
    }

    _systemListMessageRouteCallbacks.push_back([](const char *buffer){
        printf(buffer);
    });

    esp_log_set_vprintf(systemLogMessageRouter);
}

void SystemLogMessageRouter::appendRouteCallback(RouteCallback clbck)
{
    _systemListMessageRouteCallbacks.push_back(clbck);
}

int SystemLogMessageRouter::systemLogMessageRouter(const char* fmt, va_list tag)
{
    SemaphoreGuard lock(_xMutex);
    int ret = vsprintf(buffer, fmt, tag);
    for(auto &clbck : _systemListMessageRouteCallbacks)
    {
        clbck(buffer);
    }
    return ret;
}