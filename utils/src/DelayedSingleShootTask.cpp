#include "RtosUtils/DelayedSingleShootTask.hpp"

#include "esp_log.h"

static const char *TAG = "DelayedSingleShootTask";

DelayedSingleShootTask::DelayedSingleShootTask(const char *name, unsigned portBASE_TYPE priority, unsigned int delay, unsigned short stackDepth)
    : Task(name, priority, stackDepth), _delay(delay)
{

}

void DelayedSingleShootTask::setDelay(unsigned int delay)
{
    _delay = delay;
}

void DelayedSingleShootTask::execute()
{
    ESP_LOGD(TAG, "Start: name:[%s], prio:[%d], delay:[%d ms] stackSize:[%d]", 
        _name,
        _priority,
        _delay,
        _stackDepth);

    sleep(_delay);

    task();
}