#include "RtosUtils/RoutineTask.hpp"

#include "esp_log.h"

static const char *TAG = "RoutineTask";

RoutineTask::RoutineTask(const char *name, unsigned portBASE_TYPE priority, unsigned short delay, unsigned short stackDepth)
    : Task(name, priority, stackDepth)
{
    _delay = delay;
}

void RoutineTask::execute()
{
    ESP_LOGD(TAG, "Start: name:[%s], prio:[%d], stackSize:[%d], delay: [%d ms]", 
        _name,
        _priority,
        _stackDepth,
        _delay);

    while(_running)
    {
        task();
        sleep(_delay);
    }
}

