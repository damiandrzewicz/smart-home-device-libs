#include "SystemUtils/SystemLogMessageRouter.hpp"

#include "esp_log.h"

std::list<SystemLogMessageRouter::RouteCallback> SystemLogMessageRouter::_systemListMessageRouteCallbacks;
char SystemLogMessageRouter::buffer[1024];

void SystemLogMessageRouter::init()
{
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
    int ret = vsprintf(buffer, fmt, tag);
    for(auto &clbck : _systemListMessageRouteCallbacks)
    {
        clbck(buffer);
    }
    return ret;
}