#include "RtosUtils/SingleShootTask.hpp"

#include "esp_log.h"

static const char *TAG = "SingleShootTask";

SingleShootTask::SingleShootTask(const char *name, unsigned portBASE_TYPE priority, unsigned short stackDepth)
    : Task(name, priority, stackDepth)
{

}

void SingleShootTask::execute()
{
    ESP_LOGD(TAG, "Start: name:[%s], prio:[%d], stackSize:[%d]", 
        _name,
        _priority,
        _stackDepth);

    task();
}