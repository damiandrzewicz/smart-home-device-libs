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

void Task::initTask()
{
    ESP_LOGD(TAG, "Init Not Implemented: [%s]", _name);
}

void Task::start()
{
    BaseType_t xReturned = xTaskCreate(&taskWrapper, _name, _stackDepth, this, _priority, &_handle);

    if(xReturned == pdPASS){ ESP_LOGD(TAG, "Task [%s] created!", _name); }
    else if(xReturned == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY){ ESP_LOGD(TAG, "Cannot create task [%s]! Too less memory!", _name); }
}

void Task::stop()
{
    _running = false;
}
    
void Task::taskWrapper(void *param)
{
    auto task = static_cast<Task*>(param);
    task->_running = true;

    ESP_LOGD(TAG, "Cal init task: [%s]", task->_name);
    task->initTask();
    xEventGroupSetBits(task->_events, TaskBit::Initialized);

    ESP_LOGD(TAG, "Cal exec task: [%s]", task->_name);
    task->execute();

    task->_running = false;
    xEventGroupSetBits(task->_events, TaskBit::Executed);
    task->deleteTask();
}

bool Task::checkInitialized(bool block)
{
    TickType_t tickstoWait = block ? portMAX_DELAY : 0;

    EventBits_t bits = xEventGroupWaitBits(
        _events,
        TaskBit::Initialized,
        pdFALSE,
        pdFALSE,
        tickstoWait);

    return (bits & TaskBit::Initialized);
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

bool Task::isInitialized()
{
    return checkInitialized(false);
}

bool Task::isExecuted()
{
    return checkExecuted(false);
}

void Task::waitForInitialized()
{
    checkInitialized(true);
}

void Task::waitForExecuted()
{
    checkExecuted(true);
}

void Task::sleep(unsigned int delay)
{
    vTaskDelay(pdMS_TO_TICKS(delay));
}

void Task::deleteTask()
{
    ESP_LOGD(TAG, "Deleting task: [%s]", _name);
    vTaskDelete(NULL);
}