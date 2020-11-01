#include "RtosUtils/Task.hpp"

#include "esp_log.h"

static const char *TAG = "Task";


Task::Task(const char *name, unsigned portBASE_TYPE priority, unsigned short stackDepth)
{
    _name = name;
    _priority = priority;
    _stackDepth = stackDepth;
    _events = xEventGroupCreate();
}

Task::~Task()
{
    stop();
}

void Task::start()
{
    xTaskCreate(&taskWrapper, _name, _stackDepth, this, _priority, &_handle);
}

void Task::stop()
{
    _running = false;
}
    
void Task::taskWrapper(void *param)
{
    auto task = static_cast<Task*>(param);
    task->_running = true;
    task->execute();
    task->_running = false;
    task->deleteTask();
    xEventGroupSetBits(task->_events, TaskBit::Executed);
}

bool Task::checkExecuted(bool block)
{
    TickType_t tickstoWait = block ? portMAX_DELAY : 0;

    EventBits_t bits = xEventGroupWaitBits(
        _events,
        TaskBit::Executed,
        pdFALSE,
        pdFALSE,
        tickstoWait);

    return (bits & TaskBit::Executed);
}

bool Task::isExecuted()
{
    return checkExecuted(false);
}

void Task::wait()
{
    checkExecuted(true);
}

void Task::deleteTask()
{
    ESP_LOGD(TAG, "Deleting task: [%s]", _name);
    vTaskDelete(NULL);
}