#pragma once

#include <string>
#include <functional>
#include <list>

class SystemLogMessageRouter
{
public:

    typedef std::function<void(const char *buffer)> RouteCallback;

    static void init();

    static void appendRouteCallback(RouteCallback clbck);

    static int systemLogMessageRouter(const char* fmt, va_list tag);

private:

    static std::list<RouteCallback> _systemListMessageRouteCallbacks;

    static char buffer[1024];
};